%{
#include <stdio.h>
#include <ctype.h>
#include "auxparser.c"

int line_number = 1;
%}

%%
"else"      { printf ("ELSE"); }
"if"        { printf ("IF"); }
"int"       { printf ("INT"); }
"return"    { printf ("RETURN"); }
"void"      { printf ("VOID"); }
"while"     { printf("WHILE"); }

"+"         { printf ("PLUS"); }
"-"         { printf ("MINUS"); }
"*"         { printf ("MULTI"); }
"/"         { printf ("DIVIDE"); }
"<="        { printf ("LEQUAL"); }
"<"         { printf ("LESS"); }
">="        { printf ("MEQUAL"); }
">"         { printf ("MORE"); }
"=="        { printf ("EQUAL"); }
"!="        { printf ("DIF"); }
"="         { printf ("ASSIGN"); }

","         { printf ("COMA"); }
";"         { printf ("SEMICOMA"); }
"\("        { printf("LPARA"); }
"\)"        { printf ("RPARA"); }
"\["        { printf ("LBRACKET"); }
"\]"        { printf ("RBRAKET"); }
"\{"        { printf ("LKEY"); }
"\}"        { printf("RKEY"); }

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

[ \t]+      {printf(" ")};  // Ignorar espaços e tabulações

\n          {printf("\n"); line_number++; }

/\*([^*]|\*+[^*/])*\*+/ {
    /* Ignorar comentários de bloco */
}

\/\/[^\n]* {
    /* Ignorar comentários de linha */
}

. ;

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