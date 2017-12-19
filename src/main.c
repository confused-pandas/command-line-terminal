#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "lecture.h"
#include "execution.h"
#include "parser.h"
#include "lexer.h"
#include "verbose.h"

extern commande* commande_lue;
extern int yyparse(void);

int option_verbose;
char* prefixe_verbose = "\t)) ";
int option_debug;
char* prefixe_debug = "\t]] ";

int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int erreur_analyse;
	char* chaine_retour_erreur;

	if (argc > 1) {
		int i;
		for(i=1;i<argc;i++) {
			if (strcmp(argv[i],"--debug") == 0) {
				option_debug = 1;
			}
			if (strcmp(argv[i],"--verbose") == 0) {
				option_verbose = 1;
			}
		}
	}

    while (!fini) {

        debug("-- Tour de boucle main --");
    	//Affiche le prompt
        if(isatty(0)) {
        	gethostname(hostname, 128);
        	hostname[127] = '\0';
        	getcwd(pwd, 1024);
        	pwd[1023] = '\0';
        	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        }

        //Lecture
		yy_scan_string(lecture());

        //Analyse
        erreur_analyse = yyparse();

        //Exécution
        if (!erreur_analyse) {
            execution(commande_lue,0);
        } else {
            verbose("Apparement il y a eu une erreur d'analyse");
			sprintf(chaine_retour_erreur,"Code de retour de yyparse() : %d",erreur_analyse);
            debug(chaine_retour_erreur);
        }
    }
}
