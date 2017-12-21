#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include "liberer.h"
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
int option_ultra;
char* prefixe_ultra = "\tUU ";

int option_e = 0;
int option_r = 0;
int option_no_prompt = 0;
int ignore_wrong_path_argument = 1;

int main(int argc, char **argv){

    char* (*readline)(char*);
    void  (*add_history)(char*);
    void* handle;
    int fini = 0;

	if (argc > 1) {
		int i;
		for(i=1;i<argc;i++) {
			if (strcmp(argv[i],"--debug") == 0) {
				option_debug = 1;
			} else if (strcmp(argv[i],"--verbose") == 0) {
				option_verbose = 1;
			} else if (strcmp(argv[i],"--ultra") == 0) {
                option_ultra = 1;
            } else if (strcmp(argv[i],"--no-prompt") == 0) {
                option_no_prompt = 1;
            } else if (strcmp(argv[i],"-e") == 0) {
                option_e = 1;
            } else if (strcmp(argv[i],"-r") == 0) {
                option_r = 1;
                handle = dlopen("libreadline.so", RTLD_LAZY);
                if (handle == NULL) {
                    debug("Readline n'a pas reussi a charger");
                    return -1;
                }
                readline = (char*(*)(char*)) dlsym(handle,"readline");
                add_history = (void (*)(char*)) dlsym(handle,"add_history");
            } else if (is_file(argv[i])) {
                freopen(argv[i], "r", stdin); 
            }
		}
	}

    if(!isatty(0)) {
        option_no_prompt = 1;
    }

    if (option_no_prompt && option_r) {
        debug("Je veux pas faire readline avec un script et puis quoi encore ?");
        return -1;
    }

    while (!feof(stdin) && !fini) {

        char* retour_readline = (char*)malloc(sizeof(char)*1024);
        char* prompt = (char*)malloc(sizeof(char)*1024);

        char* hostname = (char*) malloc(sizeof(char)*128);
        char* pwd = (char*) malloc(sizeof(char)*1024);

        int erreur_analyse;
        char* chaine_retour_erreur = (char*) malloc(sizeof(char)*1024);


        debug("-- Tour de boucle main --");
    	//Affiche le prompt
        if(!option_no_prompt) {
        	gethostname(hostname, 128);
        	getcwd(pwd, 1024);
        	snprintf(prompt,1024,"%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        }

        if (option_r) {
            retour_readline = readline(prompt);
            add_history(retour_readline);
        } else {
            printf("%s",prompt);
            retour_readline = lecture();
        }

        yy_scan_string(retour_readline);
        free(retour_readline);

        // Lecture & Analyse
        erreur_analyse = yyparse();

        // Exécution
        if (!erreur_analyse) {

            int valeur_retour = execution(commande_lue);
            liberer(commande_lue);
            if (valeur_retour != 0 && option_e) {
                fini = 1;
            }

        } else {
            verbose("Apparement il y a eu une erreur d'analyse");
			snprintf(chaine_retour_erreur,1024,"Code de retour de yyparse() : %d",erreur_analyse);
            debug(chaine_retour_erreur);
            free(chaine_retour_erreur);
        }
    }
    return 0;
}