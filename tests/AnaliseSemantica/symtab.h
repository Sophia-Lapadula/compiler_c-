#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"  // Inclui definições globais, provavelmente NodeKind, ExpType e 'listing'

// Declarações de funções da tabela de símbolos (implementadas em symtab.c)

// Insere um novo símbolo na tabela de símbolos.

void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type);

// Procura um símbolo na tabela de símbolos.
TreeNode* lookupSymbol(char *name);

// Imprime o conteúdo da tabela de símbolos para um arquivo de listagem.
void printSymbolTable();

#endif