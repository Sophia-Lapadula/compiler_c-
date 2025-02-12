# Nome do executável
TARGET = compilador

# Fontes do projeto
BISON_SRC      = parser.y
FLEX_SRC       = scanner.l
MAIN_SRC       = main.cpp
AUXPARSER_SRC  = aux_parser.cpp
AUXSCANNER_SRC = aux_scanner.cpp
SEMANTIC_SRC   = semanitc.cpp
SYMTAB_SRC     = symtab.cpp
UTIL_SRC       = util.cpp
HASH_SRC       = hash.cpp

# Arquivos gerados pelo Bison e Flex
BISON_C        = parser.tab.c
BISON_H        = parser.tab.h
FLEX_C         = lex.yy.c

# Compilador e flags
CXX      = g++
CXXFLAGS = -Wall -g
LDFLAGS  = -lfl  # Apenas -lfl, sem -ly

# Regra padrão
all: $(TARGET)

# Regra para gerar os arquivos do Bison (header e source)
$(BISON_C) $(BISON_H): $(BISON_SRC)
    bison -d $(BISON_SRC)

# Regra para gerar o arquivo do Flex
$(FLEX_C): $(FLEX_SRC)
    flex $(FLEX_SRC)

# Garante que main.o dependa do header gerado pelo Bison
main.o: $(BISON_H)

# Regra genérica para compilar arquivos .c em .o
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Lista de objetos
OBJS = main.o $(BISON_C:.c=.o) $(FLEX_C:.c=.o) aux_parser.o aux_scanner.o semanitc.o symtab.o util.o hash.o

# Regra para linkar e gerar o executável
$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Limpeza dos arquivos gerados
clean:
    rm -f $(TARGET) $(OBJS) $(BISON_C) $(BISON_H) $(FLEX_C)

.PHONY: all clean