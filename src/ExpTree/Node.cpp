#include <string.h>
#include "Node.h"
#include "Logger.h"
#include "utils.h"
#include <ctype.h>


//--------------------------------------------------------------------------------------------------------------------

ExprNode* newNode(ExprNodeType type, ExprNodeValue value, ExprNode* left, ExprNode* right){
    
    ExprNode* node = (ExprNode*)mgk_calloc(1, sizeof(ExprNode));

    node->type  = type;
    node->value = value;
    node->left  = left;
    node->right = right;

    return node;
}

//--------------------------------------------------------------------------------------------------------------------

void deleteNode(ExprNode* node){
    LOG_ASSERT(node != NULL);

    if(node->left){
        deleteNode(node->left);
        node->left = NULL;
    }
    if(node->right){
        deleteNode(node->right);
        node->right = NULL;
    }
    node->type = ExprNodeType::NONE;
    node->value.num = 0;
    free(node);
}

//--------------------------------------------------------------------------------------------------------------------

void treeSearch(ExprNode* node, TreeSearchType type, search_action_f func, void* extra){
    LOG_ASSERT(node != NULL);
    LOG_ASSERT(func != NULL);

    TreeSearchData data = {node, type, extra};

    if((unsigned)type & (unsigned)TreeSearchType::PREFIX){
        data.type = TreeSearchType::PREFIX;
        func(&data);
    }

    if(node->left){
        treeSearch(node->left, type, func, extra);
    }

    if((unsigned)type & (unsigned)TreeSearchType::INFIX){
        data.type = TreeSearchType::INFIX;
        func(&data);
    }

    if(node->right){
        treeSearch(node->right, type, func, extra);
    }

    if((unsigned)type & (unsigned)TreeSearchType::SUFFIX){
        data.type = TreeSearchType::SUFFIX;
        func(&data);
    }
}

//--------------------------------------------------------------------------------------------------------------------


int isVariable(ExprNode* node, var_t var){
    LOG_ASSERT(node != NULL);

    IsVarInfo info = {0, var};
    treeSearch(node, TreeSearchType::INFIX, &isNodeVar, &info);

    return info.isVar;
}

//--------------------------------------------------------------------------------------------------------------------

void isNodeVar(TreeSearchData* data){
    LOG_ASSERT(data != NULL);

    IsVarInfo* info = (IsVarInfo*)data->extra;

    info->isVar |= (data->node->type == ExprNodeType::VARIABLE) && (data->node->value.var == info->var);
}

//--------------------------------------------------------------------------------------------------------------------

ExprNode* copyTree(ExprNode* node){
    LOG_ASSERT(node != NULL);

    return newNode(node->type, node->value, (node->left  ? copyTree(node->left)  : NULL),
                                            (node->right ? copyTree(node->right) : NULL));
}

//--------------------------------------------------------------------------------------------------------------------


#define OP_DEF(name, flags, ...)                                                \
ExprNode* name(ExprNode* x, ExprNode* y){                                       \
    if(HAS_FLAG(flags, OperatorFlags::OF_BothArgs)){                            \
        return newNode(ExprNodeType::OPERATOR, {.opr = Operator::name}, x, y);  \
    }                                                                           \
    return newNode(ExprNodeType::OPERATOR, {.opr = Operator::name}, NULL, x);   \
}

#include OPERATORS_H
#undef OP_DEF 

//--------------------------------------------------------------------------------------------------------------------

int getOperatorFlags(Operator opr){
    #define OP_DEF(name, flags, ...) case Operator::name: return flags;
    switch (opr)
    {
    #include OPERATORS_H
    case Operator::NONE:
        return 0;
    default:
        LOG_ERROR("Unkwnow operator");
        return 0;
        break;
    }
    #undef OP_DEF
}

//--------------------------------------------------------------------------------------------------------------------

void findVars(ExprNode* node, var_t* varList, size_t* nVars){
    LOG_ASSERT(node    != NULL);
    LOG_ASSERT(varList != NULL);
    LOG_ASSERT(nVars   != NULL);


    if(node->left)
        findVars(node->left, varList, nVars);
    if(node->right)
        findVars(node->right, varList, nVars);
    
    if(node->type == ExprNodeType::VARIABLE){
        for(size_t i = 0; i < *nVars; ++i){
            if(varList[i] == node->value.var)
                return;
        }
        if(*nVars == MAX_VARS){
            LOG_WARNING("Too many variables\n");
            return;
        }
        varList[(*nVars)++] = node->value.var;
    }
}

//--------------------------------------------------------------------------------------------------------------------

int isTreeEq(ExprNode* tree1, ExprNode* tree2){
    LOG_ASSERT(tree1 != NULL);
    LOG_ASSERT(tree2 != NULL);

    if(tree1 == tree2) return 1;

    if (tree1->type != tree2->type) return 0;

    switch (tree1->type)
    {
    case ExprNodeType::NONE:
        return 1;
    case ExprNodeType::NUMBER:
        if(tree1->value.num != tree2->value.num) return 0;
        break;
    case ExprNodeType::VARIABLE:
        if(tree1->value.var != tree2->value.num) return 0;
        break;
    case ExprNodeType::OPERATOR:
        if(tree1->value.opr.opr != tree2->value.opr.opr) return 0;
        if(tree1->left || tree2->left){
            LOG_ASSERT(tree1->left);
            LOG_ASSERT(tree2->left);
            if(!isTreeEq(tree1->left, tree2->left)) return 0;
        }
        if(tree2->right || tree2->right){
            LOG_ASSERT(tree1->right);
            LOG_ASSERT(tree2->right);
            if(!isTreeEq(tree1->right, tree2->right)) return 0;
        }
    break;
    default:
        LOG_ERROR("Unkown tree type\n");
        return -1;
    }
    return 1;
}