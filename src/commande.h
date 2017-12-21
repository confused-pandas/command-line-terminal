#ifndef COMMANDE_H
#define COMMANDE_H

typedef struct string_vector commande_simple;
typedef struct commande_redirigee commande_redirigee;
typedef struct liste_pipe liste_pipe;
typedef struct liste_and_or liste_and_or;
typedef struct commande commande;

void cs_append(commande_simple* cs, char* mot);
char** get_NULL_terminated_form(commande_simple* cs);

commande_simple* new_commande_simple();
commande_redirigee* new_commande_redir();
liste_pipe* new_pipe();
liste_and_or* new_and_or();
commande* new_commande();

typedef enum {
	REDIR_INPUT, 	/* < */
	REDIR_OUTPUT, 	/* > */
	APPEND, 		/* >> */
	RED_RIEN, 		/* si pas de redirection */
} redirecteur;

typedef enum {
	OR, /* || */
	AND, /* && */
} operateur;

typedef enum {
	SEMICOLUMN, /* ; */
	AMPERSAND,  /* & */
	SEP_RIEN,		/* si c'est la fin */
} separateur;

// Sert à contenir une commande simple
// tout en bas de la hiérarchie, directement executable par exec()
struct string_vector {
	char** mots;
	int taille;
	int taille_reelle;
	int taille_realloc;
};

// Contient une commande redirigée
struct commande_redirigee {
	commande_simple* commande;
	redirecteur red;
	char* fichier;
};


// Sert à former des listes de commandes pipés
struct liste_pipe {
	commande_redirigee* commande;
	liste_pipe* suivante;
};

// Sert à former des listes de listes pipés enchainées avec des && et des ||
struct liste_and_or {
	liste_and_or* precedente;
	operateur op;
	liste_pipe* liste;
};

struct commande {
	liste_and_or* liste;
	separateur sep;
	commande* suivante;
};

#endif
