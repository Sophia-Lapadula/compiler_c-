# include "globals.h"

int line_number = 0;


int main(int argc, char *argv[]) {

    char pgm[120];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".cm");
    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s wasnot found\n", pgm);
        exit(1);
    }
    //listing = stdout;
    //fprintf(listing, "\nC- COMPILATION: %s\n", pgm);

    while (getToken() != ENDFILE);
    

    fclose(source);
    return 0;
}
