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

//Ponteiros dos arquivos (NÃO MEXER O LUGAR!!!)
extern FILE *source;
extern FILE *listing;


//Variável que armazena o número da linha 
extern int line_number;


//Definições para o analisador 

typedef int TokenType;//'TokenType' é um inteiro
//Função que retorna o token
void printToken(TokenType token, const char *tokenString);


//----------------------------- Definindo os tipos de nós da árvore sintática -----------------------------
typedef enum {
    statementK, expressionK 
} NodeKind;//Tipos de nós

typedef enum {
    ifK, whileK, assignK, variableK, arrayK, paramK, functionK, callK, returnK, numberK
} StatementKind;//Tipos de declarações

typedef enum {
    operationK, constantK, idK, vectorK, vectorIdK, typeK
} ExpressionIdentifier;//Tipos de expressões

typedef enum {
    voidK, integerK, booleanK
} ExpressionType;//Tipos de expressões

#define MAXCHILDREN 3//Número máximo de filhos

//Estrutura da árvore sintática
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

extern int Error;//Variável que indica se já ocorreu um erro
/* Protótipo da função parse */
TreeNode * parse(void);//Função que retorna a árvore sintática (Construida em parser.y)


#endif /* GLOBALS_H */