#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "execution.h"
#include "verbose.h"

int execution(commande* c) {

    debug ("execution()");
    if (c != NULL) {
        switch(c->sep) {
            case SEMICOLUMN:
                NULL;
                int res = execution_and_or(c->liste);
                if (c->suivante == NULL) {
                    return res;
                } else {
                    return execution(c->suivante);
                }
            case AMPERSAND:
                NULL;
                pid_t pid = fork();
                if (pid == 0) {
                    exit(execution_and_or(c->liste));
                } else {
                    printf("[%d]\n",pid);
                    fflush(stdout);
                    if (c->suivante == NULL) {
                        return 0;
                    } else {
                        return execution(c->suivante);
                    }
                }
            case SEP_RIEN:
                if (c->suivante != NULL) {
                    debug("Commande non nul après un séparateur nul !");
                }
                return execution_and_or(c->liste);
        }
    }
    return -1;
}

int execution_and_or(liste_and_or* l) {

    debug("execution_and_or()");

    if (l->precedente != NULL) {

        int res = execution_and_or(l->precedente);

        // Les deux seuls cas ou on doit sauter la commande suivante :
        //
        //      -La commande précédente a réussi et c'est un OR
        //      -La commande précédente a échoué et c'est un AND
        //
        if ((res == 0 && l->op == OR) || (res != 0 && l->op == AND)) {
            return res;
        } else {
            return execution_pipe(l->liste);
        }

    } else {

        return execution_pipe(l->liste);
    
    }
    return -1;
}

int execution_pipe(liste_pipe* l) {

    debug("execution_pipe()");

    if(l->suivante == NULL) {

        int res = execution_redirigee(l->commande);
        return res;

    } else {

        pid_t pid = fork();
        if(pid == 0) {
            exit(execution_pipe_(l, NULL));
        } else {
            int status;
            if ( waitpid(pid, &status, 0) == -1 ) {
                perror("waitpid() failed");
                exit(EXIT_FAILURE);
            }
            if ( WIFEXITED(status) ) {
                int es = WEXITSTATUS(status);
                return es;
            }
        }
    }
    return -1;

}

int execution_pipe_(liste_pipe* l, int pipe_in[2]) {

    if (l->suivante == NULL) {

        if(pipe_in != NULL) {
            dup2(pipe_in[0],0);
            close(pipe_in[0]);
        }
        int res = execution_redirigee(l->commande);
        return res;

    } else {

        int pipe_out[2];
        pipe(pipe_out);
        pid_t pid = fork();
        if (pid == 0) {

            close(pipe_out[0]);

            if(pipe_in != NULL) {
                dup2(pipe_in[0],0);
                close(pipe_in[0]);
            }
            dup2(pipe_out[1],1);
            close(pipe_out[1]);
            exit(execution_redirigee(l->commande));

        } else {

            close(pipe_out[1]);
            return execution_pipe_(l->suivante, pipe_out);

        }

    }
    return -1;

}


int execution_redirigee(commande_redirigee* c) {

    debug("execution_redirigee()");
    if (c->fichier != NULL) {

        pid_t pid = fork();
        if (pid == 0) { 
            switch (c->red) {
                case REDIR_INPUT: // <
                    NULL; // ON sait pas pk mais gcc m'empèche de définir une variable juste après un case:
                    freopen(c->fichier, "r", stdin);
                    exit(execution_simple(c->commande));
                case REDIR_OUTPUT: // >
                    freopen(c->fichier, "w", stdout);
                    exit(execution_simple(c->commande));
                case APPEND: // >>
                    freopen(c->fichier, "a", stdout);
                    exit(execution_simple(c->commande));
                case RED_RIEN:
                    debug("Fichier non nul après un redirecteur vide !");
                    exit(-1);
                default:
                    debug("wtf ? (commande_redirigee)");
                    exit(-1);
            }
        } else {
            int status;
            if ( waitpid(pid, &status, 0) == -1 ) {
                perror("waitpid() failed");
                exit(EXIT_FAILURE);
            }
            if ( WIFEXITED(status) ) {
                int es = WEXITSTATUS(status);
                return es;
            }
        }
    } else {
        return execution_simple(c->commande);
    }
    return -1;
}

int execution_simple(commande_simple* c) {

    debug("execution_simple()");

    pid_t pid;
    int errexec;

    char** parsed = get_NULL_terminated_form(c);

    /******************************
    *        build-in 1 :         *
    *                             *
    *             cd              *
    ******************************/
    if (strcmp(parsed[0],"cd")==0) {
        ultra("build-in : cd");
        if (parsed[1] != NULL) {
            if (strcmp(parsed[1], "~") == 0) {
                ultra("build-in : cd ~ spécifié");
                chdir(getenv("HOME"));
            } else {
                ultra("build-in : cd path spécifié");
                ultra(parsed[1]);
                chdir(parsed[1]);
            }
        } else {
            ultra("build-in : cd sans path");
            chdir(getenv("HOME"));
        }
        free(parsed);
        return 0;
    }

    /******************************
    *        build-in 2 :         *
    *                             *
    *            exit             *
    ******************************/
    if (strcmp(parsed[0],"exit")==0) {
        ultra("build-in : exit");
        free(parsed);
        exit(0);
    }

    /******************************
    *        build-in 3 :         *
    *                             *
    *             fg              *
    ******************************/
    if (strcmp(parsed[0],"fg")==0) {
        ultra("build-in : fg");
        if(parsed[1] == NULL) {
            free(parsed);
            ultra("build-in : fg sans pid");
            int status;
            int pid_retour = wait(&status);
            if ( pid_retour == -1 ) {
                perror("wait() failed");
                exit(EXIT_FAILURE);
            }
            if ( WIFEXITED(status) ) {
                int es = WEXITSTATUS(status);
                printf("[%d->%d]\n",pid_retour,es);
                return 0;
            }
        } else {
            ultra("build-in : fg avec pid");
            int status;
            if ( waitpid(atoi(parsed[1]),&status,0) == -1 ) {
                perror("waitpid() failed");
                free(parsed);
                exit(EXIT_FAILURE);
            }
            if ( WIFEXITED(status) ) {
                int es = WEXITSTATUS(status);
                printf("[%d->%d]\n",atoi(parsed[1]),es);
                free(parsed);
                fflush(stdout);
                return 0;
            }
        }
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Erreur dans %d\n", getpid());
        perror("Erreur lors du fork");
        return -1;
    }else if (pid == 0) {
        errexec = execvp(parsed[0], parsed);
        if (errexec == -1) {
            perror("Erreur lors de l'exécution");
            exit(-1);
        }
    }else{ 
        int status;

        if ( waitpid(pid, &status, 0) == -1 ) {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }

        if ( WIFEXITED(status) ) {
            int es = WEXITSTATUS(status);
            return es;
        }
    }
    return -1;
}