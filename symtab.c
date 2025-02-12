#include "symtab.h"
#include "globals.h"

Scope currentScope = NULL; //

static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << 1) + key[i]) % 211;
        ++i;
    }
    return temp;
}

void enterScope(char *name) {
    Scope newScope = (Scope)malloc(sizeof(struct ScopeRec));
    newScope->name = name;
    newScope->parent = currentScope; // Aponta para o escopo anterior
    for (int i = 0; i < SIZE; ++i) {
        newScope->hashTable[i] = NULL;
    }
    currentScope = newScope; // Atualiza o escopo atual
}

void leaveScope() {
    if (currentScope != NULL) {
        currentScope = currentScope->parent; // Volta ao escopo anterior
    }
}

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

void printSymbolTable() {
    Scope sc = currentScope;
    while (sc != NULL) {
        fprintf(listing, "Scope: %s\n", sc->name);
        for (int i = 0; i < SIZE; ++i) {
            BucketList l = sc->hashTable[i];
            while (l != NULL) {
                LineList lines = l->lines;
                while (lines != NULL) {
                    fprintf(listing, "\t%s\t%d\t%d\n", l->name, lines->line_number, l->kind);
                    lines = lines->next;
                }
                l = l->next;
            }
        }
        sc = sc->parent;
    }
}