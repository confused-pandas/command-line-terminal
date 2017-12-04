#ifndef EXECUTION_H
#define EXECUTION_H

int execution(commande* c);
int execution_and_or(liste_and_or* l);
int execution_pipe(liste_pipe* l, int niveau);
int execution_redirigee(commande_redirigee* c);
int execution_simple(commande_simple* c);

#endif 