# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "symtab.h"


static BucketList hashTable[SIZE]; //Tamanho da tabela sintática

//Função de hash 
static int hash (char* name, char* scope) { 
    int temp = 0;
    int i = 0;
    while (name[i] != '\0') { 
        temp = ((temp << SHIFT) + name[i]) % SIZE;
        ++i;
    }
    i = 0;
    while (scope[i] != '\0') { 
        temp = ((temp << SHIFT) + scope[i]) % SIZE;
        ++i;
    }
    return temp;
}

//Insere nó na tabela sintática
void st_insert( char * name, int line_number, int loc, char* scope, char* typeID, char* typeData) { 
    int h = hash(name, scope);
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0)) 
            l = l->next;
    if (l == NULL) {
        /* variable not yet in table */
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LineListRec));
        l->lines->line_number = line_number;
        l->memloc = loc;
        l->lines->next = NULL;
        l->scope = scope;
        l->typeID = typeID;
        l->typeData = typeData;
        l->next = hashTable[h];
        hashTable[h] = l; 
    }
    else {
        LineList t = l->lines;
        while (t->next != NULL) 
            t = t->next;
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->line_number = line_number;
        t->next->next = NULL;  
    }
} 
//Procura nó na tabela sintática
int st_lookup (char* name, char* scope) { 
    int h = hash(name, scope);	
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
    if (l == NULL) 
        return -1;
    else 
        return l->memloc;
}
//Procura tipo na tabela sintática
char* st_lookup_type (char* name, char* scope) { 
    int h = hash(name, scope);	
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
    if (l == NULL) 
        return "null";
    else 
        return l->typeData;
}
//Procura ID na tabela sintática
char* st_lookup_id (char* name, char* scope) { 
    int h = hash(name, scope);	
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(scope,l->scope) != 0))
        l = l->next;
    if (l == NULL) 
        return "null";
    else 
        return l->typeID;
}
//Printa tabela de símbolos
void printSymTab(FILE * listing){
    int i;
    fprintf(listing, "     Name           Scope           Type         Data Type        NumLine  \n");
    fprintf(listing, " ------------   -------------   ------------   ------------   -------------\n");
    for (i=0;i<SIZE;++i) {
        if (hashTable[i] != NULL) {
            BucketList l = hashTable[i];
            while (l != NULL) {
                LineList t = l->lines;
                fprintf(listing, "%-14s   ", l->name);
                fprintf(listing, "%-13s   ", l->scope);
                fprintf(listing, "%-12s   ", l->typeID);
                fprintf(listing, "%-12s   ", l->typeData);	
                while (t != NULL) {
                    fprintf(listing,"%-4d ",t->line_number);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}
