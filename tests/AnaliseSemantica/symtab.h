#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

// ========================== DEFINIÇÃO DAS ESTRUTURAS ==========================

/**
 * Estrutura para armazenar todas as ocorrências de um identificador no código.
 * Cada nó da lista contém um número de linha onde o identificador foi usado.
 */
typedef struct LineListRec {
    int lineno;              // Número da linha onde o identificador apareceu
    struct LineListRec *next; // Próxima ocorrência na lista
} *LineList;


 // Estrutura da tabela de símbolos 
typedef struct BucketListRec {
    char *name;         // Nome do identificador 
    NodeKind kind;      // Tipo do nó AST (ex.: DeclK para variáveis/funções)
    ExpType type;       // Tipo do identificador (ex.: Integer, Void)
    LineList lines;     // Lista encadeada de todas as linhas onde o identificador aparece
    struct BucketListRec *next; // Próximo nó na lista encadeada (para resolver colisões)
} *BucketList;

/**
 * Estrutura de escopo para gerenciar variáveis locais e globais.
 * - Cada escopo tem um nome (`name`) e um ponteiro para o escopo pai (`parent`).
 * - Contém uma tabela hash para armazenar símbolos dentro do escopo.
 */
typedef struct ScopeRec {
    char *name;                 // Nome do escopo (ex.: nome da função)
    struct ScopeRec *parent;    // Ponteiro para o escopo pai (escopo superior)
    BucketList hashTable[211];  // Tabela hash para armazenar símbolos no escopo
} *Scope;

// ========================== FUNÇÕES PRINCIPAIS ==========================

/**
 * Insere um novo identificador na tabela de símbolos do escopo atual.
 * Se o identificador já existir no escopo, um erro será exibido.
 *
 * @param name  Nome do identificador
 * @param lineno Número da linha onde foi declarado
 * @param kind Tipo do nó AST (DeclK, StmtK, ExpK)
 * @param type Tipo do identificador (Integer, Void)
 */
void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type);

/**
 * Adiciona um número de linha a um identificador existente na tabela de símbolos.
 * Se o identificador não existir, essa função não faz nada.
 *
 * @param name Nome do identificador
 * @param lineno Número da linha onde foi usado
 */
void addLineNumber(char *name, int lineno);

/**
 * Procura um identificador na tabela de símbolos, verificando escopos superiores.
 *
 * @param name Nome do identificador a ser procurado
 * @return Ponteiro para a entrada da tabela de símbolos, ou NULL se não for encontrado
 */
TreeNode* lookupSymbol(char *name);

/**
 * Imprime a tabela de símbolos, incluindo todas as linhas onde cada identificador aparece.
 */
void printSymbolTable();

// ========================== CONTROLE DE ESCOPO ==========================

/**
 * Cria um novo escopo e define-o como o escopo atual.
 * Um novo escopo é criado, por exemplo, ao entrar em uma função ou bloco `{}`.
 *
 * @param name Nome do novo escopo
 */
void enterScope(char *name);

/**
 * Sai do escopo atual e retorna ao escopo superior.
 * Isso é chamado ao final de uma função ou bloco `{}`.
 */
void leaveScope();

#endif
