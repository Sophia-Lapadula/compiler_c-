
#include "util.h"

Scope currentScope = NULL;
/* Cria um novo nó de declaração */
TreeNode *newStmtNode(StatementKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = statementK;
        t->kind.stmt = kind;
        t->line_number = line_number;
        t->attr.scope = "global";
    }
    return t;
}

/* Cria um novo nó de expressão */
TreeNode *newExpNode(ExpressionIdentifier kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = expressionK;
        t->kind.exp = kind;
        t->line_number = line_number;
        t->type = voidK;
        t->attr.scope = "global";
    }
    return t;
}