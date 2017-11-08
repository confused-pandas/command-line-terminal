#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lecture.h"
#define TAILLE_TAMPON_DECOUPE 64

int main(int argc, char **argv){


    int fini = 0;

    while (!fini) {

        //Lecture
        char* commande = lecture();

        printf("%s\n",commande);
        return 0;
        //Analyse
        //char** commande_decoupe = analyse(commande)

        //Exécution

    }

}