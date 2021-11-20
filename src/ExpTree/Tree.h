#ifndef DEDFTR_TREE_TREE_H
#define DEDFTR_TREE_TREE_H
#include <stdlib.h>
#include <stdio.h>
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

    SIN  = 0x6E6973, // 'nis' <=> sin
    COS  = 0x736F63, // 'soc' <=> cos
    TAN  = 0x006774, // 'gt'  <=> tg
    COT  = 0x677463, // 'gtc' <=> ctg

    ABS  = 0x736261, // 'sba' <=> abs

};

typedef int      num_t;
typedef char     var_t;

union opr_t
{
    Operator opr;
    char     str[sizeof(int)];
};

opr_t getOperator(const char* string);

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

ExprNode* growTree(const char* str, size_t *nRead = NULL);

int parceNodeData(const char* str, ExprNode* node);

struct SearchData{
    ExprNode* node = NULL;
    void* extra = NULL;
};

typedef void (*search_action_f)(SearchData* data);

void treeSearch(ExprNode* node, search_action_f func);

void writeTree(const ExprNode* tree);

char* graphTree(ExprNode* tree);

void skipSpaces(const char** str);
#endif