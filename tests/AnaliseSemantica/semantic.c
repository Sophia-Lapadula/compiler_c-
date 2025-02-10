#include "semantic.h"
#include "symtab.h"
#include "symtab.c"

// Controle do escopo atual
typedef struct ScopeRec {
    char *name;           // Nome do escopo (ex.: função)
    struct ScopeRec *parent; // Escopo pai
} *Scope;

Scope currentScope = NULL;  // Escopo global
ExpType currentFunctionType; // Tipo da função atual
char *currentFunctionName;   // Nome da função atual

// ========================= CONTROLE DE ESCOPO =========================
// Entra em um novo escopo (ex.: início de uma função ou bloco {})
void enterScope(char *name) {
    Scope newScope = (Scope) malloc(sizeof(struct ScopeRec));
    newScope->name = name;
    newScope->parent = currentScope;
    currentScope = newScope;
}

// Sai do escopo atual
void leaveScope() {
    if (currentScope != NULL) {
        currentScope = currentScope->parent;
    }
}

// ========================= PERCORRER A ÁRVORE =========================
// Função recursiva para percorrer a árvore sintática e aplicar funções
static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t); // Processa antes dos filhos (ex.: inserir na tabela)
        for (int i = 0; i < MAXCHILDREN; i++) 
            traverse(t->child[i], preProc, postProc);
        postProc(t); // Processa após os filhos (ex.: verificar identificadores)
        traverse(t->sibling, preProc, postProc);
    }
}

// ========================= INSERÇÃO NA TABELA DE SÍMBOLOS =========================
// Registra variáveis e funções na tabela de símbolos
static void insertNode(TreeNode *t) {
    if (t->nodeKind == DeclK) {
        insertSymbol(t->attr_name, t->lineno, t->nodeKind, t->type);

        if (t->kind.decl == FunK) {
            enterScope(t->attr_name); // Entramos no escopo da função
            currentFunctionType = t->type;
            currentFunctionName = t->attr_name;
        }
    } else if (t->nodeKind == StmtK && t->kind.stmt == CompK) {
        enterScope("Bloco"); // Blocos compostos ({ }) criam novo escopo
    }
}

// ========================= VERIFICAÇÕES SEMÂNTICAS =========================
// Verifica se identificadores usados estão declarados
static void checkNode(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == IdK) {
        if (lookupSymbol(t->attr_name) == NULL) {
            fprintf(stderr, "Erro: Identificador não declarado '%s' na linha %d\n", t->attr_name, t->lineno);
        }
    }
}

// Verifica se os tipos estão corretos em operações matemáticas
static void checkType(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == OpK) {
        TreeNode *left = t->child[0];
        TreeNode *right = t->child[1];

        if (left->type != Integer || right->type != Integer) {
            fprintf(stderr, "Erro de tipo na linha %d: operação requer inteiros.\n", t->lineno);
        }
    }
}

// Verifica chamadas de função (argumentos corretos)
static void checkFunctionCall(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == IdK) {
        BucketList func = lookupSymbol(t->attr_name);
        if (func == NULL || func->kind != FunK) {
            fprintf(stderr, "Erro: função '%s' não declarada na linha %d.\n", t->attr_name, t->lineno);
        } else {
            // (Futuro) Verificar número e tipos de argumentos
        }
    }
}

// Verifica retornos de função
static void checkReturn(TreeNode *t) {
    if (t->nodeKind == StmtK && t->kind.stmt == ReturnK) {
        if (currentFunctionType == Integer && t->child[0] == NULL) {
            fprintf(stderr, "Erro na linha %d: função '%s' deve retornar um inteiro.\n", t->lineno, currentFunctionName);
        }
    }
}

// ========================= EXECUTAR ANÁLISE SEMÂNTICA =========================
// Percorre a árvore e aplica verificações semânticas
void analyze(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, checkNode);
    traverse(syntaxTree, checkType, NULL);
    traverse(syntaxTree, checkFunctionCall, NULL);
    traverse(syntaxTree, checkReturn, NULL);
    leaveScope(); // Sai do escopo global ao final
}
