#ifndef DEDFTR_DIFFERNTIATOR_H
#define DEDFTR_DIFFERNTIATOR_H
#include "ExpTree/Tree.h"
#include "ExpTree/TreeTex.h"

#undef OPERATORS_H
#define OPERATORS_H "ExpTree/Operators.h"

ExprNode* diffentiate(const ExprNode* node, var_t var, int silent = 0);

ExprNode* fullDifferntial(const ExprNode* node);

ExprNode* taylorSeries(const ExprNode* node, var_t var, num_t value, int degree);

num_t factorial(num_t n);

#endif