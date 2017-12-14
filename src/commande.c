#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commande.h"

void cs_append(commande_simple* commande_simple, char* mot) {
	append(commande_simple->commande,mot);
}

commande_simple* new_commande_simple() {
	commande_simple* cs = (commande_simple*) malloc(sizeof(commande_simple));
	cs->commande = new_string_vector();
	return cs;
}


commande_redirigee* new_commande_redir() {
	commande_redirigee* cr = (commande_redirigee*) malloc(sizeof(commande_redirigee));
	return cr;
}

liste_pipe* new_pipe() {
	liste_pipe* lp = (liste_pipe*) malloc(sizeof(liste_pipe));
	return lp;
}

liste_and_or* new_and_or() {
	liste_and_or* lao = (liste_and_or*) malloc(sizeof(liste_and_or));
	return lao;
}

commande* new_commande() {
	commande* cm = (commande*) malloc(sizeof(commande));
	return cm;
}