#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type);
TreeNode* lookupSymbol(char *name);
void printSymbolTable();

#endif
