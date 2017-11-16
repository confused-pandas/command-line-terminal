#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "execution.h"

#define TAILLE_TAMPON_EXECUTION 32

int execution(char** parsed) {

    int position = 0;

    while (parsed[position] != NULL) {
        int taille_commande = TAILLE_TAMPON_EXECUTION;
        int position_commande = 0;
        char** commande_simple = (char**) malloc(sizeof(char*)*taille_commande); 
        if (commande_simple == NULL) {
            printf("Erreur lors de l'allocation initiale du tampon d'éxecution\n");
            exit(105); /* No buffer space available */
        }

        int fini=0;

        while(!fini) {
            switch (read_token(parsed[position])) {
                case SEMICOLUMN:
                    commande_simple[position_commande] = NULL;
                    execution_simple(commande_simple);
                    fini = 1;
                    break;
                case DEFAULT:
                    commande_simple[position_commande] = parsed[position];
                    break;
                case END:
                    commande_simple[position_commande] = NULL;
                    return execution_simple(commande_simple);
            }

            position++;
            position_commande++;

            if (position_commande >= taille_commande) {
                taille_commande += TAILLE_TAMPON_EXECUTION;
                commande_simple = realloc(commande_simple,taille_commande);
                if (commande_simple == NULL) {
                    printf("Erreur lors de la réallocation du tampon d'éxecution\n");
                    exit(105); /* No buffer space available */
                }

            }
        }
    }
    return 0;
}


delimiteur read_token(char* token) {
    if (token == NULL) {
        return END;
    }else if (strcmp(token, ";") == 0){
        return SEMICOLUMN;
    }else if (strcmp(token, "||") == 0){
        return OR;
    }else if (strcmp(token, "&&") == 0){
        return AND;
    }else if (strcmp(token, "|") == 0){
        return PIPE;
    }else if (strcmp(token, "<") == 0){
        return REDIR_INPUT;
    }else if (strcmp(token, "<<") == 0){
        return HEREDOC;
    }else if (strcmp(token, ">") == 0){
        return REDIR_OUTPUT;
    }else if (strcmp(token, ">>") == 0){
        return APPEND;
    } else {
        return DEFAULT;
    }
}


int execution_simple(char** parsed) {
    pid_t pid;
    int status;
    int errexec;
    if (parsed == NULL) {
        return 0;
    }
    if (strcmp(parsed[0],"cd")==0){
        chdir(parsed[1]);  
    }
    else{
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Erreur dans %d\n", getpid());
            perror("Erreur lors du fork");
            exit(1);
        }else if (pid == 0) {
            errexec = execvp(parsed[0], parsed);
            if (errexec == -1) {
                perror("Erreur lors de l'exécution");
            }
            exit(EXIT_FAILURE);
        }else{ 
            wait(NULL);
        }
    }
    return 1;
}
