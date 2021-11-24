#ifndef DEDFTR_TREE_TREE_H
#define DEDFTR_TREE_TREE_H
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

const size_t MAX_VARS = 255;

enum class ExprNodeType{
    NONE     = 0,
    NUMBER   = 1,
    VARIABLE = 2,
    OPERATOR = 3,
};

//######################## Operator ##########################################################

#ifndef OPERATORS_H
#define OPERATORS_H "Operators.h"
#endif


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

//######################## Expr Node ##########################################################

typedef int      num_t;
typedef char     var_t;
union opr_t
{
    Operator opr;
    char     str[sizeof(int)];
};

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


//######################## Tree Search ##########################################################

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
    const ExprNode* node = NULL;
    TreeSearchType type = TreeSearchType::NONE;
    void* extra = NULL;
};

typedef void (*search_action_f)(TreeSearchData* data);

void treeSearch(const ExprNode* node, TreeSearchType type, search_action_f func, void* extra);

//########################## Utility function ############################################

void findVars(const ExprNode* node, var_t* varList, size_t* nVars);

struct IsVarInfo{
    int isVar = 0;
    var_t var = 0;
};

void isNodeVar(TreeSearchData* data);

int isVariable(const ExprNode* node, var_t var = 'x');

ExprNode* copyTree(const ExprNode* tree);

int isTreeEq(const ExprNode* tree1, const ExprNode* tree2);

void setVar(ExprNode* tree, var_t var, num_t value);

//############################# newNode wrappers ##################################

#define CONST(x)  newNode(ExprNodeType::NUMBER  , {.num = x}, NULL, NULL)
#define VAR(x)    newNode(ExprNodeType::VARIABLE, {.var = x}, NULL, NULL)

#define OP_DEF(name, ...) ExprNode* name(ExprNode* x, ExprNode* y = NULL);
#include OPERATORS_H
#undef OP_DEF 

#ifdef USE_LABEL_SYSTEM
#include "TreeLabeler.h"
#endif

#endif