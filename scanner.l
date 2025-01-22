%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "y.tab.h"

int line_number = 1;
%}

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
";"         { return SEMICOLON; }
","         { return COMMA; }

[0-9]+      { yylval.intval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return ID; }

[ \t\n]     { /* ignore whitespace */ }
.           { /* ignore any other character */ }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *input = fopen(argv[1], "r");
        if (!input) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        yyin = input;
    }

    yylex(); /* Inicia a análise léxica */
    return 0;
}

int yywrap() {
    return 1;
}
