#ifndef DEDFTR_TREE_TREE_TEX_H
#define DEDFTR_TREE_TREE_TEX_H
#include "Tree.h"
#include "TreeOptimizer.h"

void TEX_Start();

void TEX_Finish();

void TEX_Printf(const char* format, ...);

void TEX_Formula(ExprNode* node);

void TEX_Node(ExprNode* node, int depth);

const char* getOpTexStr(const Operator opr);


#endif