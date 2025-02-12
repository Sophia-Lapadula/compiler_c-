%{
#define YYPARSER
#define YYSTYPE TreeNode*

#include "globals.h"
#include "aux_scanner.h"
#include "aux_parser.h"
#include "symtab.h"
#include "util.h"

static TreeNode *savedTree;
static int yylex(void);
int Error;
int yyerror(char *);

//currentScope = NULL; // Variável para o escopo atual

%}

%expect 1

%token NUM ID
%token IF ELSE WHILE INT VOID RETURN
%token ASSIGN EQ NE LT LTE GT GTE PLUS MINUS TIMES DIVIDE
%token LPAREN RPAREN LBRACKET RBRACKET LKEYS RKEYS COMMA SEMI
%token ERROR ENDFILE


%%

program             :   list_declaration
                        {
                          savedTree = $1;
                        }
                    ;
list_declaration    :   list_declaration declaration
                        {
                            TreeNode *t = $1;
                            if(t != NULL) {
                                while(t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            } else {
                                $$ = $2;
                            }
                        }
                    |   declaration
                        {
                           $$ = $1;
                        }
                    ;
declaration         :   var_declaration
                        {
                           $$ = $1;
                        }
                    |   fun_declaration
                        {
                           $$ = $1;
                        }
                    ;
var_declaration     :   INT identfier SEMI
                        {
                            $$ = newStmtNode(variableK);
                            $$->type = integerK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   VOID identfier SEMI
                        {
                            $$ = newStmtNode(variableK);
                            $$->type = voidK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   INT identfier LBRACKET number RBRACKET SEMI
                        {
                            $$ = newStmtNode(arrayK);
                            $$->type = integerK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                            $$->attr.len = $4->attr.val;
                        }
                    ;
fun_declaration     :   INT identfier LPAREN params RPAREN compound_decl
                        {
                            $$ = newStmtNode(functionK);
                            $$->type = integerK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = $2->attr.name;
                            $$->child[0] = $4;
                            $$->child[1] = $6;
                        }
                    |   VOID identfier LPAREN params RPAREN compound_decl
                        {
                            $$ = newStmtNode(functionK);
                            $$->type = voidK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = $2->attr.name;
                            $$->child[0] = $4;
                            $$->child[1] = $6;
                        }
                    ;
params              :   param_list
                        {
                           $$ = $1;
                        }
                    |   VOID
                        {
                            $$ = newExpNode(typeK);
                            $$->attr.name = "void";
                            $$->type = voidK;
                        }
                    ;
param_list          :   param_list COMMA param
                        {
                            TreeNode *t = $1;
                            if (t != NULL) {
                                while (t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $3;
                                $$ = $1;
                            } else {
                                $$ = $3;
                            }
                        }
                    |   param
                        {
                            $$ = $1;
                        }
                    ;
param               :   INT identfier
                        {
                            $$ = newStmtNode(paramK);
                            $$->type = integerK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   INT identfier LBRACKET RBRACKET
                        {
                            $$ = newStmtNode(paramK);
                            $$->type = integerK;
                            $$->attr.name = $2->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
compound_decl       :   LKEYS local_declarations statement_list RKEYS
                        {
                            TreeNode *t = $2;
                            if (t != NULL) {
                                while (t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $3;
                                $$ = $2;
                            } else {
                                $$ = $3;
                            }
                        }
                    |   LKEYS local_declarations RKEYS
                        {
                            $$ = $2;
                        }
                    |   LKEYS statement_list RKEYS
                        {
                            $$ = $2;
                        }
                    |   LKEYS RKEYS
                        {
                            $$ = NULL;
                        }
                    ;
local_declarations  :   local_declarations var_declaration
                        {
                            TreeNode *t = $1;
                            if (t != NULL) {
                                while (t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            } else {
                                $$ = $2;
                            }
                        }
                    |   var_declaration
                        {
                            $$ = $1;
                        }
                    ;
statement_list      :   statement_list statement
                        {
                            TreeNode *t = $1;
                            if (t != NULL) {
                                while (t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                            } else {
                                $$ = $2;
                            }
                        }
                    |   statement
                        {
                            $$ = $1;
                        }
                    ;
statement           :   expression_decl
                        {
                            $$ = $1;
                        }
                    |   compound_decl
                        {
                            $$ = $1;
                        }
                    |   selection_decl
                        {
                            $$ = $1;
                        }
                    |   iterator_decl
                        {
                            $$ = $1;
                        }
                    |   return_decl
                        {
                            $$ = $1;
                        }
                    ;
expression_decl     :   expression SEMI
                        {
                            $$ = $1;
                        }
                    |   SEMI
                        {
                            $$ = NULL;
                        }
                    ;
selection_decl      :   IF LPAREN expression RPAREN statement
                        {
                            $$ = newStmtNode(ifK);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   IF LPAREN expression RPAREN statement ELSE statement
                        {
                            $$ = newStmtNode(ifK);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                            $$->child[2] = $7;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
iterator_decl       :   WHILE LPAREN expression RPAREN statement
                        {
                            $$ = newStmtNode(whileK);
                            $$->child[0] = $3;
                            $$->child[1] = $5;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
return_decl         :   RETURN SEMI
                        {
                            $$ = newStmtNode(returnK);
                            $$->type = voidK;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   RETURN expression SEMI
                        {
                            $$ = newStmtNode(returnK);
                            $$->child[0] = $2;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
expression          :   var ASSIGN expression
                        {
                            $$ = newStmtNode(assignK);
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                            $$->type = $3->type; // Propaga o tipo da expressão
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   simple_expression
                        {
                            $$ = $1;
                        }
                    ;
var                 :   identfier
                        {
                            $$ = $1;
                        }
                    |   identfier LBRACKET expression RBRACKET
                        {
                            $$ = $1;
                            $$->child[0] = $3;
                            $$->kind.exp = vectorK;
                            $$->type = integerK; // Assume tipo inteiro para vetor
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
simple_expression   :   sum_expression relational sum_expression
                        {
                            $$ = newExpNode(operationK);
                            $$->attr.op = $2->attr.op;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                            $$->type = booleanK; // Tipo booleano para expressões relacionais
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   sum_expression
                        {
                            $$ = $1;
                        }
                    ;
relational          :   EQ    { $$ = newExpNode(operationK); $$->attr.op = EQ; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   NE    { $$ = newExpNode(operationK); $$->attr.op = NE; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   LT    { $$ = newExpNode(operationK); $$->attr.op = LT; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   LTE   { $$ = newExpNode(operationK); $$->attr.op = LTE; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   GT    { $$ = newExpNode(operationK); $$->attr.op = GT; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   GTE   { $$ = newExpNode(operationK); $$->attr.op = GTE; $$->type = booleanK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    ;
sum_expression      :   sum_expression sum term
                        {
                            $$ = newExpNode(operationK);
                            $$->attr.op = $2->attr.op;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                            $$->type = integerK; // Tipo inteiro para soma
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   term
                        {
                            $$ = $1;
                        }
                    ;
sum                 :   PLUS  { $$ = newExpNode(operationK); $$->attr.op = PLUS; $$->type = integerK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   MINUS { $$ = newExpNode(operationK); $$->attr.op = MINUS; $$->type = integerK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    ;
term                :   term mult factor
                        {
                            $$ = newExpNode(operationK);
                            $$->attr.op = $2->attr.op;
                            $$->child[0] = $1;
                            $$->child[1] = $3;
                            $$->type = integerK; // Tipo inteiro para multiplicação/divisão
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   factor
                        {
                            $$ = $1;
                        }
                    ;
mult                :   TIMES { $$ = newExpNode(operationK); $$->attr.op = TIMES; $$->type = integerK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    |   DIVIDE{ $$ = newExpNode(operationK); $$->attr.op = DIVIDE; $$->type = integerK; $$->attr.scope = currentScope ? currentScope->name : "global"; }
                    ;
factor              :   LPAREN expression RPAREN
                        {
                            $$ = $2;
                        }
                    |   var
                        {
                            $$ = $1;
                        }
                    |   activation
                        {
                            $$ = $1;
                        }
                    |   number
                        {
                            $$ = $1;
                        }
                    ;
activation          :   identfier LPAREN arg_list RPAREN
                        {
                            $$ = newStmtNode(callK);
                            $$->attr.name = $1->attr.name;
                            $$->child[0] = $3;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    |   identfier LPAREN RPAREN
                        {
                            $$ = newStmtNode(callK);
                            $$->attr.name = $1->attr.name;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
arg_list            :   arg_list COMMA expression
                        {
                            TreeNode *t = $1;
                            if (t != NULL) {
                                while (t->sibling != NULL)
                                    t = t->sibling;
                                t->sibling = $3;
                                $$ = $1;
                            } else {
                                $$ = $3;
                            }
                        }
                    |   expression
                        {
                            $$ = $1;
                        }
                    ;
identfier           :   ID
                        {
                            $$ = newExpNode(idK);
                            $$->attr.name = copyString(tokenBuffer);
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
number              :   NUM
                        {
                            $$ = newExpNode(constantK);
                            $$->attr.val = atoi(tokenBuffer);
                            $$->type = integerK;
                            $$->attr.scope = currentScope ? currentScope->name : "global";
                        }
                    ;
%%

int yyerror(char* message) {
    fprintf(listing, "Syntax error at line %d: %s\n", line_number, message);
    fprintf(listing, "On token: ");
    printToken(yychar, tokenBuffer);
    Error = TRUE;
    return 0;
}

static int yylex(void) {
    return getToken();
}

TreeNode* parse(void) {
    currentScope = NULL; // Inicializa o escopo global
    yyparse();
    return savedTree;
}