#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"  // Inclui definições globais, provavelmente NodeKind, ExpType e 'listing'

// Declarações de funções da tabela de símbolos (implementadas em symtab.c)

// Insere um novo símbolo na tabela de símbolos.
// name: Nome do identificador (variável, função, etc.).
// lineno: Número da linha onde o identificador foi declarado.
// kind: Tipo do nó AST (ex: variableK, functionK, parameterK).
// type: Tipo de dado do identificador (ex: integerT, realT, voidT).
void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type);

// Procura um símbolo na tabela de símbolos.
// name: Nome do identificador a ser procurado.
// Retorna: Um ponteiro para o nó da árvore sintática (TreeNode) associado ao símbolo,
//          ou NULL se o símbolo não for encontrado.
// Nota:  A função `lookupSymbol` no arquivo `symtab.c` retorna um `BucketList`, não um `TreeNode*`.
//        Isso pode indicar um problema de design ou uma dependência implícita entre as duas estruturas.
TreeNode* lookupSymbol(char *name);

// Imprime o conteúdo da tabela de símbolos para um arquivo de listagem.
// O arquivo de listagem é presumivelmente acessível através de uma variável global
// chamada 'listing' (definida em globals.h).
void printSymbolTable();

#endif