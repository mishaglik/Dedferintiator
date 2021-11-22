#include "TreeOptimizer.h"
#include "Logger.h"
#include "string.h"
#include "math.h"

int isNodeZero(ExprNode* node){
    LOG_ASSERT(node != NULL);
    return node->type == ExprNodeType::NUMBER && node->value.num == 0;
}

int isNodeOne(ExprNode* node){
    LOG_ASSERT(node != NULL);
    return node->type == ExprNodeType::NUMBER && node->value.num == 1;
}

int isOperator(ExprNode* node, Operator opr){
    LOG_ASSERT(node != NULL);
    return node->type == ExprNodeType::OPERATOR && node->value.opr.opr == opr;
}

void nodeCpy(ExprNode* dest, ExprNode* src){
    LOG_ASSERT(dest != NULL);
    LOG_ASSERT(src  != NULL);

    memcpy(dest, src, sizeof(ExprNode));
}

void nodeCutL(ExprNode* node){
    LOG_ASSERT(node != NULL);

    ExprNode* right = node->right;

    if(node->left){
        deleteNode(node->left);
        node->left = NULL;
    }

    LOG_ASSERT(right);
    nodeCpy(node, right);
    free(right);
}

void nodeCutR(ExprNode* node){
    LOG_ASSERT(node != NULL);

    ExprNode* left = node->left;
    LOG_ASSERT(left);
    if(node->right){
        deleteNode(node->right);
        node->right = NULL;
    }

    nodeCpy(node, left);
    free(left);
}

