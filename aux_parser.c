#include "globals.h"
#include "aux_parser.h"
#include "util.h"

/* Função que cria uma cópia de uma string */
char *copyString(char *s) {
    int n;
    char *t;
    if (s == NULL)
        return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", line_number);
    else
        strcpy(t, s);
    return t;
}

static int indentno = 0;

#define INDENT indentno += 4
#define UNINDENT indentno -= 4

static void printSpaces(void) {
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(listing, " ");
}

/* Função que imprime recursivamente a árvore sintática */
void printTree(TreeNode *tree) {
    int i;
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind == statementK) {
            switch (tree->kind.stmt) {
                case ifK:
                    fprintf(listing, "If\n");
                    break;
                case assignK:
                    fprintf(listing, "Assign\n");
                    break;
                case whileK:
                    fprintf(listing, "While\n");
                    break;
                case variableK:
                    fprintf(listing, "Variable %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case arrayK:
                    fprintf(listing, "Array %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case paramK:
                    fprintf(listing, "Param %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case functionK:
                    fprintf(listing, "Function %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case callK:
                    fprintf(listing, "Call to Function %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case returnK:
                    fprintf(listing, "Return\n");
                    break;
                default:
                    fprintf(listing, "Unknown statement kind\n");
                    break;
            }
        } else if (tree->nodekind == expressionK) {
            switch (tree->kind.exp) {
                case operationK:
                    fprintf(listing, "Operation: ");
                    printToken(tree->attr.op, "\0");
                    fprintf(listing, "[%s]\n", tree->attr.scope);
                    break;
                case constantK:
                    fprintf(listing, "Constant: %d [%s]\n", tree->attr.val, tree->attr.scope);
                    break;
                case idK:
                    fprintf(listing, "Id: %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case vectorK:
                    fprintf(listing, "Vector: %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                case vectorIdK:
                    fprintf(listing, "Index [%d] [%s]\n", tree->attr.val, tree->attr.scope);
                    break;
                case typeK:
                    fprintf(listing, "Type %s [%s]\n", tree->attr.name, tree->attr.scope);
                    break;
                default:
                    fprintf(listing, "Unknown expression kind\n");
                    break;
            }
        } else
            fprintf(listing, "Unknown node kind\n");
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

void freeTree(TreeNode *tree) {
    int i;
    if (tree != NULL) {
        for (i = 0; i < MAXCHILDREN; i++) {
            freeTree(tree->child[i]);
        }
        freeTree(tree->sibling);
        free(tree);
    }
}