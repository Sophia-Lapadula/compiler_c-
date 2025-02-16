//Funções auxiliares para a construção da árvore sintática
#include "globals.h"
#include "aux_parser.h"


//Função que propaga um escopo para toda a árvore 't'
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

//Cria um novo nó de declaração 
TreeNode *newStmtNode(StatementKind kind) {
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory in line %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = statementK;
        t->kind.stmt = kind;
        t->line_number = line_number;
        t->attr.scope = "global";
    }
    return t;
}

//Cria um nó de expressão
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
        t->type = VOID;
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
//Printa árvore

void printTreeGraphHelper(TreeNode* tree, char* prefix, int isTail) {
    if (tree == NULL) 
        return;
    
    // Imprime o prefixo e o marcador (└── ou ├──)
    fprintf(listing, "%s", prefix);
    if (isTail)
        fprintf(listing, "└── ");
    else
        fprintf(listing, "├── ");
    
    // Exibe informações do nó conforme seu tipo
    if (tree->nodekind == statementK) {
        switch(tree->kind.stmt) {
            case ifK:
                fprintf(listing, "If");
                break;
            case assignK:
                fprintf(listing, "Assign");
                break;
            case whileK:
                fprintf(listing, "While");
                break;
            case variableK:
                fprintf(listing, "Variable (%s)", tree->attr.name);
                break;
            case arrayK:
                fprintf(listing, "Array (%s)", tree->attr.name);
                break;
            case paramK:
                fprintf(listing, "Param (%s)", tree->attr.name);
                break;
            case functionK:
                fprintf(listing, "Function (%s)", tree->attr.name);
                break;
            case callK:
                fprintf(listing, "Call to Function (%s)", tree->attr.name);
                break;
            case returnK:
                fprintf(listing, "Return");
                break;
            default:
                fprintf(listing, "Unknown statement kind");
                break;
        }
    } else if (tree->nodekind == expressionK) {
        switch(tree->kind.exp) {
            case operationK:
                fprintf(listing, "Operation: ");
                printToken(tree->attr.op, "\0");
                break;
            case constantK:
                fprintf(listing, "Constant (%d)", tree->attr.val);
                break;
            case idK:
                fprintf(listing, "Id (%s)", tree->attr.name);
                break;
            case vectorK:
                fprintf(listing, "Vector (%s)", tree->attr.name);
                break;
            case vectorIdK:
                fprintf(listing, "Index [%d]", tree->attr.val);
                break;
            case typeK:
                fprintf(listing, "Type (%s)", tree->attr.name);
                break;
            default:
                fprintf(listing, "Unknown expression kind");
                break;
        }
    } else {
        fprintf(listing, "Unknown node kind");
    }
    fprintf(listing, "\n");

    // Define o prefixo para os filhos
    char childPrefix[1024];
    snprintf(childPrefix, sizeof(childPrefix), "%s%s", prefix, isTail ? "    " : "│   ");
    
    // Conta quantos filhos (não nulos) o nó possui
    int childCount = 0, i;
    for (i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL)
            childCount++;
    }
    
    // Imprime recursivamente cada filho
    int count = 0;
    for (i = 0; i < MAXCHILDREN; i++) {
        if (tree->child[i] != NULL) {
            count++;
            printTreeGraphHelper(tree->child[i], childPrefix, count == childCount);
        }
    }
    
    // Imprime os irmãos do nó corrente com o mesmo prefixo
    if (tree->sibling != NULL)
        printTreeGraphHelper(tree->sibling, prefix, 0);
}
//Função que printa a árvore
void printTree(TreeNode* tree) {
    
    printTreeGraphHelper(tree, "", 1);
    
}








