%{
#include "globals.h"
#include "aux_parser.c"
int yylex(void);
void yyerror(const char *s);
%}

%token NUM ID
%token IF ELSE WHILE INT VOID RETURN
%token ASSIGN EQ NE LT LTE GT GTE PLUS MINUS TIMES DIVIDE LPAREN RPAREN LBRACKET RBRACKET LKEYS RKEYS COMMA SEMI
%token ERROR ENDFILE


%start program

%%

program:
    declaration_list
    ;

declaration_list:
    declaration_list declaration
    | declaration
    ;

declaration:
    var_declaration
    | fun_declaration
    ;

var_declaration:
    type_specifier ID SEMI
    | type_specifier ID LBRACKET NUM RBRACKET SEMI
    ;

type_specifier:
    INT | ID | VOID
    ;

fun_declaration:
    type_specifier ID LPAREN params RPAREN compound_stmt
    ;

params:
    param_list
    | VOID
    ;

param_list:
    param_list COMMA param
    | param
    ;

param:
    type_specifier ID
    | type_specifier ID LBRACKET RBRACKET
    ;

compound_stmt:
    LBRACKET local_declarations statement_list RBRACKET
    ;

local_declarations:
    local_declarations var_declaration
    | /* vazio */
    ;

statement_list:
    statement_list statement
    | /* vazio */
    ;

statement:
    expression_stmt
    | compound_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    ;

expression_stmt:
    expression SEMI
    | SEMI
    ;

selection_stmt:
    IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

iteration_stmt:
    WHILE LPAREN expression RPAREN statement
    ;

return_stmt:
    RETURN SEMI
    | RETURN expression SEMI
    ;

expression:
    var ASSIGN expression
    | simple_expression
    ;

var:
    ID
    | ID LBRACKET expression RBRACKET
    ;

simple_expression:
    additive_expression relop additive_expression
    | additive_expression
    ;

relop:
    LT
    | LTE
    | GT
    | GTE
    | EQ
    | NE
    ;

additive_expression:
    additive_expression addop term
    | term
    ;

addop:
    PLUS
    | MINUS
    ;

term:
    term mulop factor
    | factor
    ;

mulop:
    TIMES
    | DIVIDE
    ;

factor:
    LPAREN expression RPAREN
    | var
    | NUM
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

