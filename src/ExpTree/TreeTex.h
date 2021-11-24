#ifndef DEDFTR_TREE_TREE_TEX_H
#define DEDFTR_TREE_TREE_TEX_H
#include "Node.h"
#include "TreeOptimizer.h"

#define TEX(format, ...) TEX_Printf(format, ## __VA_ARGS__)
#define nTab(n) for(int _ = 0; _ < n; ++_) TEX("\t");

void TEX_Start();

void TEX_Pause();

void TEX_Resume();

void TEX_Finish();

void TEX_Printf(const char* format, ...);

void TEX_Formula(const ExprNode* node, const char* str = NULL);

void TEX_Node(const ExprNode* node, int depth);

const char* getOpTexStr(const Operator opr);

enum class TEX_PLACE{
    DiffStart,
    SingleDiffStart,
    FindVars,
    NoDiff,
    SummUp,
    DiffOpt,
    MarkAs,
};

void TEX_Phrase(TEX_PLACE place, ...);

void TEX_D(const ExprNode* node, var_t var);

void TEX_RAW_D(const ExprNode* node, var_t var);

void TEX_Replace(const ExprNode* node);

void TEX_Part(int n);

void TEX_DLib();

const char* TEX_GetVarName();

#endif
