#include "globals.h"   /* Header gerado pelo Bison (declara a função parse) */
#include "aux_parser.h" /* Funções auxiliares para a construção da árvore sintática */
#include "aux_scanner.h" /* Funções auxiliares para o scanner */
# include "analyze.h"


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
   
    fprintf(listing, "\nSyntax tree:\n");
    printTree(syntaxTree);
    fprintf(listing, "\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
    fprintf(listing, "\nChecking Types...\n");
    typeCheck(syntaxTree);
    fclose(source);
    return 0;
}
