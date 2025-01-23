#!/bin/bash

# Generate the parser files from parser.y
bison -d parser.y


# Generate the scanner file from scanner.l
flex scanner.l

# Compile the generated C files
gcc parser.tab.c lex.yy.c main.c -o parser -lfl

# Run the parser with an optional input file
if [ "$#" -eq 1 ]; then
    ./parser "$1"
else
    ./parser
fi