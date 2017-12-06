	/* PARTIE DEFINITIONS */
%{
	/* Le code C ici présent se copié tel quel */
	#include <stdio.h>
	#include <string.h>

	/* ATTENTION A BIEN IMPORTER LES STRUCTURES AVANT LE PARSER */
	#include "commande.h"
	#include "parser.h"
	#define SAVE_TEXT yylval.string = strdup(yytext)

%}
	//Je fais de la magie noire je comprends rien laissez moi tranquille
%option noyywrap

%%
	/* PARITE REGLES */

[ \t\n]				;
[a-zA-Z0-9\_\-\.]+	{ SAVE_TEXT; return T_MOT; }
">"					{ return T_REDIR_INPUT; }
"<"					{ return T_REDIR_OUTPUT; }
">>"				{ return T_APPEND; }
"|"					{ return T_PIPE; }
"&&"				{ return T_AND; }
"||"				{ return T_OR; }
";"					{ return T_SEMICOLON; }
"&"					{ return T_AMPERSAND; }
.					{ printf("!! Symbole inconnu : %s\n",yytext); }

%%
	/* PARTIE ? */
/*
int main(int argc, char** argv) {

	// On lit l'entrée
	yylex();

}
*/