	/* PARTIE DEFINITIONS */
%{
	/* Le code C ici présent se copié tel quel */
	#include <stdio.h>
	#include <string.h>

	/* ATTENTION A BIEN IMPORTER LES STRUCTURES AVANT LE PARSER */
	#include "commande.h"
	#include "parser.h"
	#include "verbose.h"
	#define SAVE_TEXT yylval.string = strdup(yytext)

%}
	//Je fais de la magie noire je comprends rien laissez moi tranquille
%option nounput noinput noyywrap


%%
	/* PARTIE REGLES */

[ \t]					;
<<EOF>>					{ yyterminate(); }
[\n\0]					{ yyterminate(); }
[a-zA-Z0-9\_\-\./!]+	{ SAVE_TEXT; return T_MOT; }
">"						{ return T_REDIR_OUTPUT; }
"<"						{ return T_REDIR_INPUT; }
">>"					{ return T_APPEND; }
"|"						{ return T_PIPE; }
"&&"					{ return T_AND; }
"||"					{ return T_OR; }
";"						{ return T_SEMICOLON; }
"&"						{ return T_AMPERSAND; }
.						{
							char* chaine_retour_erreur = (char*) malloc(sizeof(char));
							sprintf(chaine_retour_erreur,"Symbole inconnu : %s = %d\n",yytext,(int) yytext[0]);
            				debug(chaine_retour_erreur);
						}

%%
	/* PARTIE ? */
/*
int main(int argc, char** argv) {

	// On lit l'entrée
	yylex();

}
*/
