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

    int valeur_retour = 0;

    if (argc > 2) {
        errno = 7;
        perror("Error ");
        exit(7);
    }

    if (strcmp(argv[1],"-e")) {
        option_e = 1;
    }

    while (!fini) {

    	//Affiche le prompt
    	gethostname(hostname, 128);
    	hostname[127] = '\0';
    	getcwd(pwd, 1024);
    	pwd[1023] = '\0';
    	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);


        //Lecture
        char* commande = lecture();

        //Analyse
        char** parsed = analyse(commande);

        //Exécution
        valeur_retour = execution(parsed);

        if (option_e == 1) {
            if (valeur_retour != 0) {
                return 0;
            }
        }

    }
}
