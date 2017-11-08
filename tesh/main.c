#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "execution.h"
#define TAILLE_TAMPON_LECTURE 64


int main(int argc, char **argv){


    int fini = 0;

    while (!fini) {

        //Lecture
        char* commande = lecture();

        //Analyse


        //Exécution
        int execute = execution();

    }

}

