#include "Differntiator.h"
#include "Logger.h"


#define C(x) copyTree(x)
#define D(x) diffentiate(x, var)

#define L node->left
#define R node->right

ExprNode* diffentiate(ExprNode* node, var_t var){
    LOG_ASSERT(node != NULL);

    switch (node->type)
    {
    case ExprNodeType::NUMBER:   return CONST(0);
    case ExprNodeType::VARIABLE: return CONST(node->value.var == var);
    case ExprNodeType::OPERATOR:
        {
            if(!isVariable(node, var)){
                return CONST(0);
            }

            #define OP_DEF(name, flags, strVal, diff, ...) case Operator::name: return diff;
            switch (node->value.opr.opr)
            {
            #include OPERATORS_H
            case Operator::NONE:
            default:
                LOG_ERROR("Inappropriate operator");
                return NULL;
            }
            #undef OP_DEF
        }
    case ExprNodeType::NONE:
    default:
        LOG_ERROR("Incorrect node [%p]\n", node);
        return NULL;
    }
}

#undef C
#undef D
#undef L
#undef R


ExprNode* fullDifferntial(ExprNode* node){
    LOG_ASSERT(node != NULL);

    TEX_Formula(node);
    TEX_Phrase(TEX_PLACE::DiffStart);

    var_t varList[MAX_VARS] = {};
    size_t nVars = 0;

    findVars(node, varList, &nVars);

    TEX_Phrase(TEX_PLACE::FindVars, varList, nVars);
    
    if(nVars == 0){
        TEX_Phrase(TEX_PLACE::NoDiff);
        return CONST(0);
    }
    if(nVars == 1){
        TEX_Phrase(TEX_PLACE::SingleDiffStart, varList[0]);
        return diffentiate(node, varList[0]);
    }

    TEX_Phrase(TEX_PLACE::SingleDiffStart, varList[0]);
    ExprNode* firstDiff = diffentiate(node, varList[0]);
    ExprNode* curNode = ADD(POW(firstDiff , CONST(2)), POW(diffentiate(node, varList[1]), CONST(2)));

    for(size_t i = 2; i < nVars; ++i){
        TEX_Phrase(TEX_PLACE::SingleDiffStart, varList[i]);
        curNode = ADD(curNode, POW(diffentiate(node, varList[i]), CONST(2)));
    }
    TEX_Phrase(TEX_PLACE::SummUp);
    return POW(curNode, DIV(CONST(1), CONST(2)));
}

