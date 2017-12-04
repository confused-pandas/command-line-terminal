	/* PARTIE DEFINITIONS */
%{
	/* Le code C ici présent se copié tel quel */
	#include <stdio.h>
%}

%%
	/* PARITE REGLES */

[ \t\n]				;
[a-zA-Z0-9\_\-\.]+	{ printf("Mot lu : %s\n",yytext); }
">"					{ printf("Redirecteur : >\n"); }
"<"					{ printf("Redirecteur : <\n"); }
">>"				{ printf("Redirecteur : >>\n"); }
"|"					{ printf("Pipe\n"); }
"&&"				{ printf("Opérateur : &&\n"); }
"||"				{ printf("Opérateur : ||\n"); }
";"					{ printf("Séparateur : ;\n"); }
"&"					{ printf("Séparateur : &\n"); }
.					{ printf("!! Token inconnu : %s\n",yytext); }

%%
	/* PARTIE ? */
int main(int argc, char** argv) {

	// On lit l'entrée
	yylex();

}