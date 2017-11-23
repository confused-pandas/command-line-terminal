#include <stdio.h>
#include <stdlib.h>

#define TAILLE_TAMPON_ANALYSE 128

char** analyse(char* commande) {

	int taille_actuelle = TAILLE_TAMPON_ANALYSE;
	int position = 0;
	int position_commande = 0;
	char** parsed = (char**) malloc(sizeof(char*)*taille_actuelle); 
	if (parsed == NULL) {
		printf("Erreur lors de l'allocation initiale du tampon d'analyse\n");
		exit(105); /* No buffer space available */
	}


	int dernier_mot = 0;
    //Boucle lecture des chaînes de caractères
	do {
		
		int taille_mot = TAILLE_TAMPON_ANALYSE;
		int position_mot = 0;

		char* mot = (char*) malloc(sizeof(char)*taille_mot);
		if (mot == NULL) {
			printf("Erreur lors de l'allocation initiale du tampon d'analyse du mot\n");
			exit(105); /* No buffer space available */
		}

		int fini = 0;
    	//Boucle interne de lecture des mots
		while (!fini) {

    		//On lit un caractère
    		//printf("je commande à lire\n");
			char c = commande[position_commande];

			fflush(stdout);

			switch (c) {
				case ' ':
					mot[position_mot] = '\0';
					fini = 1;
					break;
				case '\0':
					mot[position_mot] = '\0';
					dernier_mot = 1;
					fini=1;
					break;
				default:
					mot[position_mot] = c;
					break;
			}

			position_mot++;
			position_commande++;

        	//Vu que l'on commence à 0 c'est bien >= et pas >
			if (position_mot >= taille_mot) {

				taille_mot += TAILLE_TAMPON_ANALYSE;
				mot = realloc(mot,taille_mot);

				if (mot == NULL) {
					printf("Erreur lors de la réallocation du tampon d'analyse du mot\n");
					exit(105); /* No buffer space available */
				}

			}

		}


        //Si l'utilisateur a fini sa commande il appuye sur entrée
        //Ce qui envoie un caractère \n
		if (dernier_mot) {
            //C'est donc le moment de fermer la chaine
			parsed[position] = mot;

			position++;
			if (position >= taille_actuelle) {

				taille_actuelle += 1;
				parsed = realloc(parsed,taille_actuelle);

				if (parsed == NULL) {

					printf("Erreur lors de la réallocation du tampon de lecture\n");
					exit(105); /* No buffer space available */
				
				}
			}
			parsed[position] = NULL;
			return parsed;

		} else {

			parsed[position] = mot;

		}

		position++;

        //Vu que l'on commence à 0 c'est bien >= et pas >
		if (position >= taille_actuelle) {

			taille_actuelle += TAILLE_TAMPON_ANALYSE;
			parsed = realloc(parsed,taille_actuelle);

			if (parsed == NULL) {

				printf("Erreur lors de la réallocation du tampon de lecture\n");
				exit(105); /* No buffer space available */
				
			}

		}

	} while (1);
}