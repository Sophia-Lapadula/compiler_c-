#include "globals.h"
#include "util.h"

// Funções auxiliares
TreeNode *newStmtNode(StatementKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Out of memory error at line %d\n", __LINE__);
    } else {
        t->nodekind = statementK; // Use the correct enumeration for declaration nodes
        t->kind.stmt = kind;
        t->attr.name = NULL;
        t->attr.scope = NULL;
        t->type = voidK; // Use the correct type enumeration
        for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
    }
    return t;
}

// Define newExpNode here
TreeNode *newExpNode(ExpressionIdentifier kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Out of memory error at line %d\n", __LINE__);
    } else {
        t->nodekind = expressionK; // Use the correct enumeration for expression nodes
        t->kind.exp = kind;
        t->attr.name = NULL;
        t->attr.scope = NULL;
        t->type = voidK; // Use the correct type enumeration
        for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
    }
    return t;
}