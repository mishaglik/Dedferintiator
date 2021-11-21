#ifndef DEDFTR_TREE_TREE_H
#define DEDFTR_TREE_TREE_H
#include <stdlib.h>
#include <stdio.h>

#ifndef OPERATORS_H
#define OPERATORS_H "Operators.h"
#endif

enum class ExprNodeType{
    NONE     = 0,
    NUMBER   = 1,
    VARIABLE = 2,
    OPERATOR = 3,
};

#define OP_DEF(name, flags, strVal, ...) name = strVal,

enum class Operator{
    NONE = 0,
    #include OPERATORS_H
};

#undef OP_DEF
// 0x00000
enum class OperatorFlags{
    OF_BothArgs = 0b100,
    OF_LZero    = 0b010,
    OF_RZero    = 0b001,

};

#define HAS_FLAG(flagset, flag) ((flagset) & int(flag))

int getOperatorFlags(Operator opr);

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

#define CONST(x)  newNode(ExprNodeType::NUMBER  , {.num = x}, NULL, NULL)
#define VAR(x)    newNode(ExprNodeType::VARIABLE, {.var = x}, NULL, NULL)

#define OP_DEF(name, ...) ExprNode* name(ExprNode* x, ExprNode* y = NULL);
#include OPERATORS_H
#undef OP_DEF 
#endif