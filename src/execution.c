#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "execution.h"
#include "commande.h"
#include "verbose.h"

#define TAILLE_TAMPON_EXECUTION 32

char* chemin_fifo_pipe = "tmp/teshFifoPipe";
char* chemin_fifo_red = "tmp/teshFifoRed";
FILE* fifo_pipe = NULL;
FILE* fifo_red = NULL;
FILE* fd = NULL;
int stdin_copy;
int stdout_copy;

int execution(commande* c, int niveau) {

	stdin_copy = dup(0);
	stdout_copy = dup(1);

	// Si c'est la toute première commande il faut creer le fifo
    if (niveau == 0) {
        remove(chemin_fifo_pipe);
        if (mkfifo(chemin_fifo_pipe,0666) != 0) {
            verbose("Erreur lors de la création du fifo du pipe");
            return -1;
        }
    }

    fifo_pipe = fopen(chemin_fifo_pipe,"r+");

    remove(chemin_fifo_red);
    if (mkfifo(chemin_fifo_red,0666) != 0) {
        verbose("Erreur lors de la création du fifo de commande redirigée");
        return -1;
    }

    fifo_red = fopen(chemin_fifo_red,"r+");

    debug("execution()");
    int resultat;
    switch (c->sep) {
        case SEMICOLUMN:
            debug("vu : ;");
            resultat = execution_and_or(c->l);
            if(c->suivante == NULL) {
                return resultat;
            } else {
                return execution(c->suivante,niveau+1);
            }
            break;
        case AMPERSAND:
            debug("vu : &");
            //TODO : a implementer
            break;
        case SEP_RIEN:
            debug("vu : (SEP_RIEN)");
            resultat = execution_and_or(c->l);
            if (c->suivante != NULL) {
                verbose("Erreur : commande non nulle après séparateur de terminaison");
                return -1;
            } else {
                return resultat;
            }
            break;
    }
    return -1;
}


int execution_and_or(liste_and_or* l) {

	debug("execution_and_or()");

    char* true = "true";
    commande_simple true_simple = {&(true)};
    commande_redirigee true_redirige = {&(true_simple),RED_RIEN,NULL};
    liste_pipe true_liste_pipe = {&(true_redirige),NULL};

    char* false = "false";
    commande_simple false_simple = {&(false)};
    commande_redirigee false_redirige = {&(false_simple),RED_RIEN,NULL};
    liste_pipe false_liste_pipe = {&(false_redirige),NULL};

    int resultat;

    switch (l->op) {
        case OR: /* || */
            debug("vu : ||");
            resultat = execution_pipe(l->liste);
            if (resultat == 0) {
                // ça a marché, pas besoin de faire la commande suivante
                // on remplace la commande suivante par true et on execute la suite
                *(l->suivante->liste) = true_liste_pipe;
            }
            return execution_pipe(l->suivante->liste);
            break;
        case AND: /* && */
            debug("vu : &&");
            resultat = execution_pipe(l->liste);
            if (resultat != 0) {
                // la première n'a pas marché, pas besoin de faire la suivante pour savoir que le AND
                // sera faux, on remplace la suivante par false et on continue
                *(l->suivante->liste) = false_liste_pipe;
            }
            return execution_pipe(l->suivante->liste);
            break;
        case OP_RIEN: /* si c'est fini */
            debug("vu : (OP_RIEN)");
            resultat = execution_pipe(l->liste);
            if (l->suivante != NULL) {
                verbose("Erreur : commande non nulle après séparateur de terminaison");
                return -1;
            } else {
                return resultat;
            }
            break;
    }
    return -1;
}

int execution_pipe(liste_pipe* l) {

	debug("execution_pipe()");

    /* Bon j'ai pas envie de faire des pipe n'importe comment
    Je vais utiliser un tube nommé je pense ça sera plus propre*/
	
	// Si c'est la deuxième fois qu'on appelle execution_pipe() dans le même appel à execution()
	// Le fifo du pipe a normalement été fermé et son FILE* remit à NULL à la fin de execution_simple()
	// Il faut donc le réouvrir
	if (fifo_pipe == NULL) {
		/*
        if (mkfifo(chemin_fifo_pipe,0666) != 0) {
            verbose("Erreur lors de la création du fifo du pipe");
            return -1;
        }
		*/
		debug("> réouverture du fifo du pipe");
        fifo_pipe = fopen(chemin_fifo_pipe,"r+");
	}


    // Si il n'y a plus de commandes après
    if (l->suivante == NULL) {
        debug("vu : Pas de pipe suivant");
        dup2(fileno(fifo_pipe),0);
        dup2(stdout_copy,1);
        return execution_redirigee(l->commande);

    } else {
        debug("vu : |");
        dup2(fileno(fifo_pipe),0);
        dup2(fileno(fifo_pipe),1);
        execution_redirigee(l->commande);
        execution_pipe(l->suivante);

    }
    return -1;
}

