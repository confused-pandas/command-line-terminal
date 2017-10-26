#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define TAILLE_TAMPON_LECTURE 64


int main(int argc, char **argv){


    int fini = 0;

    while (!fini) {

        //Lecture
        char* commande = lecture();

        //Analyse


        //Exécution

    }

}

char* lecture() {

    int taille_actuelle = TAILLE_TAMPON_LECTURE;
    int position = 0;

    char* commande;
    commande = (char*) malloc(sizeof(char)*taille_actuelle);
    
    if (commande == NULL) {
        print("Erreur lors de l'allocation du tampon de lecture\n");
        return NULL;
    }

    char c = getchar();
    while (c != EOF) {
        if (position < taille_actuelle) {
            commande[position] = c;
        }
    }

}
