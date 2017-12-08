#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commande.h"

void cs_append(commande_simple* commande_simple, char* mot) {

	int taille_cs = taille(commande_simple->commande);
	commande_simple->commande = realloc(commande_simple->commande, taille_cs+2);
	commande_simple->commande[taille_cs] = strdup(mot);
	commande_simple->commande[taille_cs+1] = NULL;

}

void cs_fusion(commande_simple* debut, commande_simple* suite) {

	int taille_debut = taille(debut->commande);
	int taille_suite = taille(suite->commande);

	debut->commande = realloc(debut->commande, taille_debut+taille_suite+1);
	debut->commande[taille_debut+taille_suite] = NULL;

	int indice_sur_debut = taille_debut;
	int indice_sur_suite = 0;
	while(debut->commande[indice_sur_debut] != NULL) {
		debut->commande[indice_sur_debut] = strdup(suite->commande[indice_sur_suite]);
		indice_sur_debut++;
		indice_sur_suite++;
	}

}

commande_simple* new_commande_simple() {
	commande_simple* cs = (commande_simple*) malloc(sizeof(commande_simple));
	char** vide = (char**) malloc(sizeof(char*));
	vide[0] = NULL;
	cs->commande = vide;
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

int taille(char** tab) {

	int res = 0;
	while(tab[res] != NULL) {
		res++;
	}
	return res;

}