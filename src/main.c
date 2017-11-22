#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lecture.h"
#include "execution.h"
#include "analyse.h"

#define TAILLE_TAMPON_LECTURE 64


int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

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
        execution(parsed);

    }
}
