#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lecture.h"
#include "execution.h"
#include "parser.h"
#include "lexer.h"

extern commande* commande_lue;
extern int yyparse(void);

int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int erreur_analyse;

    while (!fini) {

        printf("\t]] -- Tour de boucle main --\n");
    	//Affiche le prompt
    	gethostname(hostname, 128);
    	hostname[127] = '\0';
    	getcwd(pwd, 1024);
    	pwd[1023] = '\0';
    	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        fflush(stdout);

        //Lecture & Analyse
        //erreur_analyse = yyparse();
		yy_scan_string(lecture());
        erreur_analyse = yyparse();

        //Exécution
        if (!erreur_analyse) {
            execution(commande_lue,0);
        } else {
            printf("\t]] ! Apparement il y a eu une erreur d'analyse\n");
            printf("\t]] ! Code de retour de yyparse() : %d\n",erreur_analyse);
        }
    }
}
