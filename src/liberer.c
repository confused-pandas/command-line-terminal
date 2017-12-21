#include <stdlib.h>

#include "liberer.h"

void liberer(commande* c) {
	if (c != NULL) {
		liberer_ao(c->liste);
		if (c->suivante != NULL) {
			liberer(c->suivante);
		}
		free(c);
	}
}

void liberer_ao(liste_and_or* lao) {
	if (lao->precedente != NULL) {
		liberer_ao(lao->precedente);
	}
	liberer_p(lao->liste);
	free(lao);
}

void liberer_p(liste_pipe* lp) {
	liberer_cr(lp->commande);
	if (lp->suivante != NULL) {
		liberer_p(lp->suivante);
	}
	free(lp);
}

void liberer_cr(commande_redirigee* cr) {
	liberer_cs(cr->commande);
	free(cr->fichier);
	free(cr);
}

void liberer_cs(commande_simple* cs) {
	
	int i;
	for(i=0; cs->mots[i]!=NULL; i++) {
		free(cs->mots[i]);
	}
	free(cs->mots);
	free(cs);

}
