#include "symtab.h"

#define SIZE 211
#define SHIFT 4

typedef struct BucketListRec {
    char *name;
    int lineno;
    NodeKind kind;
    ExpType type;
    struct BucketListRec *next;
} *BucketList;

static BucketList hashTable[SIZE];

static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;

    if (l == NULL) {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lineno = lineno;
        l->kind = kind;
        l->type = type;
        l->next = hashTable[h];
        hashTable[h] = l;
    } else {
        l->lineno = lineno;
    }
}

TreeNode* lookupSymbol(char *name) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    return l; // NULL se não encontrado
}

void printSymbolTable() {
    fprintf(listing, "Nome\tLinha\tTipo\n");
    fprintf(listing, "--------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        if (hashTable[i] != NULL) {
            BucketList l = hashTable[i];
            while (l != NULL) {
                fprintf(listing, "%s\t%d\t%d\n", l->name, l->lineno, l->type);
                l = l->next;
            }
        }
    }
}
