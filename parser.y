%{
# include "globals.h"

// Include the scanner header
int yylex(void);
void yyerror(const char *s);

%}

%token INT FLOAT CHAR VOID MAIN INPUT OUTPUT
%token IF ELSE WHILE
%token ID NUM
%token PLUS MINUS TIMES DIVIDE ASSIGN
%token LT LTE GT GTE EQ NEQ
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token RETURN ERROR

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
    type_specifier ID SEMICOLON
    | type_specifier ID LBRACE NUM RBRACE SEMICOLON
    ;

type_specifier:
    INT
    | FLOAT
    | CHAR
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
    | type_specifier ID LBRACE RBRACE
    ;

compound_stmt:
    LBRACE local_declarations statement_list RBRACE
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
    expression SEMICOLON
    | SEMICOLON
    ;

selection_stmt:
    IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

iteration_stmt:
    WHILE LPAREN expression RPAREN statement
    ;

return_stmt:
    RETURN SEMICOLON
    | RETURN expression SEMICOLON
    ;

expression:
    var ASSIGN expression
    | simple_expression
    ;

var:
    ID
    | ID LBRACE expression RBRACE
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
    | NEQ
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


