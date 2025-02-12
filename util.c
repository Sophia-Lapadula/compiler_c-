#include "globals.h"
//Funções auxiliares
TreeNode *newDeclNode(StatementKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    t->nodekind = statementK;
    t->kind.stmt = kind;
    return t;
}

TreeNode *newExpNode(ExpressionIdentifier kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    t->nodekind = expressionK;
    t->kind.exp = kind;
    return t;
}