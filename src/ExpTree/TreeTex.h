#ifndef DEDFTR_TREE_TREE_TEX_H
#define DEDFTR_TREE_TREE_TEX_H
#include "Tree.h"
#include "TreeOptimizer.h"

#define TEX(format, ...) TEX_Printf(format, ## __VA_ARGS__)
#define nTab(n) for(int _ = 0; _ < n; ++_) TEX("\t");

void TEX_Start();

void TEX_Finish();

void TEX_Printf(const char* format, ...);

void TEX_Formula(ExprNode* node);

void TEX_Node(ExprNode* node, int depth);

const char* getOpTexStr(const Operator opr);

enum class TEX_PLACE{
    DiffStart,
    SingleDiffStart,
    FindVars,
    NoDiff,
    SummUp,
    DiffOpt,
};

void TEX_Phrase(TEX_PLACE place, ...);

void TEX_D(ExprNode* node, var_t var);

#endif