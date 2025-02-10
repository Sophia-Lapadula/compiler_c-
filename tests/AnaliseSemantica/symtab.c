#include "symtab.h"

#define SIZE 211  // Tamanho da tabela hash 
#define SHIFT 4   // Valor de shift para a função hash - distribuição das chaves

// ========================== DEFINIÇÃO DE ESTRUTURAS ==========================

// Definição da estrutura dos buckets (entrada da tabela de símbolos)
typedef struct BucketListRec {
    char *name;    // Nome do identificador (string)
    int lineno;    // Número da linha onde o identificador foi declarado no código-fonte
    NodeKind kind; // Tipo do nó AST associado (ex: variável, função) - definido em symtab.h
    ExpType type;  // Tipo de dado do identificador (ex: int, float, etc.) - definido em symtab.h
    struct BucketListRec *next; // Ponteiro para o próximo nó na lista encadeada (para tratamento de colisões)
} *BucketList; // BucketList é um ponteiro para struct BucketListRec (sinônimo para facilitar a leitura)

// Estrutura de escopo para suportar variáveis locais
typedef struct ScopeRec {
    char *name;                 // Nome do escopo (ex.: nome da função)
    struct ScopeRec *parent;    // Ponteiro para o escopo pai (escopo superior)
    BucketList hashTable[SIZE]; // Tabela hash local para armazenar símbolos deste escopo
} *Scope; // Scope é um ponteiro para struct ScopeRec

// ========================== VARIÁVEIS GLOBAIS ==========================

// Escopo atual da análise semântica (usado para manipular blocos aninhados e funções)
Scope currentScope = NULL;  

// ========================== FUNÇÃO HASH ==========================
// Função hash: converte um nome de identificador (string) em um índice na tabela hash.
static int hash(char *key) {
    int temp = 0;  // Valor hash temporário
    int i = 0;     // Índice para iterar sobre a string key
    while (key[i] != '\0') { // Loop através de cada caractere da string
        // Calcula o valor hash usando um esquema de deslocamento e adição
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i; // Avança para o próximo caractere
    }
    return temp; // Retorna o valor hash calculado (um índice na tabela hash)
}

// ========================== CONTROLE DE ESCOPO ==========================
// Cria um novo escopo e o define como o escopo atual
void enterScope(char *name) {
    Scope newScope = (Scope) malloc(sizeof(struct ScopeRec));
    newScope->name = name;
    newScope->parent = currentScope; // Aponta para o escopo anterior
    currentScope = newScope; // Atualiza o escopo atual
}

// Sai do escopo atual e retorna para o escopo superior
void leaveScope() {
    if (currentScope != NULL) {
        currentScope = currentScope->parent; // Retorna ao escopo anterior
    }
}

// ========================== INSERÇÃO NA TABELA DE SÍMBOLOS ==========================
// Insere um novo identificador no escopo atual
void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type) {
    if (currentScope == NULL) {
        fprintf(stderr, "Erro: Nenhum escopo ativo para inserir '%s'.\n", name);
        return;
    }

    int h = hash(name); // Calcula o valor hash do nome do identificador
    BucketList l = currentScope->hashTable[h]; // Obtém a lista encadeada correspondente ao hash

    // Procura na lista encadeada do escopo atual para evitar duplicação
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next; // Avança para o próximo nó na lista encadeada

    if (l == NULL) { // Se o símbolo não foi encontrado no escopo atual, insere
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = strdup(name); // Cria uma cópia do nome do identificador
        l->lineno = lineno; // Registra a linha da declaração
        l->kind = kind; // Define o tipo do nó AST
        l->type = type; // Define o tipo de dado
        l->next = currentScope->hashTable[h]; // Insere no início da lista encadeada
        currentScope->hashTable[h] = l; // Atualiza a tabela hash do escopo
    } else {
        // Se o símbolo já existe no mesmo escopo, exibe erro
        fprintf(stderr, "Erro: Identificador duplicado '%s' na linha %d.\n", name, lineno);
    }
}

// ========================== BUSCA NA TABELA DE SÍMBOLOS ==========================
// Procura um identificador na tabela de símbolos, verificando escopos superiores
TreeNode* lookupSymbol(char *name) {
    Scope s = currentScope; // Começa a busca no escopo atual
    while (s != NULL) { // Percorre os escopos superiores até o global
        int h = hash(name);
        BucketList l = s->hashTable[h];

        // Procura na lista encadeada do escopo atual
        while ((l != NULL) && (strcmp(name, l->name) != 0))
            l = l->next;

        if (l != NULL) return (TreeNode *)l; // Se encontrado, retorna
        s = s->parent; // Passa para o escopo superior
    }
    return NULL; // Retorna NULL se o símbolo não for encontrado
}

// ========================== IMPRESSÃO DA TABELA DE SÍMBOLOS ==========================
// Imprime a tabela de símbolos, incluindo escopos locais e globais
void printSymbolTable() {
    Scope s = currentScope; // Começa no escopo atual
    while (s != NULL) { // Percorre todos os escopos existentes
        fprintf(listing, "\nEscopo: %s\n", s->name);
        fprintf(listing, "Nome\tLinha\tTipo\n");
        fprintf(listing, "--------------------------\n");

        for (int i = 0; i < SIZE; i++) { // Percorre os buckets da tabela hash
            if (s->hashTable[i] != NULL) { // Se houver símbolos no bucket
                BucketList l = s->hashTable[i];
                while (l != NULL) { // Percorre a lista encadeada
                    fprintf(listing, "%s\t%d\t%d\n", l->name, l->lineno, l->type);
                    l = l->next;
                }
            }
        }
        s = s->parent; // Move para o escopo superior
    }
}
