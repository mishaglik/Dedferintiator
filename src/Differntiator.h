#ifndef DEDFTR_DIFFERNTIATOR_H
#define DEDFTR_DIFFERNTIATOR_H
#include "ExpTree/Tree.h"

#undef OPERATORS_H
#define OPERATORS_H "ExpTree/Operators.h"

ExprNode* diffentiate(ExprNode* node, var_t var);

ExprNode* fullDifferntial(ExprNode* node);

void findVars(ExprNode* node, var_t* varList, size_t* nVars);
#endif