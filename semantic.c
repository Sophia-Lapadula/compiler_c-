#include "semantic.h"
#include "symtab.h"
#include "globals.h"

// ========================== CONTROLE DE ESCOPO ==========================
Scope currentScope = NULL;

ExpressionType currentFunctionType;
char *currentFunctionName;

// ========================== PERCORRER A ÁRVORE ==========================
static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t);
        for (int i = 0; i < MAXCHILDREN; i++)
            traverse(t->child[i], preProc, postProc);
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

// ========================== INSERÇÃO NA TABELA DE SÍMBOLOS ==========================
static void insertNode(TreeNode *t) {
    if (t->nodekind == statementK) {
        switch (t->kind.stmt) {
            case functionK:
                insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
                enterScope(t->attr.name);
                currentFunctionType = t->type;
                currentFunctionName = t->attr.name;
                break;
            case variableK:
                insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
                break;
            case arrayK:
                insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
                break;
            case paramK:
                insertSymbol(t->attr.name, t->line_number, t->nodekind, t->type);
                break;
            case ifK:
                enterScope("if");
                break;
            case whileK:
                enterScope("while");
                break;
            case assignK:
                // Verifica tipo de atribuição.
                if(t->child[0] != NULL && t->child[1] != NULL) {
                    if(t->child[0]->type != t->child[1]->type) {
                        fprintf(stderr, "Erro de tipo na linha %d: Atribuição incompatível.\n", t->line_number);
                        Error = TRUE;
                    }
                }
                break;
        }
    }
}


static void checkNode(TreeNode *t) {
    if (t->nodekind == expressionK && t->kind.exp == idK) {
        BucketList symbol = lookupSymbol(t->attr.name);
        if (symbol == NULL) {
            fprintf(stderr, "Erro: Identificador não declarado '%s' na linha %d, coluna %d.\n", t->attr.name, t->line_number, 0); // Adicione coluna
            Error = TRUE;
        } else {
            addLineNumber(t->attr.name, t->line_number);
        }
    } else if (t->nodekind == statementK && t->kind.stmt == callK) {
        checkFunctionCall(t);
    }
}

void checkType(TreeNode *t) {
    if (t->nodekind == expressionK && t->kind.exp == operationK) {
        TreeNode *left = t->child[0];
        TreeNode *right = t->child[1];
        if (left->type != integerK || right->type != integerK) {
            fprintf(stderr, "Erro de tipo na linha %d: Operação requer inteiros.\n", t->line_number);
            Error = TRUE;
        }
        t->type = integerK; // Define o tipo do nó operação.
    } else if (t->nodekind == expressionK && t->kind.exp == constantK) {
        t->type = integerK;
    } else if (t->nodekind == expressionK && t->kind.exp == idK) {
        BucketList symbol = lookupSymbol(t->attr.name);
        if (symbol != NULL) {
            t->type = symbol->type;
        }
    }
}

void checkFunctionCall(TreeNode *t) {
    if (t->nodekind == statementK && t->kind.stmt == callK) {
        BucketList func = lookupSymbol(t->attr.name);
        if (func == NULL || func->kind != functionK) {
            fprintf(stderr, "Erro: Função '%s' não declarada na linha %d.\n", t->attr.name, t->line_number);
            Error = TRUE;
        }
    }
}

void checkReturn(TreeNode *t) {
    if (t->nodekind == statementK && t->kind.stmt == returnK) {
        if (currentFunctionType == integerK && t->child[0] == NULL) {
            fprintf(stderr, "Erro na linha %d: Função '%s' deve retornar um inteiro.\n", t->line_number, currentFunctionName);
            Error = TRUE;
        } else if (currentFunctionType == voidK && t->child[0] != NULL) {
            fprintf(stderr, "Erro na linha %d: Função '%s' não deve retornar um valor.\n", t->line_number, currentFunctionName);
            Error = TRUE;
        }
    }
}

void analyze(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, checkNode);
    traverse(syntaxTree, checkType, NULL);
    leaveScope();
}