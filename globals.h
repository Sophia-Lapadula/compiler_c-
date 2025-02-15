#ifndef GLOBALS_H
#define GLOBALS_H


/* Bibliotecas necessárias */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


# ifndef YYPARSER

# include "parser.tab.h"

# define ENDFILE 0

# endif

# ifndef FALSE
# define FALSE 0
# endif

# ifndef TRUE
# define TRUE 1
# endif


extern FILE *source;
extern FILE *listing;


/* Variável que armazena o número da linha */
extern int line_number;


/* Definições para o analisador */

typedef int TokenType;
void printToken(TokenType token, const char *tokenString);
/* Tipos para o parser */
typedef enum {
    statementK, expressionK 
} NodeKind;

typedef enum {
    ifK, whileK, assignK, variableK, arrayK, paramK, functionK, callK, returnK, numberK
} StatementKind;

typedef enum {
    operationK, constantK, idK, vectorK, vectorIdK, typeK
} ExpressionIdentifier;

typedef enum {
    voidK, integerK, booleanK
} ExpressionType;

#define MAXCHILDREN 3

typedef struct treeNode {
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int line_number;
    NodeKind nodekind;
    union { 
        StatementKind stmt; 
        ExpressionIdentifier exp;
    } kind;
    struct { 
        TokenType op;
        int val;
        int len;
        char* name; 
        char* scope;	
    } attr;
    ExpressionType type;
} TreeNode;

extern int Error;
/* Protótipo da função parse */
TreeNode * parse(void);


#endif /* GLOBALS_H */