void treeOptimize(ExprNode* root){
    LOG_INFO("Begin tree optimiztion\n");
    int nOptimized = 1;
    
    #define OPTIMIZE(func)                                  \
    {                                                       \
        LOG_INFO("\tBegin optmization: %s()\n", #func);     \
        int nOpt = func(root);                              \
        LOG_INFO("\tFinished with n = %d\n\n", nOpt);       \
        if(nOpt < 0) {LOG_ERROR("Opt error\n"); return; }   \
        nOptimized += nOpt;                                 \
                                                            \
    }

    while (nOptimized > 0)
    {
        nOptimized = 0;
        OPTIMIZE(zeroOneArgOpt);
        OPTIMIZE(constExprOpt);
        OPTIMIZE(doubleDivOpt);
    }
    LOG_INFO("Tree optimization finished\n");
    
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

int zeroOneArgOpt(ExprNode* node){
    LOG_ASSERT(node != NULL);

    int nOpt = 0;

    if(node->left){
        nOpt += zeroOneArgOpt(node->left);
    }

    if(node->right){
        nOpt += zeroOneArgOpt(node->right);
    }


    if(node->type == ExprNodeType::OPERATOR){
        if(node->left && isNodeZero(node->left)){
            LOG_DEBUG("Found left 0. Node[%p]\n", node);

            if(HAS_FLAG(getOperatorFlags(node->value.opr.opr), OperatorFlags::OF_LZero)){
                nodeCutR(node);
                return nOpt + 1;
            }

            switch(node->value.opr.opr){
                case Operator::ADD:
                    nodeCutL(node);
                    return nOpt + 1;

                case Operator::SUB:
                    node->left->value.num = -1;
                    node->value.opr.opr = Operator::MUL;
                    return nOpt + 1;
                default:
                    break;
            }
        }

        if(node->left && isNodeOne(node->left)){
            LOG_DEBUG("Found left 1. Node[%p]\n", node);

            switch(node->value.opr.opr){
                case Operator::MUL:
                    nodeCutL(node);
                    return nOpt + 1;

                case Operator::POW:
                    nodeCutR(node);
                    return nOpt + 1;
                default:
                    break;
            }
        }

        if(node->right && isNodeZero(node->right)){
            LOG_DEBUG("Found right 0. Node[%p]\n", node);
            // LOG_DEBUG("Type: %d Operator: %s\n", node->type, node->value.opr.opr);

            if(HAS_FLAG(getOperatorFlags(node->value.opr.opr), OperatorFlags::OF_RZero)){
                nodeCutL(node);
                return nOpt + 1;
            }

            switch(node->value.opr.opr){
                case Operator::ADD:
                case Operator::SUB:
                    nodeCutR(node);
                    return nOpt + 1;

                case Operator::COS:
                case Operator::POW:
                    nodeCutL(node);
                    node->value.num = 1;
                    return nOpt + 1;

                case Operator::LN:
                case Operator::DIV:
                case Operator::COT:
                    LOG_WARNING("Found dividion/cot/ln with zero. It's abnormal\n");
                    return nOpt;
                default:
                    break;
            }
        }

        if(node->right && isNodeOne(node->right)){
            LOG_DEBUG("Found right 1. Node[%p]\n", node);

            switch(node->value.opr.opr){
                case Operator::MUL:
                case Operator::DIV:
                case Operator::POW:
                    nodeCutR(node);
                    return nOpt + 1;

                case Operator::LN:
                    nodeCutL(node);
                    node->value.num = 0;
                    return nOpt + 1;
                default:
                    break;
            }
        }
    }

    return nOpt;
}



int constExprOpt(ExprNode* node){
    LOG_ASSERT(node != NULL);

    int nOpt = 0;

    if(node->left){
        nOpt += constExprOpt(node->left);
    }

    if(node->right){
        nOpt += constExprOpt(node->right);
    }

    if(node->type == ExprNodeType::OPERATOR && node->left && node->right && node->left->type == ExprNodeType::NUMBER && node->right->type == ExprNodeType::NUMBER){
        switch (node->value.opr.opr)
        {
        case Operator::ADD:
            node->left->value.num += node->right->value.num;
            break;
        case Operator::SUB:
            node->left->value.num -= node->right->value.num;
            break;
        case Operator::MUL:
            node->left->value.num *= node->right->value.num;
            break;
        case Operator::DIV:
            if(!node->right->value.num || node->left->value.num % node->right->value.num)
                return nOpt;
            node->left->value.num /= node->right->value.num;
            break;
        case Operator::POW:
            if(node->right->value.num < 0) return nOpt;
            node->left->value.num = (int)pow(node->left->value.num, node->right->value.num);
            break;
        default:
            return nOpt;
        }

        nOpt++;
        nodeCutR(node);
    }

    return nOpt;
}

int doubleDivOpt(ExprNode* node){
    LOG_ASSERT(node != NULL);

    int nOpt = 0;

    if(node->left){
        nOpt += doubleDivOpt(node->left);
    }
    if(node->right){
        nOpt += doubleDivOpt(node->right);
    }

    if(isOperator(node, Operator::DIV)){
        LOG_ASSERT(node->left);
        LOG_ASSERT(node->right);

        if(isOperator(node->left, Operator::DIV)){
            ExprNode* left = node->left;

            node->left = left->left;
            left->left = NULL;

            node->right = MUL(node->right, left->right);
            left->right = NULL;

            deleteNode(left);
            return nOpt + 1;
        }

        if(isOperator(node->right, Operator::DIV)){
            ExprNode* right = node->right;

            node->right = right->left;
            right->left = NULL;

            node->right = MUL(node->right, right->right);
            right->right = NULL;

            deleteNode(right);
            return nOpt + 1;
        }
    }

    if(isOperator(node, Operator::MUL)){
        LOG_ASSERT(node->left);
        LOG_ASSERT(node->right);

        if(isOperator(node->left, Operator::DIV)){
            ExprNode* right = node->right;
            node->right = NULL;

            nodeCutR(node);
            node->left = MUL(node->left, right);
            return nOpt + 1;
        }

        if(isOperator(node->right, Operator::DIV)){
            ExprNode* left = node->left;
            node->left = NULL;
            
            nodeCutL(node);
            
            node->left = MUL(left,node->left);
            return nOpt + 1;
        }
    }

    if(isOperator(node, Operator::POW)){
        LOG_ASSERT(node->left);
        LOG_ASSERT(node->right);

        if(isOperator(node->left, Operator::POW)){
            ExprNode* left = node->left;

            node->left = left->left;
            left->left = NULL;

            node->right = MUL(left->right, node->right);
            left->right = NULL;

            deleteNode(left);
            return nOpt + 1;
        }
    }
    return nOpt;
}
#pragma GCC diagnostic pop