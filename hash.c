#include "globals.h"
#include "util.h"
#include "symtab.h"

// Variável global para o escopo atual
extern Scope currentScope;
static BucketList hashTable[SIZE];
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

// Insere um novo identificador na tabela de símbolos do escopo atual
void insertSymbol(char *name, int line_number, NodeKind kind, ExpressionIdentifier type) {
    if (currentScope == NULL) {
        fprintf(stderr, "Erro: Nenhum escopo ativo para inserir '%s'.\n", name);
        return;
    }

    int h = hash(name); // Calcula o índice hash
    BucketList l = currentScope->hashTable[h]; // Acessa o bucket correspondente

    // Verifica se o identificador já existe no escopo atual
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;

    if (l == NULL) { // Novo identificador
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = strdup(name);
        l->kind = kind;
        l->type = type;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->line_number = line_number;
        l->lines->next = NULL;
        l->next = currentScope->hashTable[h];
        currentScope->hashTable[h] = l;
    } else { // Identificador já existe, adiciona nova linha
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->line_number = line_number;
        t->next->next = NULL;
    }
}

// Adiciona um número de linha a um identificador existente na tabela de símbolos
void addLineNumber(char *name, int line_number) {
    BucketList l = lookupSymbol(name);
    if (l != NULL) {
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->line_number = line_number;
        t->next->next = NULL;
    }
}

// Procura um identificador na tabela de símbolos, verificando escopos superiores
BucketList lookupSymbol(char *name) {
    Scope sc = currentScope;
    while (sc != NULL) {
        int h = hash(name);
        BucketList l = sc->hashTable[h];
        while ((l != NULL) && (strcmp(name, l->name) != 0))
            l = l->next;
        if (l != NULL) return l;
        sc = sc->parent;
    }
    return NULL;
}



/* Exibe a tabela de símbolos */
void printSymbolTable() {
    Scope sc = currentScope;
    while (sc != NULL) {
        for (int i = 0; i < 211; ++i) {
            BucketList l = sc->hashTable[i];
            while (l != NULL) {
                LineList lines = l->lines;
                while (lines != NULL) {
                    fprintf(listing, "%s\t%d\t%d\n", l->name, lines->line_number, l->kind);
                    lines = lines->next;
                }
                l = l->next;
            }
        }
        sc = sc->parent;
    }
}