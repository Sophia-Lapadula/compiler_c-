#include "semantic.h"
#include "symtab.h"

// ========================== CONTROLE DE ESCOPO ==========================
// Estrutura que mantém o escopo atual
Scope currentScope = NULL;

// Variáveis globais para rastrear a função atual
ExpType currentFunctionType;  // Tipo da função atual (Integer, Void)
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
    if (t->nodeKind == DeclK) {
        insertSymbol(t->attr_name, t->lineno, t->nodeKind, t->type);

        // Se for uma função, atualiza o escopo
        if (t->kind.decl == FunK) {
            enterScope(t->attr_name); // Entramos no escopo da função
            currentFunctionType = t->type;
            currentFunctionName = t->attr_name;
        }
    } 
    else if (t->nodeKind == StmtK && t->kind.stmt == CompK) {
        enterScope("Bloco"); // Blocos `{}` criam novo escopo
    }
}

// ========================== VERIFICAÇÕES SEMÂNTICAS ==========================

/**
 * Verifica se identificadores usados estão declarados e adiciona o número da linha.
 */
static void checkNode(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == IdK) {
        if (lookupSymbol(t->attr_name) == NULL) {
            fprintf(stderr, "Erro: Identificador não declarado '%s' na linha %d\n", t->attr_name, t->lineno);
        } else {
            addLineNumber(t->attr_name, t->lineno); // Registra o uso do identificador
        }
    }
}

/**
 * Verifica se os tipos estão corretos em expressões matemáticas e atribuições.
 */
static void checkType(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == OpK) {
        TreeNode *left = t->child[0];
        TreeNode *right = t->child[1];

        if (left->type != Integer || right->type != Integer) {
            fprintf(stderr, "Erro de tipo na linha %d: operação requer inteiros.\n", t->lineno);
        }
    }
}

/**
 * Verifica chamadas de função (número e tipo de argumentos).
 */
static void checkFunctionCall(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == IdK) {
        BucketList func = lookupSymbol(t->attr_name);
        if (func == NULL || func->kind != FunK) {
            fprintf(stderr, "Erro: Função '%s' não declarada na linha %d.\n", t->attr_name, t->lineno);
        } else {
            // (Futuro) Verificar número e tipos de argumentos
        }
    }
}

/**
 * Verifica se funções retornam um valor adequado ao seu tipo declarado.
 */
static void checkReturn(TreeNode *t) {
    if (t->nodeKind == StmtK && t->kind.stmt == ReturnK) {
        if (currentFunctionType == Integer && t->child[0] == NULL) {
            fprintf(stderr, "Erro na linha %d: Função '%s' deve retornar um inteiro.\n", t->lineno, currentFunctionName);
        }
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
    traverse(syntaxTree, insertNode, checkNode);
    traverse(syntaxTree, checkType, NULL);
    traverse(syntaxTree, checkFunctionCall, NULL);
    traverse(syntaxTree, checkReturn, NULL);
    leaveScope(); // Sai do escopo global ao final
}
