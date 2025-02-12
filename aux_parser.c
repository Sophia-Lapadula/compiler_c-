#include "globals.h"
#include "aux_parser.h"


/* Função que propaga um escopo para toda a árvore */
void aggScope(TreeNode *t, char *scope) {
    int i;
    while (t != NULL) {
        for (i = 0; i < MAXCHILDREN; ++i) {
            t->attr.scope = scope;
            aggScope(t->child[i], scope);
        }
        t = t->sibling;
    }
}

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
                    fprintf(listing, "Variable %s\n", tree->attr.name);
                    break;
                case arrayK:
                    fprintf(listing, "Array %s\n", tree->attr.name);
                    break;
                case paramK:
                    fprintf(listing, "Param %s\n", tree->attr.name);
                    break;
                case functionK:
                    fprintf(listing, "Function %s\n", tree->attr.name);
                    break;
                case callK:
                    fprintf(listing, "Call to Function %s\n", tree->attr.name);
                    break;
                case returnK:
                    fprintf(listing, "Return\n");
                    break;
                default:
                    fprintf(listing, "Unknown statement kind\n");
                    break;
            }
        }
        else if (tree->nodekind == expressionK) {
            switch (tree->kind.exp) {
                case operationK:
                    fprintf(listing, "Operation: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case constantK:
                    fprintf(listing, "Constant: %d\n", tree->attr.val);
                    break;
                case idK:
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                    break;
                case vectorK:
                    fprintf(listing, "Vector: %s\n", tree->attr.name);
                    break;
                case vectorIdK:
                    fprintf(listing, "Index [%d]\n", tree->attr.val);
                    break;
                case typeK:
                    fprintf(listing, "Type %s\n", tree->attr.name);
                    break;
                default:
                    fprintf(listing, "Unknown expression kind\n");
                    break;
            }
        }
        else
            fprintf(listing, "Unknown node kind\n");
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

void printAbstractTree(TreeNode *tree) {
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
                case returnK:
                    fprintf(listing, "Return\n");
                    break;
                case callK:
                    fprintf(listing, "Call to Function: %s\n", tree->attr.name);
                    break;
                default:
                    break; // Ignore other statement kinds
            }
        } else if (tree->nodekind == expressionK) {
            switch (tree->kind.exp) {
                case operationK:
                    fprintf(listing, "Operation: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case constantK:
                    fprintf(listing, "Constant: %d\n", tree->attr.val);
                    break;
                case idK:
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                    break;
                default:
                    break; // Ignore other expression kinds
            }
        }
        for (i = 0; i < MAXCHILDREN; i++) {
            if (tree->child[i] != NULL) {
                printSpaces();
                fprintf(listing, "Child %d:\n", i);
                printAbstractTree(tree->child[i]);
            }
        }
        tree = tree->sibling;
        if (tree != NULL) {
            printSpaces();
            fprintf(listing, "Sibling:\n");
        }
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