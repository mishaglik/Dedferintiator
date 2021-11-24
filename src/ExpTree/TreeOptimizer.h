#ifndef DEDFTR_TREE_TREE_OPTIMIZER_H
#define DEDFTR_TREE_TREE_OPTIMIZER_H
#include "Node.h"

int isNodeZero(const ExprNode* node);

int isNodeOne(const ExprNode* node);

int isOperator(const ExprNode* node, Operator opr);

void nodeCpy(ExprNode* dest,const ExprNode* src);

void nodeCutL(ExprNode* node);

void nodeCutR(ExprNode* node);

void treeOptimize(ExprNode* root);

int zeroOneArgOpt(ExprNode* node);

int constExprOpt(ExprNode* node);

int doubleDivOpt(ExprNode* node);
#endif