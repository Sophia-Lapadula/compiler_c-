#ifndef AUX_PARSER_H
#define AUX_PARSER_H

//Escopop das funções auxiliares
void aggScope(TreeNode *t, char *scope);
TreeNode *newStmtNode(StatementKind kind);
TreeNode *newExpNode(ExpressionIdentifier kind);
char *copyString(char *s);
void printTree(TreeNode* tree);

#endif /* AUX_PARSER_H */