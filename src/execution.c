#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int execution(char** parsed) {
    pid_t pid;
    int status;
    int errexec;

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
            while (!WIFEXITED(status) && !WIFSIGNALED(status)){
                waitpid(-1, &status, 0);
            }
        }
    }
    return 1;
}
