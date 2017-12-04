%{
#include <stdio.h>
#define YYSTYPE char*
int yyparse();
int yylex();
int yyerror(char *s);
%}

//Symboles terminaux qui seront fournis par yylex()
%token MOT
%left REDIR_INPUT
%left REDIR_OUTPUT
%left APPEND
%left PIPE
%left OR
%left AND
%left SEMICOLUMN
%left AMPERSAND
%start Commande

%%

Commande:
	  Liste_And_Or '\n'
	| Liste_And_Or SEMICOLUMN Commande
	| Liste_And_Or AMPERSAND Commande {}
	;

Liste_And_Or: 
	  Liste_Pipe
	| Liste_Pipe AND Liste_And_Or
	| Liste_Pipe OR Liste_And_Or  {}
	;

Liste_Pipe:
	  Commande_Redirigee
	| Commande_Redirigee PIPE Liste_Pipe {}
	;

Commande_Redirigee:
	  Commande_Simple
	| Commande_Simple REDIR_INPUT Fichier
	| Commande_Simple REDIR_OUTPUT Fichier
	| Commande_Simple APPEND Fichier {}
	;

Commande_Simple:
	  /* Vide */
	| MOT Commande_Simple { push($1) }
	;

%%

int yyerror(char *s) {
	printf("yyerror : %s\n",s);
	return 0;
}

int main(void) {
	yyparse();
	return 0;
}