int execution_redirigee(commande_redirigee* c) {

	debug("execution_redirigee()");

	if (fifo_red == NULL) {
		/*
        if (mkfifo(chemin_fifo_red,0666) != 0) {
            verbose("Erreur lors de la création du fifo redirecteur");
            return -1;
        }
		*/
		debug("> réouverture du fifo de redirection");
        fifo_red = fopen(chemin_fifo_red,"r+");
	}

    if (c->fichier != NULL) {
        switch(c->red) {
            case REDIR_INPUT:    /* < */
                debug("vu : <");
                {
                //bonjour l'efficacité : j'écris TOUT le fichier sur mon fifo transitoire
                fd = fopen(c->fichier, "r");
                char buffer[1];
                while(read(fileno(fd), buffer, 1) > 0) {
                    write(fileno(fifo_red), buffer, 1);
                }
                //puis TOUT ce qu'il y avait dans le fifo du dessus dans mon fifo
                while(read(0, buffer, 1) > 0) {
                    write(fileno(fifo_red), buffer, 1);
                }
                dup2(fileno(fifo_red),0);
                return execution_simple(c->commande);
                break;
                }
            case REDIR_OUTPUT:   /* > */
                debug("vu : >");
                {
                fd = fopen(c->fichier, "w");
                dup2(fileno(fd),1);
                return execution_simple(c->commande);
                break;
                }
            case APPEND:         /* >> */
                debug("vu : >>");
                {
                fd = fopen(c->fichier, "a");
                dup2(fileno(fd),1);
                return execution_simple(c->commande);
                break;
                }
            case RED_RIEN:       /* si pas de redirection */
                debug("vu : (RED_RIEN)");
                verbose("Erreur : commande non nulle après redirecteur nul");
                return -1;
        }
    } else {
        debug("vu : (RED_RIEN)");
        return execution_simple(c->commande);
    }

    return -1;
}

int execution_simple(commande_simple* c) {

	debug("execution_simple()");

    /*
    Renvoie 0 si tout s'est bien passé,
    n'importe quel autre nombre sinon
    */

    pid_t pid;
    int status;

    // Si la commande est vide, on ne fait rien
    if (c->commande == NULL) {
        debug("c->commande == NULL");
        return 0;
    }

    // On traite le cd séparément
    // ATTENTION ET SI IL FAUT LIRE LES PARAMETRES DU CD SUR L'ENTREE STANDARD ?
    // à faire pour plus tard ...
    if (strcmp(c->commande[0],"cd")==0){
		int erreur;
        if (c->commande[1] != NULL) {
            if (strcmp(c->commande[1], "~") == 0) {
                erreur = chdir(getenv("HOME"));
            } else {
                erreur = chdir(c->commande[1]);
            }
        } else {
            erreur = chdir(getenv("HOME"));
        }
		tout_fermer();
		if (erreur) {
			perror("Erreur : ");		
		}
		return erreur;
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
            debug("execvp(c->commande[0], c->commande)");
            execvp(c->commande[0], c->commande);

            /*
            Si jamais on arrive à cette ligne
            cela signifie que la commande n'est pas parvenue
            à se lancer, on peut renvoyer -1
            */
			perror("Erreur ");
            return -1;

        }

        //Dans le père 
        if ( waitpid(pid, &status, 0) == -1 ) {
            perror("Erreur lors du waitpid ");
			tout_fermer();
            return -1;
        }

        if ( WIFEXITED(status) ) {
            debug("execvp() terminé correctement");
			tout_fermer();
            return WEXITSTATUS(status);
        }
    }
    return -1;
}

void tout_fermer() {
	debug("> fermeture des fifo, remise à 0 de stdin / stdout");
	if (fifo_pipe != NULL) {
		fclose(fifo_pipe);
		fifo_pipe = NULL;	
	}
	if (fifo_red != NULL) {
		fclose(fifo_red);
		fifo_red = NULL;	
	}
	if (fd != NULL) {
		fclose(fd);
		fd = NULL;	
	}
	dup2(stdin_copy, 0);
	dup2(stdout_copy, 1);
	close(stdin_copy);
	close(stdout_copy);
	stdin_copy = dup(0);
	stdout_copy = dup(1);
}
