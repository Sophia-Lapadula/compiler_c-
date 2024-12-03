%{
#include <stdio.h>
#include <ctype.h>
#include "auxparser.c"

int line_number = 1;
%}

%%
"else"      { return ("ELSE"); }
"if"        { return ("IF"); }
"int"       { return ("INT"); }
"return"    { return ("RETURN"); }
"void"      { return ("VOID"); }
"while"     { return("WHILE"); }

"+"         { return ("PLUS"); }
"-"         { return ("MIUS"); }
"*"         { return ("MULTI"); }
"/"         { return ("DIVIDE"); }
"<="        { return ("LEQUAL"); }
"<"         { return ("LESS"); }
">="        { return ("MEQUAL"); }
">"         { return ("MORE"); }
"=="        { return ("EQUAL"); }
"!="        { return ("DIF"); }
"="         { return ("ASSIGN"); }

","         { return ("COMA"); }
";"         { return ("SEMICOMA"); }
"\("        { return("LPARA"); }
"\)"        { return ("RPARA"); }
"\["        { return ("LBRACKET"); }
"\]"        { return ("RBRAKET"); }
"\{"        { return ("LKEY"); }
"\}"        { return ("RKEY"); }

[0-9]+      { return ("NUMBER", yytext); } 
[a-zA-Z][a-zA-Z0-9]* {
    //caso seja uma palavra chave
                if (!strcmp(yytext, "else") || !strcmp(yytext, "if") || 
                    !strcmp(yytext, "int") || !strcmp(yytext, "return") ||
                    !strcmp(yytext, "void") || !strcmp(yytext, "while")) {
                } else {
                    printf("ID", yytext);
                }
            }

[ \t]+      ;  // Ignorar espaços e tabulações

\n          { line_number++; }

.           { printf("ERRO LÉXICO: %s LINHA:%d\n", yytext, line_number); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}