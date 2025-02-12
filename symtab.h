#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

extern Scope currentScope;

//Funções auxiliares
TreeNode *newDeclNode(StatementKind kind);
TreeNode *newExpNode(ExpressionIdentifier kind);

// ========================== DEFINIÇÃO DAS ESTRUTURAS ==========================
typedef struct LineListRec {
    int line_number;
    struct LineListRec *next;
} *LineList;

typedef struct BucketListRec {
    char *name;
    NodeKind kind;
    ExpressionIdentifier type;
    LineList lines;
    struct BucketListRec *next;
} *BucketList;

typedef struct ScopeRec {
    char *name;
    struct ScopeRec *parent;
    BucketList hashTable[SIZE];
} *Scope;

// ========================== FUNÇÕES PRINCIPAIS ==========================
void insertSymbol(char *name, int line_number, NodeKind kind, ExpressionIdentifier type);
void addLineNumber(char *name, int line_number);
BucketList lookupSymbol(char *name);
void printSymbolTable();

// ========================== CONTROLE DE ESCOPO ==========================
void enterScope(char *name);
void leaveScope();

#endif