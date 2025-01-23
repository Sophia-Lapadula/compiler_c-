//Librarys all archieves need for the parser

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>

# include "parser.tab.h"

//Variable that stores the line number
extern int line_number;

extern void getToken(const char *filename);