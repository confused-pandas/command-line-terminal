#ifndef EXECUTION_H
#define EXECUTION_H

#include "commande.h"

int execution(commande* c, int niveau);
int execution_and_or(liste_and_or* l);
int execution_pipe(liste_pipe* l);
int execution_redirigee(commande_redirigee* c);
int execution_simple(commande_simple* c);
void tout_fermer();

#endif 
