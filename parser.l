%{
#include <stdio.h>
#include <ctype.h>

int line_number = 1;
%}

%%
"ELSE"      { printf("KEYWORD: else\n"); }
"IF"        { printf("KEYWORD: if\n"); }
"INT"       { printf("KEYWORD: int\n"); }
"RETURN"    { printf("KEYWORD: return\n"); }
"VOID"      { printf("KEYWORD: void\n"); }
"WHILE"     { printf("KEYWORD: while\n"); }

"PLUS"         { printf("OPERATOR: +\n"); }
"MINUS"         { printf("OPERATOR: -\n"); }
"MULTI"         { printf("OPERATOR: *\n"); }
"DIVIDE"         { printf("OPERATOR: /\n"); }
"LEQUAL"        { printf("OPERATOR: <=\n"); }
"LESS"         { printf("OPERATOR: <\n"); }
"MEQUAL"        { printf("OPERATOR: >=\n"); }
"MORE"         { printf("OPERATOR: >\n"); }
"EQUAL"        { printf("OPERATOR: ==\n"); }
"!="        { printf("OPERATOR: !=\n"); }
"ASSIGN"         { printf("OPERATOR: =\n"); }

"COMA"         { printf("SYMBOL: ,\n"); }
"SEMICOMA"         { printf("SYMBOL: ;\n"); }
"LBRACKET"        { printf("SYMBOL: (\n"); }
"\)"        { printf("SYMBOL: )\n"); }
"\["        { printf("SYMBOL: [\n"); }
"\]"        { printf("SYMBOL: ]\n"); }
"\{"        { printf("SYMBOL: {\n"); }
"\}"        { printf("SYMBOL: }\n"); }

[0-9]+      { printf("NUMBER: %s\n", yytext); } 
[a-zA-Z][a-zA-Z0-9]* {
                if (!strcmp(yytext, "else") || !strcmp(yytext, "if") || 
                    !strcmp(yytext, "int") || !strcmp(yytext, "return") ||
                    !strcmp(yytext, "void") || !strcmp(yytext, "while")) {
                    /* Already handled as keywords */
                } else {
                    printf("IDENTIFIER: %s\n", yytext);
                }
            }

[ \t]+      ;  // Ignorar espaços e tabulações

\n          { line_number++; }

"/\\*"([^*]|\*+[^*/])*\*+/"   ;  // Ignorar comentários

.           { printf("ERRO LÉXICO: %s LINHA:%d\n", yytext, line_number); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}