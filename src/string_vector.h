#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

typedef struct string_vector string_vector;

string_vector* new_string_vector();
void append(string_vector* sv, char* mot);
char** get_NULL_terminated_form(string_vector* sv);

struct string_vector {
	char** mots;
	int taille;
	int taille_reelle;
	int taille_realloc;
};

#endif