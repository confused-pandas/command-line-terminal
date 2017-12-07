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

    void* scanner;

    while (!fini) {

        printf("Tour de boucle main\n");
    	//Affiche le prompt
    	gethostname(hostname, 128);
    	hostname[127] = '\0';
    	getcwd(pwd, 1024);
    	pwd[1023] = '\0';
    	printf("%s@%s:%s$ ",getenv("USER"),hostname,pwd);
        fflush(stdout);

        //Lecture & Analyse

        printf("Initialisation du scanner : ");
        fflush(stdout);
        scanner = NULL;
        yylex_init(& scanner);
        printf("OK\n");
        fflush(stdout);

        printf("Lancement de l'analyse : ");
        fflush(stdout);
        erreur_analyse = yyparse(scanner);
        printf("OK\n");
        fflush(stdout);

        printf("Destruction du scanner : ");
        fflush(stdout);
        yylex_destroy(scanner);
        printf("OK\n");
        fflush(stdout);

        //Exécution
        if (!erreur_analyse) {
            execution(commande_lue);
        } else {
            printf("Je n'execute pas la commande apparement il y a eu une erreur\n");
            printf("Code de retour de yyparse() : %d\n\n",erreur_analyse);
        }

    }
}
