#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lecture.h"
#include "execution.h"
#include "analyse.h"
#include "errno.h"

#define TAILLE_TAMPON_LECTURE 64


int main(int argc, char* argv[]){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int option_e = 0;
    int option_r = 0;
    int mode_non_interactif = 0;

    FILE* file;

    int valeur_retour = 0;

    if (argc > 3) {
        errno = 7;
        perror("Error ");
        exit(7);
    }

    if (argc == 2) {    
        if (strcmp(argv[1],"-e") == 0) {
            option_e = 1;
        }
        else if (strcmp(argv[1],"-r") == 0) {
            option_r = 1;
        } 
        else { // Mode non interactif 
            mode_non_interactif = 1;
            file = freopen(argv[1],"r",stdin);
            if ( file == NULL ) {
                perror("Error ");
                return -1;
            }
        }
    }

    if (argc == 3) {
        if ((strcmp(argv[1],"-e") && strcmp(argv[2],"-r")) || (strcmp(argv[2],"-e") && strcmp(argv[1],"-r")))  {
            option_e = 1;
            option_r = 1;
        } else if (strcmp(argv[2], "-e")){
            mode_non_interactif = 1;
            file = freopen(argv[1],"r",stdin);
            if ( file == NULL ) {
                perror("Error ");
                return -1;
            }
        } else {
            option_e = 1;
            file = freopen(argv[1],"r",stdin);
            if ( file == NULL ) {
                perror("Error ");
                return -1;
            }
        }
    }

    if (isatty(0) != 1) {
        mode_non_interactif = 1;
    }
        

    while (!fini) {

    	//Affiche le prompt si on est en mode interactif
        if (!mode_non_interactif) {
            gethostname(hostname, 128);
            hostname[127] = '\0';
            getcwd(pwd, 1024);
            pwd[1023] = '\0';
            printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        }
    	

        //Lecture
        char* commande = lecture();

        if (commande == NULL) {
            return -1;
        }

        if (commande != NULL && commande[0] != '\0') {

            //Analyse
            char** parsed = analyse(commande);

            //Exécution
            valeur_retour = execution(parsed);
        }

        

        if (option_e == 1) {
            if (valeur_retour != 0) {
                printf("%d",valeur_retour);
                return 0;
            }
        }

        if (mode_non_interactif == 1 && feof(file)) {
            return 0;
        } else if (mode_non_interactif == 1 && option_e == 1) {
            if (valeur_retour != 0){
                return 0;
            }
        }
    }
}