#include "globals.h"
#include "symtab.h"



// ========================== CONTROLE DE ESCOPO ==========================
// Estrutura que mantém o escopo atual
//Scope currentScope = NULL;

// Variáveis globais para rastrear a função atual
ExpressionType currentFunctionType;  // Tipo da função atual (Integer, Void)
char *currentFunctionName;    // Nome da função atual

// ========================== PERCORRER A ÁRVORE ==========================

/**
 * Percorre a árvore sintática aplicando duas funções:
 * - `preProc()`: Executada antes de processar os filhos (ex.: inserir na tabela de símbolos).
 * - `postProc()`: Executada depois dos filhos (ex.: verificar identificadores).
 */
static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t); // Executa a função de pré-processamento
        for (int i = 0; i < MAXCHILDREN; i++) 
            traverse(t->child[i], preProc, postProc); // Percorre os filhos do nó
        postProc(t); // Executa a função de pós-processamento
        traverse(t->sibling, preProc, postProc); // Percorre os nós irmãos
    }
}

// ========================== INSERÇÃO NA TABELA DE SÍMBOLOS ==========================

/**
 * Insere identificadores na tabela de símbolos durante a criação da árvore sintática.
 * - Registra variáveis e funções.
 * - Atualiza o escopo ao entrar em funções ou blocos `{}`.
 */
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
        enterScope("global");
    }
}

// ========================== VERIFICAÇÕES SEMÂNTICAS ==========================

/**
 * Verifica se identificadores usados estão declarados e adiciona o número da linha.
 */
static void checkNode(TreeNode *t) {
    if (t->nodekind == expressionK && t->kind.exp == idK) {
        if (lookupSymbol(t->attr.name) == NULL) {
            fprintf(stderr, "Erro: Identificador não declarado '%s' na linha %d\n", t->attr.name, t->line_number);
        } else {
            addLineNumber(t->attr.name, t->line_number); // Registra o uso do identificador
        }
    }
}

/**
 * Verifica se os tipos estão corretos em expressões matemáticas e atribuições.
 */
void checkType(TreeNode *t) {
    if (t->nodekind == expressionK && t->kind.exp == operationK) {
        TreeNode *left = t->child[0];
        TreeNode *right = t->child[1];

        if (left->type != integerK || right->type != integerK) {
            fprintf(stderr, "Erro de tipo na linha %d: operação requer inteiros.\n", t->line_number);
        }
    }
}

/**
 * Verifica chamadas de função (número e tipo de argumentos).
 */
void checkFunctionCall(TreeNode *t) {
    if (t->nodekind == expressionK && t->kind.exp == idK) {
        BucketList func = lookupSymbol(t->attr.name);
        if (func == NULL || func->kind != functionK) {
            fprintf(stderr, "Erro: Função '%s' não declarada na linha %d.\n", t->attr.name, t->line_number);
        } else {
            // (Futuro) Verificar número e tipos de argumentos
        }
    }
}

/**
 * Verifica se funções retornam um valor adequado ao seu tipo declarado.
 */
void checkReturn(TreeNode *t) {
    if (t->nodekind == statementK && t->kind.stmt == returnK) {
        if (currentFunctionType == integerK && t->child[0] == NULL) {
            fprintf(stderr, "Erro na linha %d: Função '%s' deve retornar um inteiro.\n", t->line_number, currentFunctionName);
        }
    }
}


static void leaveScopeNode(TreeNode *t) {
    /* Se o nó criou um novo escopo, sai dele */
    if (t->nodekind == statementK &&
        (t->kind.stmt == functionK || t->kind.stmt == ifK)) {
        leaveScope();
    }
}

// ========================== EXECUTAR ANÁLISE SEMÂNTICA ==========================

/**
 * Inicia a análise semântica, percorrendo a árvore sintática e verificando:
 * - Declaração e uso de identificadores.
 * - Tipos de variáveis e expressões.
 * - Chamadas de função.
 * - Retornos de função.
 */
 void analyze(TreeNode *syntaxTree) {
    enterScope("global");  // Cria o escopo global
    /* Primeira travessia: insere os símbolos e trata o controle de escopo */
    traverse(syntaxTree, insertNode, leaveScopeNode);
    /* Outras travessias para verificações semânticas (sem alteração de escopo) */
    traverse(syntaxTree, checkNode, NULL);
    traverse(syntaxTree, checkType, NULL);
    traverse(syntaxTree, checkFunctionCall, NULL);
    traverse(syntaxTree, checkReturn, NULL);
    leaveScope(); // Sai do escopo global ao final
}