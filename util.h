#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"
TreeNode *newStmtNode(StatementKind kind);
TreeNode *newExpNode(ExpressionIdentifier kind);
#endif