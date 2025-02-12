#include "symtab.h"
#include "globals.h"

//#define SIZE 211  // Tamanho da tabela hash 
//#define SHIFT 4   // Valor de shift para a função hash - melhora a distribuição das chaves


// Escopo atual da análise semântica (controla variáveis locais e funções)
extern Scope currentScope; // Declare currentScope as an external variable

// Função hash para calcular o índice na tabela hash
static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << 1) + key[i]) % 211;
        ++i;
    }
    return temp;
}

// Cria um novo escopo e define-o como o escopo atual
void enterScope(char *name) {
    Scope newScope = (Scope)malloc(sizeof(struct ScopeRec));
    newScope->name = name;
    newScope->parent = currentScope; // Aponta para o escopo anterior
    for (int i = 0; i < 211; ++i) {
        newScope->hashTable[i] = NULL;
    }
    currentScope = newScope; // Atualiza o escopo atual
}

// Sai do escopo atual e retorna ao escopo superior
void leaveScope() {
    if (currentScope != NULL) {
        currentScope = currentScope->parent; // Volta ao escopo anterior
    }
}