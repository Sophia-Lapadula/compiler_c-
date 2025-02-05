#include "symtab.h"

#define SIZE 211  // Tamanho da tabela hash 
#define SHIFT 4   // Valor de shift para a função hash - distribuição das chaves

// Definição da estrutura dos buckets
typedef struct BucketListRec {
    char *name;    // Nome do identificador (string)
    int lineno;    // Número da linha onde o identificador foi declarado no código fonte
    NodeKind kind;  // Tipo do nó AST associado (ex: variável, função) - definido em symtab.h
    ExpType type;   // Tipo de dado do identificador (ex: int, float, etc.) - definido em symtab.h
    struct BucketListRec *next; // Ponteiro para o próximo nó na lista encadeada (para tratamento de colisões)
} *BucketList; // BucketList é um ponteiro para struct BucketListRec (sinônimo para facilitar a leitura)

// A tabela hash propriamente dita.  Um array de listas encadeadas (buckets).
// Cada elemento do array aponta para o início de uma lista encadeada de símbolos que fazem hash para esse índice.
static BucketList hashTable[SIZE];

// Função hash: converte um nome de identificador (string) em um índice na tabela hash.
static int hash(char *key) {
    int temp = 0;  // Valor hash temporário
    int i = 0;    // Índice para iterar sobre a string key
    while (key[i] != '\0') { // Loop através de cada caractere da string
        // Calcula o valor hash usando um esquema de deslocamento e adição
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i; // Avança para o próximo caractere
    }
    return temp; // Retorna o valor hash calculado (um índice na tabela hash)
}

// Função para inserir um novo símbolo na tabela de símbolos.
void insertSymbol(char *name, int lineno, NodeKind kind, ExpType type) {
    int h = hash(name); // Calcula o valor hash do nome do identificador
    BucketList l = hashTable[h]; // Obtém o início da lista encadeada (bucket) correspondente ao valor hash

    // Procura na lista encadeada para ver se o símbolo já existe
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next; // Avança para o próximo nó na lista encadeada

    // Se o símbolo não foi encontrado (l == NULL), insere um novo nó
    if (l == NULL) {
        // Aloca memória para um novo nó da lista encadeada
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name; // Armazena o nome do identificador (atenção: não faz cópia! assume que 'name' persiste)
        l->lineno = lineno; // Armazena o número da linha
        l->kind = kind;   // Armazena o tipo do nó AST
        l->type = type;   // Armazena o tipo de dado
        l->next = hashTable[h]; // Insere o novo nó no início da lista encadeada
        hashTable[h] = l; // Atualiza o início da lista encadeada na tabela hash
    } else {
        // Se o símbolo já existe, atualiza o número da linha (pode ser usado em várias linhas)
        l->lineno = lineno;
        // Nota: Não atualiza o tipo ou kind, assume que o primeiro uso define essas propriedades
    }
}

// Função para procurar um símbolo na tabela de símbolos.
TreeNode* lookupSymbol(char *name) {
    int h = hash(name); // Calcula o valor hash do nome
    BucketList l = hashTable[h]; // Obtém o início da lista encadeada (bucket)

    // Procura na lista encadeada pelo símbolo
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next; // Avança para o próximo nó na lista

    return l; // Retorna um ponteiro para o nó se encontrado, NULL caso contrário
    //Importante: Retorna um ponteiro para o BucketListRec, que é do tipo BucketList. No entanto, a assinatura da função declara que retorna um TreeNode*.  Isso sugere que há uma relação entre BucketListRec e TreeNode. Talvez BucketListRec possa ser implicitamente convertido para TreeNode*. No entanto, o código não mostra essa relação diretamente e pode haver um problema de type safety aqui dependendo de como TreeNode é definido.
}

// Função para imprimir o conteúdo da tabela de símbolos (para depuração ou listagem)
void printSymbolTable() {
    fprintf(listing, "Nome\tLinha\tTipo\n"); // Imprime o cabeçalho da tabela
    fprintf(listing, "--------------------------\n");
    for (int i = 0; i < SIZE; i++) { // Itera sobre cada bucket na tabela hash
        if (hashTable[i] != NULL) { // Se o bucket não estiver vazio
            BucketList l = hashTable[i]; // Obtém o início da lista encadeada
            while (l != NULL) { // Itera sobre a lista encadeada
                fprintf(listing, "%s\t%d\t%d\n", l->name, l->lineno, l->type); // Imprime as informações do símbolo
                l = l->next; // Avança para o próximo nó na lista
            }
        }
    }
}