# ifndef _SYMTAB_H_
# define _SYMTAB_H_

void st_insert( char * name, int lineno, int loc, char* scope, char* typeID, char* typeData);

int st_lookup (char * name, char* scope );

char* st_lookup_type(char* name, char* scope);

char* st_lookup_id (char* name, char* scope);

void printSymTab(FILE * listing);

# endif
