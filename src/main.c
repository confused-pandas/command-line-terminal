#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "lecture.h"
#include "execution.h"
#include "parser.h"
#include "lexer.h"
#include "verbose.h"
#include "is_file.h"

extern commande* commande_lue;
extern int yyparse(void);

int option_verbose;
char* prefixe_verbose = "\t)) ";
int option_debug;
char* prefixe_debug = "\t]] ";

int option_e = 0;
int option_r = 0;
int option_no_prompt = 0;
int ignore_wrong_path_argument = 1;

int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int erreur_analyse;
	char* chaine_retour_erreur = (char*) malloc(sizeof(char));

	if (argc > 1) {
		int i;
		for(i=1;i<argc;i++) {
			if (strcmp(argv[i],"--debug") == 0) {
				option_debug = 1;
			} else if (strcmp(argv[i],"--verbose") == 0) {
				option_verbose = 1;
			} else if (strcmp(argv[i],"--no-prompt") == 0) {
                option_no_prompt = 1;
            } else if (strcmp(argv[i],"-e") == 0) {
                option_e = 1;
            } else if (strcmp(argv[i],"-r") == 0) {
                option_r = 1;
            } else if (is_file(argv[i])) {
                freopen(argv[i], "r", stdin); 
            }
		}
	}

    if(!isatty(0)) {
        option_no_prompt = 1;
    }

    while (!feof(stdin) && !fini) {

        debug("-- Tour de boucle main --");
    	//Affiche le prompt
        if(!option_no_prompt) {
        	gethostname(hostname, 128);
        	hostname[127] = '\0';
        	getcwd(pwd, 1024);
        	pwd[1023] = '\0';
        	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        }

        // Lecture & Analyse
        erreur_analyse = yyparse();

        // Exécution
        if (!erreur_analyse) {

            int valeur_retour = execution(commande_lue);
            if (valeur_retour != 0 && option_e) {
                fini = 1;
            }

        } else {
            verbose("Apparement il y a eu une erreur d'analyse");
			sprintf(chaine_retour_erreur,"Code de retour de yyparse() : %d",erreur_analyse);
            debug(chaine_retour_erreur);
        }
    }
    return 0;
}
