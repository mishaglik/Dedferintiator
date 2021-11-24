#include "Differntiator.h"
#include "Logger.h"

#define L node->left
#define R node->right

#define C(x) copyTree(x)
#define D(x) diffentiate(x, var, silent)


ExprNode* diffentiate(const ExprNode* node, var_t var, int silent){
    LOG_ASSERT(node != NULL);

    ExprNode* result = NULL;
    
    
    switch (node->type)
    {
    case ExprNodeType::NUMBER:   return CONST(0);
    case ExprNodeType::VARIABLE: return CONST(node->value.var == var);
    case ExprNodeType::OPERATOR:
        {
            if(!isVariable(node, var)){
                return CONST(0);
            }

            #define OP_DEF(name, flags, strVal, diff, ...) case Operator::name: result = diff; break;
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
        break;
    case ExprNodeType::NONE:
    default:
        LOG_ERROR("Incorrect node [%p]\n", node);
        return NULL;
    }

    static int skip = 0;
    if(silent == 0){
        treeOptimize(result);
        buildTreeLabeling(result);
        if(skip) {
            skip--;
            return result;
        }
        if(rand() % 20 == 0) {
            skip = rand() % 10;
            TEX("Этот кусок мы оставляем читателю в качестве упражнения.\n");
        }
        TEX_D(node, var);
        TEX_Phrase(TEX_PLACE::DiffOpt, node->value.opr.opr);
        ExprNode* tmp = DF(copyTree(node));
        TEX("$$\n");
        TEX_Node(tmp, 0);
        TEX(" = ");
        TEX_Node(result, 0);
        TEX("$$\n");
        deleteNode(tmp);
    }
    return result;
}

#undef C
#undef D
#undef L
#undef R


ExprNode* fullDifferntial(const ExprNode* node){
    LOG_ASSERT(node != NULL);

    free(graphTree(node));

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
    ExprNode* diff =  POW(curNode, DIV(CONST(1), CONST(2)));
    free(graphTree(diff));
    treeOptimize(diff);
    buildTreeLabeling(diff);
    free(graphTree(diff));

    return diff;
}

ExprNode* taylorSeries(const ExprNode* node, var_t var, num_t value, int degree){
    LOG_ASSERT(node != NULL);
    ExprNode* ans = CONST(0);
    ExprNode* cur = copyTree(node);

    for(int i = 0; i < degree; ++i){
        ExprNode* nxt = diffentiate(cur, var, 1);

        ExprNode* koef = cur;
        setVar(koef, var, value);
        koef = DIV(MUL(koef, POW(SUB(VAR(var), CONST(value)), CONST(i))), CONST(factorial(i)));
        ans = ADD(ans, koef);

        cur = nxt;
    }
    deleteNode(cur);
    treeOptimize(ans);
    return ans;
}

num_t factorial(num_t n){
    num_t ans = 1;
    for(num_t i = 1; i <= n; ++i){
        ans *= i;
    }
    return ans;
}