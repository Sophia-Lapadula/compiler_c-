#include "globals.h"
#include "symtab.h"
#include "semantic.h"

int line_number = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        exit(1);
    }

    source = fopen(argv[1], "r");
    listing = stdout;

    TreeNode *syntaxTree = NULL; // Carregar a árvore sintática aqui
    analyze(syntaxTree);         // Análise semântica

    fprintf(listing, "\nTabela de Símbolos:\n");
    printSymbolTable();

    fclose(source);
    return 0;
}
