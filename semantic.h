#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include "globals.h"
#include "util.h"

// Função principal da análise semântica
void analyze(TreeNode *syntaxTree);

// Controle de escopo
void enterScope(char *name);
void leaveScope();

// Verificações semânticas
void checkType(TreeNode *t);
void checkFunctionCall(TreeNode *t);
void checkReturn(TreeNode *t);

#endif