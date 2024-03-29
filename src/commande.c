#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commande.h"

#define TAILLE_DEPART 4

void cs_append(commande_simple* cs, char* mot) {
		// Si on doit réallouer
	if (cs->taille == cs->taille_reelle) {

		cs->taille_reelle += cs->taille_realloc;
		cs->mots = realloc(cs->mots,cs->taille_reelle*sizeof(char*));

		if (cs->mots == NULL) {
			printf("Erreur lors de la réallocation d'un string_vector\n");
			exit(105); /* No buffer space available */
		}
		

	}

	cs->mots[cs->taille] = strdup(mot);

	cs->taille += 1;
}

char** get_NULL_terminated_form(commande_simple* cs) {
	
	char** res;
	char** tmp = (char**) malloc(sizeof(char*)*(cs->taille+1));
	if (tmp != NULL) {
		res = tmp;
	} else  {
		printf("Erreur lors de l'allocation du char** dans get_NULL_terminated_form()");
		free(tmp);
		exit(-1);
	}

	int i;
	for(i=0; i<cs->taille; i++) {
		res[i] = strdup(cs->mots[i]);
	}
	res[cs->taille] = NULL;
	return res;
}

commande_simple* new_commande_simple() {

	commande_simple* cs;
	commande_simple* tmp = (commande_simple*) calloc(1,sizeof(commande_simple));
	if (tmp == NULL) {
		printf("Erreur lors de l'allocation d'une commande simple");
		free(tmp);
		exit(-1);
	} else {
		cs = tmp;
	}

	char** chaine_depart;
	char** tmp_ = (char**) calloc(TAILLE_DEPART,sizeof(char*));
	if (tmp_ == NULL) {
		printf("Erreur lors de l'allocation du char** d'une commande simple\n");
		free(tmp_);
		exit(-1);
	} else {
		chaine_depart = tmp_;
	}


	cs->mots = chaine_depart;

	cs->taille = 0;
	cs->taille_reelle = TAILLE_DEPART;
	cs->taille_realloc = 4;

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