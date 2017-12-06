#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "execution.h"

extern commande* commande_lue;
extern int yyparse(void);

int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int erreur_analyse;

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
        if (!erreur_analyse) {
            execution(commande_lue);
        }

    }
}
