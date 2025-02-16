# compiler_c-
Trabalho prático de compiladores para a linguagem fictícia  C-

Professor: Luiz Galvão

Alunas:
- Luiza Saliba (163931)
- Sophia Lapadula (148282)

# Objetivo
Construir um compilador para a linguagem C- que demonstre os aprendizados práticos da matéria de compiladores

# Sobre o código em cada arquivo
## README.md
Documento com a descrição geral do projeto, objetivos, instruções de compilação e execução, bem como informações sobre os testes e a arquitetura do compilador.

## parser.y
Arquivo fonte do Bison que define a gramática da linguagem C- e especifica as ações associadas a cada produção, sendo responsável pela criação da árvore sintática.

## scanner.l
Arquivo fonte do Flex que contém as regras léxicas para tokenizar o código-fonte da linguagem C-, identificando palavras-chave, identificadores, constantes e operadores.

## symtab.c / symtab.h
Implementação e definição da tabela de símbolos, contendo funções para inserção, busca e gerenciamento dos símbolos (identificadores, tipos, linhas de ocorrência, etc.) durante a análise semântica.

## aux_parser.c
Contém funções auxiliares para o tratamento da árvore sintática, como a criação de nós, impressão da árvore (inclusive com visualização gráfica por meio de prefixos) e possíveis outras utilidades de suporte ao parser.

## parser.tab.c / parser.tab.h
Arquivos gerados automaticamente pelo Bison a partir do arquivo parser.y. Eles contêm o código do parser, incluindo a análise sintática e a construção da árvore sintática.


# Como compilar
Basta usar o make file. Digite os comandos na seguinte ordem:

make
./exec ./tests/{arquivo que deseja testar}


