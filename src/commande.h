#ifndef COMMANDE_H
#define COMMANDE_H

typedef enum {
	REDIR_INPUT, 	/* < */
	REDIR_OUTPUT, 	/* > */
	APPEND, 		/* >> */
	RED_RIEN, 		/* si pas de redirection */
} redirecteur;

typedef enum {
	OR, /* || */
	AND, /* && */
	OP_RIEN, /* si c'est fini */
} operateur;

typedef enum {
	SEMICOLUMN, /* ; */
	AMPERSAND,  /* & */
	SEP_RIEN,		/* si c'est la fin */
} separateur;

// Sert à contenir une commande simple
// tout en bas de la hiérarchie, directement executable par exec()
typedef struct {
	char** commande; // contient par exemple ["ls",'-lR']
} commande_simple;

// Contient une commande redirigée
typedef struct {
	commande_simple* commande;
	redirecteur red;
	char* fichier;
} commande_redirigee;


// Sert à former des listes de commandes pipés
typedef struct liste_pipe {
	commande_redirigee* commande;
	struct liste_pipe* suivante;
} liste_pipe;

// Sert à former des listes de listes pipés enchainées avec des && et des ||
typedef struct liste_and_or{
	liste_pipe* liste;
	operateur op;
	struct liste_and_or* suivante;
} liste_and_or;

typedef struct commande{
	liste_and_or* l;
	separateur sep;
	struct commande* suivante;
} commande;

void cs_append(commande_simple* commande_simple, char* mot);
void cs_fusion(commande_simple* debut, commande_simple* suite);
commande_simple* new_commande_simple();
commande_redirigee* new_commande_redir();
liste_pipe* new_pipe();
liste_and_or* new_and_or();
commande* new_commande();

#endif