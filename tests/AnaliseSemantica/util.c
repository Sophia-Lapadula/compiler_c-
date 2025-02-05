#include "globals.h"

TreeNode *newDeclNode(DeclKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    t->nodeKind = DeclK;
    t->kind.decl = kind;
    return t;
}

TreeNode *newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    t->nodeKind = ExpK;
    t->kind.exp = kind;
    return t;
}
