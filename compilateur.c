#include "analyseur_syntaxique.h"
#include "analyseur_lexical.h"
#include <stdlib.h>
#include <string.h>

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Usage : ./compilateur [OPTION] input.l\n");
	}

  	yyin = fopen(argv[2], "r");
  	if(yyin == NULL){
    	fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[2]);
    	exit(1);
  	}

 	if (strcmp(argv[1], "-l") == 0) {
 		test_yylex_internal(yyin);
 	} else if (strcmp(argv[1], "-s") == 0) {
 		analyserSyntaxe(yyin, 1, 0, 0, 0, 0);
 	} else if (strcmp(argv[1], "-a") == 0) {
 		analyserSyntaxe(yyin, 0, 1, 0, 0, 0);
 	} else if (strcmp(argv[1], "-t") == 0) {
 		analyserSyntaxe(yyin, 0, 0, 1, 1, 0);
 	} else if (strcmp(argv[1], "-m") == 0) {
 		analyserSyntaxe(yyin, 0, 0, 1, 0, 1);
 	}

  	return 0;
}
