#include "globals.h"
#include "symtab.h"


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

// Cria um novo escopo e define-o como o escopo atual
void enterScope(char *name) {
    Scope newScope = (Scope)malloc(sizeof(struct Scope));
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