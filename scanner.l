%{
#include <stdio.h>
%}

/* Seçãozinha da definição de tokens */
%token COMMENT

/* Regras do analisador léxico */
%%

/\*([^*]|\*+[^*/])*\*+/ {
    /* Ignorar comentários de bloco */
}

\/\/[^\n]* {
    /* Ignorar comentários de linha */
}


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
