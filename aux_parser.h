#ifndef AUX_PARSER_H
#define AUX_PARSER_H


// Function prototypes from aux_parser.c
void aggScope(TreeNode *t, char *scope);

char *copyString(char *s);
void printTree(TreeNode *tree);
void printAbstractTree(TreeNode *tree);
void freeTree(TreeNode *tree);

#endif /* AUX_PARSER_H */