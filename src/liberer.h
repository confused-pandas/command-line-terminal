#ifndef LIBERER_H
#define LIBERER_H

#include "commande.h"

void liberer(commande* c);
void liberer_ao(liste_and_or* lao);
void liberer_p(liste_pipe* lp);
void liberer_cr(commande_redirigee* cr);
void liberer_cs(commande_simple* cs);

#endif