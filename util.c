#include "util.h"
#include "symtab.h" // Necessário para acessar currentScope

/*
 * Função para criar um novo nó na árvore sintática representando uma declaração.
 * Inicializa os campos do nó com valores padrão.
 */
TreeNode *newStmtNode(StatementKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing, "Erro: Memória insuficiente na linha %d\n", line_number);
        exit(1); // Sai do programa em caso de erro de alocação
    } else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = statementK;
        t->kind.stmt = kind;
        t->line_number = line_number;
        t->attr.scope = currentScope ? currentScope->name : "global"; // Define o escopo corretamente
        t->type = voidK; // Define o tipo padrão como void
    }
    return t;
}

/*
 * Função para criar um novo nó na árvore sintática representando uma expressão.
 * Inicializa os campos do nó com valores padrão.
 */
TreeNode *newExpNode(ExpressionIdentifier kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        fprintf(listing, "Erro: Memória insuficiente na linha %d\n", line_number);
        exit(1); // Sai do programa em caso de erro de alocação
    } else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = expressionK;
        t->kind.exp = kind;
        t->line_number = line_number;
        t->attr.scope = currentScope ? currentScope->name : "global"; // Define o escopo corretamente
        t->type = voidK; // Define o tipo padrão como void
    }
    return t;
}