#include "globals.h"
//#include "parser.h"   // Para chamar o parser => Tirar esse comentário no momento da junção
#include "semantic.h" // Para chamar a análise semântica
#include "symtab.h"   // Para a tabela de símbolos

int line_number = 0;

int main(int argc, char *argv[]) {
    char pgm[120]; // Nome do arquivo de entrada

    // Verifica se o nome do arquivo foi passado como argumento
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    // Copia o nome do arquivo e adiciona a extensão .cm se necessário
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".cm");

    // Abre o arquivo fonte para leitura
    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s was not found\n", pgm);
        exit(1);
    }

    listing = stdout; // Define a saída padrão para o console
    fprintf(listing, "\nC- COMPILATION: %s\n", pgm);

    // ======= Chama o parser para construir a árvore sintática =======
    TreeNode *syntaxTree = parse(); 

    // ======= Executa a análise semântica =======
    analyze(syntaxTree); 

    // ======= Imprime a tabela de símbolos =======
    fprintf(listing, "\nTabela de Símbolos:\n");
    printSymbolTable();

    // Fecha o arquivo fonte
    fclose(source);
    return 0;
}
