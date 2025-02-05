#include "globals.h"
#include "symtab.h"

#define SIZE 211  // Tamanho da tabela hash (número primo melhora a distribuição)
#define SHIFT 4   // Para a função de dispersão

/* Estrutura de um nó da tabela de símbolos */
typedef struct BucketListRec {
    char *name;                // Nome do identificador
    int lineno;                // Linha onde apareceu
    NodeKind kind;             // Tipo do nó (DeclK, StmtK, ExpK)
    struct BucketListRec *next; // Próximo nó na lista (colisões na hash table)
} *BucketList;

/* Estrutura da tabela de símbolos */
static BucketList hashTable[SIZE];

/* Função hash */
static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        i++;
    }
    return temp;
}

/* Insere um identificador na tabela de símbolos */
void insertSymbol(char *name, int lineno, NodeKind kind) {
    int h = hash(name);
    BucketList l = hashTable[h];
    
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;

    if (l == NULL) { // Novo símbolo
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lineno = lineno;
        l->kind = kind;
        l->next = hashTable[h];
        hashTable[h] = l;
    } else { // Já existe, só atualiza a linha
        l->lineno = lineno;
    }
}

/* Busca um identificador na tabela de símbolos */
BucketList lookupSymbol(char *name) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    return l; // Retorna NULL se não encontrado
}

/* Exibe a tabela de símbolos */
void printSymbolTable() {
    fprintf(listing, "Nome\tLinha\tTipo\n");
    fprintf(listing, "--------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        if (hashTable[i] != NULL) {
            BucketList l = hashTable[i];
            while (l != NULL) {
                fprintf(listing, "%s\t%d\t%d\n", l->name, l->lineno, l->kind);
                l = l->next;
            }
        }
    }
}