#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "execution.h"
#include "parser.h"
#include "lexer.h"

extern commande* commande_lue;
extern int yyparse(void* scanner);

int main(int argc, char **argv){

	char hostname[128];
	char pwd[1024];
    int fini = 0;

    int erreur_analyse;

    while (!fini) {

        printf("Tour de boucle main\n");
    	//Affiche le prompt
    	gethostname(hostname, 128);
    	hostname[127] = '\0';
    	getcwd(pwd, 1024);
    	pwd[1023] = '\0';
    	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);

        //Lecture & Analyse
        void* scanner;
        yylex_init(& scanner);
        erreur_analyse = yyparse(scanner);
        yylex_destroy(scanner); 
        printf("\nJ'ai fini l'analyse\n");

        //Exécution
        if (!erreur_analyse) {
            execution(commande_lue);
        } else {
            printf("Je n'execute pas la commande apparement il y a eu une erreur\n");
            printf("Code de retour de yyparse() : %d\n\n",erreur_analyse);
        }

    }
}
