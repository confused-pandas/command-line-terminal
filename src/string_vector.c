#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_vector.h"

#define TAILLE_DEPART 4

string_vector* new_string_vector() {

	string_vector* sv = (string_vector*) malloc(sizeof(string_vector));
	if (sv == NULL) {
		printf("Erreur lors de l'allocation d'un string_vector_n");
		exit(105); /* No buffer space available */
	}

	char** chaine_depart = (char**) malloc(sizeof(char*)*TAILLE_DEPART);
	if (chaine_depart == NULL) {
		printf("Erreur lors de l'allocation du char** d'un string_vector\n");
		exit(105);
	}

	sv->mots = chaine_depart;

	sv->taille = 0;
	sv->taille_reelle = TAILLE_DEPART;
	sv->taille_realloc = 4;

	return sv;

}

void append(string_vector* sv, char* mot) {

	// Si on doit réallouer
	if (sv->taille == sv->taille_reelle) {

		sv->taille_reelle += sv->taille_realloc;
		sv->mots = realloc(sv->mots,sv->taille_reelle);

		if (sv->mots == NULL) {
			printf("Erreur lors de la réallocation d'un string_vector\n");
			exit(105); /* No buffer space available */
		}
		

	}

	sv->mots[sv->taille] = strdup(mot);

	sv->taille += 1;
}

char** get_NULL_terminated_form(string_vector* sv) {
	char** res = (char**) malloc(sizeof(char*)*(sv->taille+1));
	int i;
	for(i=0; i<sv->taille; i++) {
		res[i] = strdup(sv->mots[i]);
	}
	res[sv->taille] = NULL;
	return res;
}