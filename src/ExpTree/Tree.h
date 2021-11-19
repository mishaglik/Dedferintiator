#ifndef DEDFTR_TREE_TREE_H
#define DEDFTR_TREE_TREE_H
#include <stdlib.h>
#include "NodeOps.h"

enum class ExprNodeType{
    NONE     = 0,
    NUMBER   = 1,
    VARIABLE = 2,
    OPERATOR = 3,
};

enum class Operator{
    NONE = 0,
    ADD  = '+',
    SUB  = '-',
    MUL  = '*',
    DIV  = '/',

    SIN  = 'nis', //sin
    COS  = 'soc', //cos
    TAN  = 'gt' , //tg
    COT  = 'gtc', //ctg

    ABS  = 'sba', //abs

};

Operator getOperator(const char* string);

const char* getOperatorStr(Operator opt);

typedef int      num_t;
typedef char     var_t;
typedef Operator opr_t;

union ExprNodeValue
{
    num_t num;
    var_t var;
    opr_t opr;
};

struct ExprNode
{
    ExprNodeType type = ExprNodeType::NONE;
    ExprNodeValue value = {};

    ExprNode* left  = NULL;
    ExprNode* right = NULL;
};

ExprNode* newNode(ExprNodeType type, ExprNodeValue value, ExprNode* left, ExprNode* right);

void deleteNode(ExprNode* node);

struct ExprTree
{
    ExprNode* root = NULL;
};

#endif