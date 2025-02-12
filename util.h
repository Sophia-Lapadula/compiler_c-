#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"
#include "symtab.h" // Include symtab.h to define Scope

extern Scope currentScope;

TreeNode *newStmtNode(StatementKind kind);
TreeNode *newExpNode(ExpressionIdentifier kind);
#endif