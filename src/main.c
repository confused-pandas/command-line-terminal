#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "lecture.h"
#include "execution.h"
#include "analyse.h"

#define TAILLE_TAMPON_LECTURE 64


int main(int argc, char **argv){


    int fini = 0;

    while (!fini) {

        //Lecture
        char* commande = lecture();

        //Analyse
        char** parsed = analyse(commande);

        //Exécution
        execution(parsed);

    }
}
