//Librarys all archieves need for the parser

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
//#include "aux_scanner.c"
# include "parser.tab.h"

#define TRUE 1
#define FALSE 0

extern FILE *yyin;
extern FILE *yyout;


FILE *source;
FILE *listing;
// ---- Needed in aux_scanner.c ----

#define MAX_TOKEN_LENGHT 50
typedef int TokenType;
extern int line_number;//Variable that stores the line number
TokenType getToken(void);
void printToken(TokenType token, const char *tokenString);
