#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define TAILLE_TAMPON_LECTURE 128

char* lecture() {

    int taille_actuelle = TAILLE_TAMPON_LECTURE;
    int position = 0;

    char* commande = (char*) malloc(sizeof(char)*taille_actuelle); 
    if (commande == NULL) {
        printf("Erreur lors de l'allocation initiale du tampon de lecture\n");
        exit(105); /* No buffer space available */
    }
    
    while(1) {

        //On lit un caractère
        char c = getchar();

        //Si l'utilisateur a fini sa commande il appuye sur entrée
        //Ce qui envoie un caractère \n
        if (c == '\n' || c == EOF) {

            //C'est donc le moment de fermer la chaine
            commande[position] = '\0';
            return commande;

        } else {

            commande[position] = c;

        }

        position++;

        //Vu que l'on commence à 0 c'est bien >= et pas >
        if (position >= taille_actuelle) {

            taille_actuelle += TAILLE_TAMPON_LECTURE;
            commande = realloc(commande,taille_actuelle);

            if (commande == NULL) {

                printf("Erreur lors de la réallocation du tampon de lecture\n");
                exit(105); /* No buffer space available */

            }

        }

    }

}