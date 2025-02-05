#include "semantic.h"
#include "symtab.h"

static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t);
        for (int i = 0; i < MAXCHILDREN; i++)
            traverse(t->child[i], preProc, postProc);
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void insertNode(TreeNode *t) {
    if (t->nodeKind == DeclK) {
        insertSymbol(t->attr_name, t->lineno, t->nodeKind, t->type);
    }
}

static void checkNode(TreeNode *t) {
    if (t->nodeKind == ExpK && t->kind.exp == IdK) {
        if (lookupSymbol(t->attr_name) == NULL) {
            fprintf(stderr, "Erro: Identificador não declarado '%s' na linha %d\n", t->attr_name, t->lineno);
        }
    }
}

void analyze(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, checkNode);
}
