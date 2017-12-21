#ifndef EXECUTION_H
#define EXECUTION_H

#include "commande.h"

int execution(commande* c);
int execution_and_or(liste_and_or* l);
int execution_pipe(liste_pipe* l);
int execution_pipe_(liste_pipe* l, int pipe_in[2]);
int execution_redirigee(commande_redirigee* c);
int execution_simple(commande_simple* c);

#endif 
