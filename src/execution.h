#ifndef EXECUTION_H
#define EXECUTION_H

typedef enum redirecteur {
	REDIR_INPUT, 	/* < */
	HEREDOC, 		/* << */
	REDIR_OUTPUT, 	/* > */
	APPEND, 		/* >> */
	RIEN, 			/* si pas de redirection */
} redirecteur;

typedef enum operateur {
	OR, /* || */
	AND, /* && */
	RIEN, /* si c'est fini */
} operateur;

typedef enum separateur {
	SEMICOLUMN, /* ; */
	AMPERSAND,  /* & */
	RIEN,		/* si c'est la fin */
} separateur;

delimiteur read_token(char* token);


// Sert à contenir une commande simple
// tout en bas de la hiérarchie, directement executable par exec()
typedef struct commande_simple {
	char** c // contient par exemple ["ls",'-lR']
} commande_simple;

// Contient une commande redirigée
typedef struct commande_redirigee {
	commande_simple c;
	redirecteur r;
	char* fichier;
} commande_redirigee;


// Sert à former des listes de commandes pipés
typedef struct liste_pipe {
	commande_redirigee c;
	commande_redirigee* suivante;
} liste_pipe;

// Sert à former des listes de listes pipés enchainées avec des && et des ||
typedef struct liste_and_or {
	liste_pipe liste;
	operateur op;
	liste_pipe* suivante;
} liste_and_or;

typedef struct commande {
	liste_and_or l;
	separateur sep;
	liste_and_or* suivante;
} commande;



#endif 