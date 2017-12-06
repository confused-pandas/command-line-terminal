#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lecture.h"
#include "execution.h"
#include "analyse.h"

extern commande* commande_lue;
extern int yyparse();


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
        //Lecture & Analyse
        yyparse();

        //Exécution
        execution(commande_lue);

    }
}
