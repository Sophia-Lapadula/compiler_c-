#include "symtab.h"
#include "globals.h"

//#define SIZE 211  // Tamanho da tabela hash 
//#define SHIFT 4   // Valor de shift para a função hash - melhora a distribuição das chaves


// Escopo atual da análise semântica (controla variáveis locais e funções)

// ========================== FUNÇÃO HASH ==========================
// Gera um índice na tabela hash a partir do nome do identificador
static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') { // Percorre cada caractere da string
        temp = ((temp << SHIFT) + key[i]) % SIZE; // Aplica deslocamento e soma
        ++i;
    }
    return temp;
}

// ========================== CONTROLE DE ESCOPO ==========================
// Entra em um novo escopo (por exemplo, ao iniciar uma função)
void enterScope(char *name) {
    Scope newScope = (Scope) malloc(sizeof(struct ScopeRec));
    newScope->name = strdup(name); // Nome do escopo
    newScope->parent = currentScope; // Aponta para o escopo anterior
    currentScope = newScope; // Atualiza o escopo atual
}

// Sai do escopo atual e retorna ao escopo superior
void leaveScope() {
    if (currentScope != NULL) {
        currentScope = currentScope->parent; // Volta ao escopo anterior
    }
}

// ========================== INSERÇÃO NA TABELA DE SÍMBOLOS ==========================
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

    if (l == NULL) { // Se o símbolo não existe, cria um novo nó
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = strdup(name); // Copia o nome do identificador
        l->kind = kind; // Define o tipo do nó AST
        l->type = type; // Define o tipo do identificador
        l->lines = (LineList) malloc(sizeof(struct LineListRec)); // Cria a primeira entrada na lista de linhas
        l->lines->line_number = line_number;
        l->lines->next = NULL;
        l->next = currentScope->hashTable[h]; // Adiciona ao início da lista encadeada
        currentScope->hashTable[h] = l;
    } else {
        // Se o símbolo já existe no mesmo escopo, exibe erro
        fprintf(stderr, "Erro: Identificador duplicado '%s' na linha %d.\n", name, line_number);
    }
}

// ========================== REGISTRO DE USO DE IDENTIFICADORES ==========================
// Adiciona um número de linha ao identificador já existente na tabela de símbolos
void addLineNumber(char *name, int line_number) {
    BucketList l = lookupSymbol(name);
    if (l != NULL) {
        LineList temp = l->lines;
        while (temp->next != NULL) // Percorre até o final da lista
            temp = temp->next;
        temp->next = (LineList) malloc(sizeof(struct LineListRec));
        temp->next->line_number = line_number;
        temp->next->next = NULL;
    }
}

// ========================== BUSCA NA TABELA DE SÍMBOLOS ==========================
// Procura um identificador na tabela de símbolos, verificando escopos superiores
BucketList lookupSymbol(char *name) {
    Scope s = currentScope; // Começa no escopo atual
    while (s != NULL) { // Percorre escopos superiores até o global
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
// Imprime a tabela de símbolos, incluindo todas as linhas onde um identificador aparece
void printSymbolTable() {
    Scope s = currentScope; // Começa no escopo atual
    while (s != NULL) { // Percorre todos os escopos existentes
        fprintf(listing, "\nEscopo: %s\n", s->name);
        fprintf(listing, "Nome\tLinhas\tTipo\n");
        fprintf(listing, "--------------------------\n");

        for (int i = 0; i < SIZE; i++) { // Percorre os buckets da tabela hash
            if (s->hashTable[i] != NULL) { // Se houver símbolos no bucket
                BucketList l = s->hashTable[i];
                while (l != NULL) { // Percorre a lista encadeada
                    fprintf(listing, "%s\t", l->name);

                    // Percorre a lista de números de linha
                    LineList temp = l->lines;
                    while (temp != NULL) {
                        fprintf(listing, "%d ", temp->line_number);
                        temp = temp->next;
                    }

                    fprintf(listing, "\t%d\n", l->type);
                    l = l->next;
                }
            }
        }
        s = s->parent; // Move para o escopo superior
    }
}
