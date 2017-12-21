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
    //TODO : CHANGER CA
    return execution_and_or(c->liste);
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

    switch (c->red) {
        case REDIR_INPUT: // <
            NULL; // ON sait pas pk mais gcc m'empèche de définir une variable juste après un case:
            freopen(c->fichier, "r", stdin);
            return execution_simple(c->commande);
        case REDIR_OUTPUT: // >
            freopen(c->fichier, "w", stdout);
            return execution_simple(c->commande);
        case APPEND: // >>
            freopen(c->fichier, "a", stdout);
            return execution_simple(c->commande);
        case RED_RIEN:
            if (c->fichier != NULL) {
                debug("Fichier non nul après un redirecteur vide !");
            }
            return execution_simple(c->commande);
        default:
            debug("wtf ? (commande_redirigee)");
            return -1;
    }
    return -1;
}

int execution_simple(commande_simple* c) {

    debug("execution_simple()");
    
    char** parsed = c->mots;

    pid_t pid;
    int errexec;

    if (strcmp(parsed[0],"cd")==0){
        if (parsed[1] != NULL) {
            if (strcmp(parsed[1], "~") == 0) {
                chdir(getenv("HOME"));
            } else {
                chdir(parsed[1]);
            }
        } else {
            chdir(getenv("HOME"));
        }
        return 0;
    }
    else{
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
    }
    return -1;
}