# compiler_c-
Trabalho prático de compiladores para a linguagem fictícia  C-

Professor: Luiz Galvão

Alunas:
- Luiza Saliba (163931)
- Sophia Lapadula (148282)

# Objetivo
Construir um compilador para a linguagem C- que demonstre os aprendizados práticos da matéria de compiladores

# Arquitetura do compilador
Teste

# Compiler Project

## Compilation and Execution

### Step 1: Generate the Parser

Use Bison to generate the parser files.

```sh
bison -d parser.y
```

This command will generate `parser.tab.c` and `parser.tab.h`.

### Step 2: Generate the Scanner

Use Flex to generate the scanner file.

```sh
flex scanner.l
```

This command will generate `lex.yy.c`.

### Step 3: Compile the Scanner and Parser

Compile the generated C files using GCC.

```sh
gcc -o parser parser.tab.c lex.yy.c -lfl
```

### Step 4: Run the Parser

Run the parser with an input file.

```sh
./parser input_file.c
```

Replace `input_file.c` with the path to your input file.

## Notes

- Ensure that `bison` and `flex` are installed on your system.
- The `-lfl` flag links the Flex library.
