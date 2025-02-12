#include "globals.h"   /* Header gerado pelo Bison (declara a função parse) */
#include "aux_parser.h" /* Funções auxiliares para a construção da árvore sintática */
#include "aux_scanner.h" /* Funções auxiliares para o scanner */
#include "semantic.h" // Para chamar a análise semântica
#include "symtab.h"   // Para manipular a tabela de símbolos
#include "util.h"


/* Definição das variáveis globais */
FILE *source;      /* Declarado como extern em globals.h */
FILE *listing;     /* Declarado como extern em globals.h */
int line_number = 0;

int main(int argc, char *argv[]) {
    char pgm[120];
    TreeNode *syntaxTree;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".cm");

    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s was not found\n", pgm);
        exit(1);
    }


    
    listing = stdout;
    fprintf(listing, "\nC- COMPILATION: %s\n", pgm);

    /* Chama o parser para construir a árvore sintática */
    syntaxTree = parse();
   
    fprintf(listing, "\n-------------------Arvore Sintatica---------------------:\n");
    printTree(syntaxTree);
   
    // Chama a análise semântica para validar o código e preencher a tabela de símbolos
    analyze(syntaxTree);

    // ========================== IMPRESSÃO DA TABELA DE SÍMBOLOS ==========================

    fprintf(listing, "\nTabela de Símbolos:\n");
    printSymbolTable(); // Imprime todos os símbolos e suas ocorrências

    freeTree(syntaxTree);
    
    fclose(source);
    return 0;
}
