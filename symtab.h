# ifndef _SYMTAB_H_
# define _SYMTAB_H_
# define SIZE 211
# define SHIFT 4

typedef struct LineListRec { 
    int line_number;
    struct LineListRec * next;
} *LineList;//Lista de linhas

typedef struct BucketListRec { 
    char* name;
    LineList lines;
    int memloc ; 
	char* scope;
	char* typeID;
	char* typeData; 
    struct BucketListRec * next; 
} *BucketList;//Lista de buckets

//Funções de symtab.c
void st_insert( char * name, int line_number, int loc, char* scope, char* typeID, char* typeData);

int st_lookup (char * name, char* scope );

char* st_lookup_type(char* name, char* scope);

char* st_lookup_id (char* name, char* scope);

void printSymTab(FILE * listing);


# endif
