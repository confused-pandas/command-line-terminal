#ifndef EXECUTION_H
#define EXECUTION_H


typedef enum delimiteur {
	SEMICOLUMN, /* ; */
	OR, /* || */
	AND, /* && */
	PIPE, /* | */
	REDIR_INPUT, /* < */
	HEREDOC, /* << */
	REDIR_OUTPUT, /* > */
	APPEND, /* >> */
	END, /* Fin de la commande*/
	DEFAULT, /* Si ce n'est pas un délimiteur */
} delimiteur;

delimiteur read_token(char* token);

int execution(char** parsed);
int execution_simple(char** parsed);

#endif 