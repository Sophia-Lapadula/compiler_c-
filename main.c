#include "globals.h"

//Arquivo para pedir pelo terminal que o usuário determine o arquivo a ser compilado
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    if (argc > 1) {
        getToken(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    return 0;
}