%{
	#include "commande.h"
	#include <stdio.h>
	commande* commande_lue;

	extern int yylex();
	void yyerror(const char* s) { printf("Erreur à l'analyse : %s\n",s); }
%}

%union {
	char* string;
	commande_simple* com_simple;
	commande_redirigee* com_redir;
	liste_pipe* lst_pipe;
	liste_and_or* lst_and_or;
	commande* commande_complete;
	int vide;
};

	/* Pour des messages d'erreur plus complets */
%define parse.error verbose

%token <string> T_MOT
%token <vide> T_SEMICOLON T_AMPERSAND
%token <vide> T_AND T_OR
%token <vide> T_PIPE
%token <vide> T_REDIR_INPUT T_REDIR_OUTPUT T_APPEND
%token <vide> T_NEWLINE

%type <com_simple> CS
%type <com_redir> CR
%type <lst_pipe> P
%type <lst_and_or> AO
%type <commande_complete> C
%type <vide> CP

%start CP

%%

CP	: C			{ commande_lue = $1; }
	| %empty 	{ commande_lue = NULL; }
	;

C 	:
	  AO				{ $$ = new_commande(); $$->liste = $1; $$->sep = SEP_RIEN; $$->suivante = NULL; }
	| AO T_SEMICOLON	{ $$ = new_commande(); $$->liste = $1; $$->sep = SEMICOLUMN; $$->suivante = NULL; }
	| AO T_AMPERSAND	{ $$ = new_commande(); $$->liste = $1; $$->sep = AMPERSAND; $$->suivante = NULL; }
	| AO T_SEMICOLON C	{ $$ = new_commande(); $$->liste = $1; $$->sep = SEMICOLUMN; $$->suivante = $3; }
	| AO T_AMPERSAND C 	{ $$ = new_commande(); $$->liste = $1; $$->sep = AMPERSAND; $$->suivante = $3; }
	;

AO 	: 
	  P				{ $$ = new_and_or(); $$->liste = $1; $$->precedente = NULL; }
	| AO T_AND P	{ $$ = new_and_or(); $$->liste = $3; $$->op = AND; $$->precedente = $1; }
	| AO T_OR P 	{ $$ = new_and_or(); $$->liste = $3; $$->op = OR; $$->precedente = $1; }
	;

P 	:
	  CR			{ $$ = new_pipe(); $$->commande = $1; $$->suivante = NULL; }
	| CR T_PIPE P	{ $$ = new_pipe(); $$->commande = $1; $$->suivante = $3; }
	;

CR 	:
	  CS						{ $$ = new_commande_redir(); $$->commande = $1; $$->red = RED_RIEN; 	$$->fichier = NULL; }
	| CS T_REDIR_INPUT T_MOT	{ $$ = new_commande_redir(); $$->commande = $1; $$->red = REDIR_INPUT; 	$$->fichier = $3; }
	| CS T_REDIR_OUTPUT T_MOT	{ $$ = new_commande_redir(); $$->commande = $1; $$->red = REDIR_OUTPUT; $$->fichier = $3; }
	| CS T_APPEND T_MOT			{ $$ = new_commande_redir(); $$->commande = $1; $$->red = APPEND; 		$$->fichier = $3; }
	;

CS:
	  T_MOT		{ $$ = new_commande_simple(); cs_append($$,$1); }
	| CS T_MOT	{ cs_append($1,$2); $$ = $1; }
	;

%%
