#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"
#include "symtab.h" // Inclui symtab.h para acesso a currentScope

// Protótipos das funções para criar nós da árvore sintática
TreeNode *newStmtNode(StatementKind kind);
TreeNode *newExpNode(ExpressionIdentifier kind);

#endif