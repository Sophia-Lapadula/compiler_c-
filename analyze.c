//Function para verificas as heuristicas de tipos
# include "globals.h"
# include "symtab.h"
# include "analyze.h"

//Seta variável de erro e printa tipo de erro
static void typeError(TreeNode *t, char *message) {
    fprintf(listing, "Semantic error in %s at line %d: %s\n", t->attr.name, t->line_number, message);
    Error = TRUE;
}

static int location = 0;//Localização na tabela sintática

//Função para iterar sobre a árvore sintática
static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t);
        int i;
        for (i = 0; i < MAXCHILDREN; i++)
            traverse(t->child[i], preProc, postProc);
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}
//Procedimento para nó nulo
static void nullProc(TreeNode *t) {
    if (t == NULL)
        return;
    else
        return;
}
//Procedimento para inserir nó na tabela sintática
static void insertNode(TreeNode *t) {
    switch (t->nodekind) {
        //Verifica se o nó é uma declaração
        case statementK:
            switch (t->kind.stmt) {
                case variableK:
                    if (t->type == voidK)
                            typeError(t, "Invalid Declaration. Variable type must be integer.");
                    else {
                        if (st_lookup(t->attr.name, t->attr.scope) == -1) {
                            if (st_lookup(t->attr.name, "global") == -1 || strcmp(st_lookup_id(t->attr.name, "global"), "function") != 0)
                                st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "variable", "integer");
                            else 
                                typeError(t, "Invalid Declaration. Variable name already declared as function.");
                        }
                        else 
                            typeError(t, "Invalid Declaration. Variable already declared.");
                    }
                    break;
                case arrayK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
                        st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "vectordecl", "integer");
                    else
                        typeError(t, "Invalid Declaration. Array already declared.");
                    break;
                case paramK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1)
                        st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "param", "integer");
                    else
                        typeError(t, "Invalid Declaration. Parameter already declared.");
                    break;
                case functionK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1) {
                        if (t->type == integerK)
                            st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "function", "integer");
                        else
                            st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "function", "void");
                    }
                    else
                        typeError(t, "Invalid Declaration. Function already declared.");
                    break;
                case callK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
                        typeError(t, "Invalid Call. It was not declared.");
                    else
                        st_insert(t->attr.name, t->line_number, location++, t->attr.scope, "call", st_lookup_type(t->attr.name, "global"));
                case returnK:
                    break;
                default:
                    break;
            }
            break;
            //Verifica se a expressão é um id ou um vetor
        case expressionK:
            switch (t->kind.exp) {
                case idK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
                        typeError(t, "Variable not declared");
                    else
                        st_insert(t->attr.name, t->line_number, 0, t->attr.scope, "variable", "integer");
                    break;
                case vectorK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
                        typeError(t, "Vector not declared");
                    else
                        st_insert(t->attr.name, t->line_number, 0, t->attr.scope, "vector", "integer");
                    break;
                case vectorIdK:
                    if (st_lookup(t->attr.name, t->attr.scope) == -1 && st_lookup(t->attr.name, "global") == -1)
                        typeError(t, "Vector Index was not declared");
                    else
                        st_insert(t->attr.name, t->line_number, 0, t->attr.scope, "vector index", "integer");
                case typeK:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
//Construtor da tabela sintática
void buildSymtab(TreeNode *syntaxTree) {
    st_insert("input", 0, location++, "global", "function", "integer");
    st_insert("output", 0, location++, "global", "function", "integer");
    traverse(syntaxTree, insertNode, nullProc);
    if (st_lookup("main", "global") == -1) {
        printf("ERROR: MAIN NOT DECLARED\n");
        Error = TRUE;
    }
    printSymTab(listing);
    
}
//Procedimento para iterar sobre o nó e verificar tipos
static void checkNode(TreeNode *t) {
    switch (t->nodekind) {
        //Verifica se a expressão é válida
        case expressionK:
            switch (t->kind.exp) {
                case operationK:
                    break;
                default:
                    break;
            }
            break;
            //Verifica se a declaração é válida
        case statementK:
            switch (t->kind.stmt) {
                case ifK:
                    if (t->child[0]->type == integerK && t->child[1]->type == integerK)
                        typeError(t->child[0], "if test is not Boolean");
                    break;
                case assignK:
                    if (t->child[0]->type == voidK || t->child[1]->type == voidK)
                        typeError(t->child[0], "assignment of non-integer value");
                    else if (t->child[1]->kind.stmt == callK) {
                        if (strcmp(t->child[1]->attr.name, "input") != 0 && strcmp(t->child[1]->attr.name, "output") != 0) {
                            if (strcmp(st_lookup_type(t->child[1]->attr.name, t->child[1]->attr.scope), "void") == 0)
                                typeError(t->child[1], "Error 2: assignment of void return");
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
//Procedimento para verificar tipos
void typeCheck(TreeNode *syntaxTree) {
    
    traverse(syntaxTree, nullProc, checkNode);
}
