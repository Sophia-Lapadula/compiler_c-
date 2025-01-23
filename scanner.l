%{
# include "globals.h"

extern FILE *yyin;

int line_number = 1;



%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
newline     \n
whitespace  [ \t\r]+

%%

"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"void"      { return VOID; }
"main"      { return MAIN; }
"input"     { return INPUT; }
"output"    { return OUTPUT; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return TIMES; }
"/"         { return DIVIDE; }
"="         { return ASSIGN; }

"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }

{newline}   { line_number++; }
{number}    { return NUM; }
{identifier} { return ID; }

"//".*      { /* skip single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+\/ { /* skip multi-line comments */ }

{whitespace} { /* ignore spaces and tabs */ }
.           { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

void getToken(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    yyin = input;
    yyparse();
    yylex(); /* Inicia a análise léxica */
}

int yywrap(void) {

    return 1;   
}

