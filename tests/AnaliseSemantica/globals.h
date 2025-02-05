#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { StmtK, ExpK, DeclK } NodeKind;
typedef enum { IfK, WhileK, ReturnK, CompK } StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;
typedef enum { VarK, FunK, ParamK } DeclKind;
typedef enum { Integer, Void } ExpType;

#define MAXCHILDREN 3

// Estrutura da árvore sintática
typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodeKind;
    union {
        StmtKind stmt;
        ExpKind exp;
        DeclKind decl;
    } kind;
    char *attr_name;
    int attr_val;
    ExpType type;
} TreeNode;

extern FILE *source;
extern FILE *listing;
extern int line_number;

#endif
