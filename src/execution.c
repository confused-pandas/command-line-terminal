#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "execution.h"

#define TAILLE_TAMPON_EXECUTION 32

char* chemin_fifo_pipe = "/tmp/teshFifoPipe";
char* chemin_fifo_red = "/tmp/teshFifoRed";

int execution(commande* c) {

    int resultat;

    switch (c->sep) {
        case SEMICOLUMN:
            resultat = execution_and_or(&(c->l));
            if(c->suivante == NULL) {
                return resultat;
            } else {
                return execution(c->suivante);
            }
            break;
        case AMPERSAND:
            //TODO : a implementer
            break;
        case SEP_RIEN:
            resultat = execution_and_or(&(c->l));
            if (c->suivante != NULL) {
                printf("Erreur : commande non nulle après séparateur de terminaison");
                return -1;
            } else {
                return resultat;
            }
            break;
    }
    return -1;
}


int execution_and_or(liste_and_or* l) {

    char* true = "true";
    commande_simple true_simple = {&(true)};
    commande_redirigee true_redirige = {true_simple,RED_RIEN,NULL};
    liste_pipe true_liste_pipe = {true_redirige,NULL};

    char* false = "false";
    commande_simple false_simple = {&(false)};
    commande_redirigee false_redirige = {false_simple,RED_RIEN,NULL};
    liste_pipe false_liste_pipe = {false_redirige,NULL};

    int resultat;

    switch (l->op) {
        case OR: /* || */
            resultat = execution_pipe(&(l->liste),0);
            if (resultat == 0) {
                // ça a marché, pas besoin de faire la commande suivante
                // on remplace la commande suivante par true et on execute la suite
                l->suivante->liste = true_liste_pipe;
            }
            return execution_pipe(&(l->suivante->liste),0);
            break;
        case AND: /* && */
            resultat = execution_pipe(&(l->liste),0);
            if (resultat != 0) {
                // la première n'a pas marché, pas besoin de faire la suivante pour savoir que le AND
                // sera faux, on remplace la suivante par false et on continue
                l->suivante->liste = false_liste_pipe;
            }
            return execution_pipe(&(l->suivante->liste),0);
            break;
        case OP_RIEN: /* si c'est fini */
            resultat = execution_pipe(&(l->liste),0);
            if (l->suivante != NULL) {
                printf("Erreur : commande non nulle après séparateur de terminaison\n");
                return -1;
            } else {
                return resultat;
            }
            break;
    }
    return -1;
}

int execution_pipe(liste_pipe* l, int niveau) {

    /* Bon j'ai pas envie de faire des pipe n'importe comment
    Je vais utiliser un tube nommé je pense ça sera plus propre*/

    // Si c'est la toute première commande il faut creer le fifo
    if (niveau == 0) {
        if (mkfifo(chemin_fifo_pipe,0666) != 0) {
            printf("Erreur lors de la création du fifo\n");
            return -1;
        }
    }

    FILE* fifo_pipe = fopen(chemin_fifo_pipe,"r+");

    // Si on est dans le fils
    if (fork() != 0) {


        // Si il n'y a plus de commandes après
        if (l->suivante == NULL) {

            dup2(fileno(fifo_pipe),0);
            dup2(fileno(stdout),1);
            return execution_redirigee(&(l->commande));

        } else {

            dup2(fileno(fifo_pipe),0);
            dup2(fileno(fifo_pipe),1);
            execution_redirigee(&(l->commande));
            execution_pipe(l->suivante, niveau+1);

        }
    }
    return -1;
}

int execution_redirigee(commande_redirigee* c) {

    if (mkfifo(chemin_fifo_red,0666) != 0) {
        printf("Erreur lors de la création du fifo\n");
        return -1;
    }

    FILE* fifo_red = fopen(chemin_fifo_red,"r+");

    if (c->fichier != NULL) {
        switch(c->red) {
            case REDIR_INPUT:    /* < */
                {
                //bonjour l'efficacité : j'écris TOUT le fichier sur mon fifo transitoire
                FILE* fd = fopen(c->fichier, "r");
                char buffer[1];
                while(read(fileno(fd), buffer, 1) > 0) {
                    write(fileno(fifo_red), buffer, 1);
                }
                //puis TOUT ce qu'il y avait dans le fifo du dessus dans mon fifo
                while(read(0, buffer, 1) > 0) {
                    write(fileno(fifo_red), buffer, 1);
                }
                dup2(fileno(fifo_red),0);
                return execution_simple(&(c->commande));
                break;
                }
            case REDIR_OUTPUT:   /* > */
                {
                FILE* fd = fopen(c->fichier, "w");
                dup2(fileno(fd),1);
                return execution_simple(&(c->commande));
                break;
                }
            case APPEND:         /* >> */
                {
                FILE* fd = fopen(c->fichier, "a");
                dup2(fileno(fd),1);
                return execution_simple(&(c->commande));
                break;
                }
            case RED_RIEN:       /* si pas de redirection */
                printf("Erreur : commande non nulle après redirecteur nul\n");
                return -1;
        }
    } else {
        return execution_simple(&(c->commande));
    }

    return -1;
}

int execution_simple(commande_simple* c) {

    /*
    Renvoie 0 si tout s'est bien passé,
    n'importe quel autre nombre sinon
    */

    pid_t pid;
    int status;;

    //Si la commande est vide, on ne fait rien
    if (c->commande == NULL) {
        return 0;
    }

    //On traite le cd séparément
    if (strcmp(c->commande[0],"cd")==0){
        if (c->commande[1] != NULL) {
            if (strcmp(c->commande[1], "~") == 0) {
                chdir(getenv("HOME"));
            } else {
                chdir(c->commande[1]);
            }
        } else {
            chdir(getenv("HOME"));
        }
    }
    //Pour toutes les autres commandes
    else {

        pid = fork();
        
        // Erreur lors du fork
        if (pid < 0) {

            fprintf(stderr, "Erreur dans %d\n", getpid());
            perror("Erreur lors du fork");
            exit(1);

        } else if (pid == 0) {

            //On lance la commande
            execvp(c->commande[0], c->commande);

            /*
            Si jamais on arrive à cette ligne
            cela signifie que la commande n'est pas parvenue
            à se lancer, on peut renvoyer -1
            */
            return -1;

        }

        //Dans le père 
        if ( waitpid(pid, &status, 0) == -1 ) {
            perror("Erreur lors du waitpid");
            return -1;
        }

        if ( WIFEXITED(status) ) {
            return WEXITSTATUS(status);
        }
    }
    return -1;
}
