#include <stdio.h>

extern int option_verbose;
extern char* prefixe_verbose;
extern int option_debug;
extern char* prefixe_debug;
extern int option_ultra;
extern char* prefixe_ultra;

void verbose(const char* message) {
	if (option_verbose || option_debug) {
		printf("%s%s\n",prefixe_verbose,message);	
	}
}
void debug(const char* message) {
	if (option_debug) {
		printf("%s%s\n",prefixe_debug,message);
	}
}
void ultra(const char* message) {
	if (option_ultra) {
		printf("%s%s\n",prefixe_ultra,message);
	}
}
