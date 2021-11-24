#ifndef EXPRTREE_TREE_IO_H
#define EXPRTREE_TREE_IO_H
#include "Node.h"

void writeTree(const ExprNode* tree, FILE* file = stdout);

void writeNodeData(TreeSearchData* data);

char* graphTree(const ExprNode* tree);

void writeGraphData(TreeSearchData* data);

void fwriteNodeStr(const ExprNode* node, FILE* file);

void skipSpaces(const char** str);

ExprNode* growTree(const char* str, size_t *nRead = NULL);

int parceNodeData(const char* str, ExprNode* node);

opr_t getOperator(const char* string);

#endif