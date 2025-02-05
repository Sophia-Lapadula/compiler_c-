#include "globals.h"
#include "symtab.h"

// ================== INÍCIO DAS DEFINIÇÕES DA ÁRVORE ==================

// Máximo de filhos por nó (pode ser ajustado conforme a gramática)
#define MAXCHILDREN 3

// Tipos de nós da árvore
typedef enum { DeclK, StmtK, ExpK } NodeKind;

// Exemplos de tipos específicos para cada categoria (ajuste conforme necessário)
typedef enum { VarK, FunK, ParamK } DeclKind;
typedef enum { CompK, IfK, WhileK, ReturnK } StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;

// Tipos de dados suportados
typedef enum { Integer, Void } ExpType;

// Estrutura do nó da árvore sintática (AST)
typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN]; // Filhos do nó
    struct treeNode *sibling;            // Nó irmão (para juntar declarações ou comandos em sequência)
    int lineno;                          // Número da linha
    NodeKind nodeKind;                   // Tipo do nó: declaração, comando ou expressão
    union {
        DeclKind decl;  // Se for nó de declaração
        StmtKind stmt;  // Se for nó de comando
        ExpKind exp;    // Se for nó de expressão
    } kind;
    // Atributos (por exemplo, para armazenar valores constantes ou nomes de identificadores)
    int attr_val;      // Ex.: valor numérico
    char *attr_name;   // Ex.: nome do identificador ou operador
    ExpType type;      // Tipo da variável ou retorno da função
} TreeNode;

// Ponteiro global para a árvore sintática
extern TreeNode *syntaxTree;

// Protótipo da função para imprimir a árvore
void printTree(TreeNode *tree, int indent);

// =================== FIM DAS DEFINIÇÕES DA ÁRVORE ===================
// =================== FUNÇÕES AUXILIARES PARA A ÁRVORE ===================

// Cria um novo nó para comandos (statements)
TreeNode *newStmtNode(StmtKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Erro de alocação na linha %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->lineno = line_number;
        t->nodeKind = StmtK;
        t->kind.stmt = kind;
    }
    return t;
}

// Cria um novo nó para expressões
TreeNode *newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Erro de alocação na linha %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->lineno = line_number;
        t->nodeKind = ExpK;
        t->kind.exp = kind;
    }
    return t;
}

// Cria um novo nó para declarações
TreeNode *newDeclNode(DeclKind kind, ExpType type, char *name) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Erro de alocação na linha %d\n", line_number);
    else {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->lineno = line_number;
        t->nodeKind = DeclK;
        t->kind.decl = kind;
        t->type = type;
        t->attr_name = strdup(name);

        // Insere na tabela de símbolos
        insertSymbol(name, line_number, DeclK, type);
    }
    return t;
}

// Declaração global para armazenar a árvore sintática
TreeNode *syntaxTree = NULL;

// =======================================================================
// Função que imprime a árvore sintática gerada
void printTree(TreeNode *tree, int indent) {
    int i;
    while (tree != NULL) {
        for (i = 0; i < indent; i++)
            fprintf(listing, " ");
        
        // Imprime informações dependendo do tipo do nó
        if (tree->nodeKind == StmtK) {
            switch (tree->kind.stmt) {
                case IfK:
                    fprintf(listing, "If Statement");
                    break;
                case WhileK:
                    fprintf(listing, "While Statement");
                    break;
                case ReturnK:
                    fprintf(listing, "Return Statement");
                    break;
                case CompK:
                    fprintf(listing, "Compound Statement");
                    break;
                default:
                    fprintf(listing, "Unknown Statement");
            }
        } else if (tree->nodeKind == ExpK) {
            if (tree->kind.exp == OpK)
                fprintf(listing, "Op: %s", tree->attr_name);
            else if (tree->kind.exp == ConstK)
                fprintf(listing, "Const: %d", tree->attr_val);
            else if (tree->kind.exp == IdK) {
                fprintf(listing, "Id: %s", tree->attr_name);
                // Verificar se o identificador já foi declarado
                if (lookupSymbol(tree->attr_name) == NULL) {
                    fprintf(stderr, "Erro: variável '%s' usada sem declaração na linha %d.\n", tree->attr_name, tree->lineno);
                }
            }
        } else if (tree->nodeKind == DeclK) {
            if (tree->kind.decl == VarK)
                fprintf(listing, "Var Declaration: %s (Tipo: %s)", tree->attr_name, tree->type == Integer ? "int" : "void");
            else if (tree->kind.decl == FunK)
                fprintf(listing, "Fun Declaration: %s (Retorno: %s)", tree->attr_name, tree->type == Integer ? "int" : "void");
            else if (tree->kind.decl == ParamK)
                fprintf(listing, "Param Declaration: %s", tree->attr_name);
        }
        fprintf(listing, "\n");

        // Imprime os filhos com indentação adicional
        for (i = 0; i < MAXCHILDREN; i++) {
            if (tree->child[i] != NULL)
                printTree(tree->child[i], indent + 4);
        }
        // Procede para o nó irmão
        tree = tree->sibling;
    }
}
