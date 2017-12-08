#include <stdio.h>

extern int option_verbose;
extern char* prefixe_verbose;
extern int option_debug;
extern char* prefixe_debug;

void verbose(const char* message) {
	if (option_verbose) {
		printf("%s%s\n",prefixe_verbose,message);	
	}
}
void debug(const char* message) {
	if (option_debug) {
		printf("%s%s\n",prefixe_debug,message);
	}
}
