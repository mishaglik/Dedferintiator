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
    POW  = '^',

    SIN  = 0x6E6973, // 'nis' <=> sin
    COS  = 0x736F63, // 'soc' <=> cos
    TAN  = 0x006774, // 'gt'  <=> tg
    COT  = 0x677463, // 'gtc' <=> ctg

    ABS  = 0x736261, // 'sba' <=> abs

    LN   = 0x006E6C, // 'nl'  <=> ln

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

enum class TreeSearchType: unsigned{
    NONE   = 0x0,
    PREFIX = 0x1,
    INFIX  = 0x2,
    SUFFIX = 0x4,

    _PI  = 0x3,
    _PS  = 0x5,
    _IS  = 0x6,
    _PIS = 0x7,
};

struct TreeSearchData{
    ExprNode* node = NULL;
    TreeSearchType type = TreeSearchType::NONE;
    void* extra = NULL;
};

typedef void (*search_action_f)(TreeSearchData* data);

void treeSearch(ExprNode* node, TreeSearchType type, search_action_f func, void* extra);

void writeTree(ExprNode* tree, FILE* file = stdout);

void writeNodeData(TreeSearchData* data);

char* graphTree(ExprNode* tree);

void writeGraphData(TreeSearchData* data);

void fwriteNodeStr(ExprNode* node, FILE* file);

void skipSpaces(const char** str);

struct IsVarInfo{
    int isVar = 0;
    var_t var = 0;
};

void isNodeVar(TreeSearchData* data);

int isVariable(ExprNode* node, var_t var);

ExprNode* copyTree(ExprNode* tree);
#endif