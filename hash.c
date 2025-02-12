#include "globals.h"
#include "symtab.h"
#include "util.h"
#include "semantic.h"


// Variável global para o escopo atual
//extern Scope currentScope;
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
static void insertNode(TreeNode *t) {
    if (t->nodekind == statementK && t->kind.stmt == functionK) {
        // Insere a função no escopo atual e entra no novo escopo da função
        insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
        enterScope(t->attr.name); // Agora o escopo corrente é o da função
        currentFunctionType = t->type;
        currentFunctionName = t->attr.name;
    }
    
    if (t->nodekind == statementK && t->kind.stmt == variableK) {
        // Insere declaração de variável
        insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
    }
    
    if (t->nodekind == statementK && t->kind.stmt == arrayK) {
        // Insere declaração de vetor
        insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
    }
    
    if (t->nodekind == statementK && t->kind.stmt == paramK) {
        // Insere parâmetro
        insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
    }
    
    if (t->nodekind == statementK && t->kind.stmt == callK) {
        // Insere chamada (se necessário – geralmente, chamadas não são inseridas, mas depende da implementação)
        insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
    }
    
    if (t->nodekind == statementK && t->kind.stmt == ifK) {
        // Para blocos de if, cria um novo escopo
        enterScope("Bloco");
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