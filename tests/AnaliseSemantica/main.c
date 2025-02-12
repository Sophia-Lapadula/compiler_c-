#include "globals.h"
#include "parser.h"   // Para chamar o parser e construir a árvore sintática
#include "semantic.h" // Para chamar a análise semântica
#include "symtab.h"   // Para manipular a tabela de símbolos

int line_number = 0; // Contador global de linhas do código-fonte

int main(int argc, char *argv[]) {
    char pgm[120]; // Nome do arquivo de entrada

    // ========================== VERIFICAÇÃO DO ARQUIVO ==========================

    // Verifica se o usuário passou um arquivo como argumento
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        exit(1);
    }

    // Copia o nome do arquivo para a variável `pgm`
    strcpy(pgm, argv[1]);

    // Se o arquivo não tiver extensão, adiciona ".cm" automaticamente
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".cm");

    // Tenta abrir o arquivo para leitura
    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "Erro: Arquivo %s não encontrado.\n", pgm);
        exit(1);
    }

    listing = stdout; // Define a saída padrão para exibição de mensagens
    fprintf(listing, "\nC- COMPILATION: %s\n", pgm);

    // ========================== ANÁLISE SINTÁTICA ==========================

    // Chama o parser para construir a árvore sintática
    TreeNode *syntaxTree = parse();

    // ========================== ANÁLISE SEMÂNTICA ==========================

    // Chama a análise semântica para validar o código e preencher a tabela de símbolos
    analyze(syntaxTree);

    // ========================== IMPRESSÃO DA TABELA DE SÍMBOLOS ==========================

    fprintf(listing, "\nTabela de Símbolos:\n");
    printSymbolTable(); // Imprime todos os símbolos e suas ocorrências

    // Fecha o arquivo fonte após a análise
    fclose(source);

    return 0; // Indica execução bem-sucedida
}
