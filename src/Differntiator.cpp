#include "Differntiator.h"
#include "Logger.h"

//! Orignal naming C(x) - copy of x, D(x) - diff of x

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
            switch (node->value.opr.opr)
            {
            case Operator::ADD: return ADD(D(L), D(R));
            case Operator::SUB: return SUB(D(L), D(R));
            case Operator::MUL: return ADD(MUL(D(L), C(R)), MUL(C(L), D(R)));
            case Operator::DIV: return DIV(SUB(MUL(D(L), C(R)), MUL(C(L), D(R))), POW(C(R), CONST(2)));

            case Operator::SIN: return MUL(COS(C(R)), D(R));
            case Operator::COS: return MUL(MUL(CONST(-1), SIN(C(R))), D(R));
            case Operator::TAN: return DIV(               D(R) , POW(COS(C(R)), CONST(2)));
            case Operator::COT: return DIV(MUL(CONST(-1), D(R)), POW(SIN(C(R)), CONST(2)));

            case Operator::ABS: return MUL(DIV(ABS(C(R)), C(R)), D(R));
            case Operator::LN : return DIV(D(R), C(R));

            case Operator::POW:
            {
                if(!isVariable(R, var)){
                    return MUL(MUL(C(R), POW(C(L), SUB(C(R), CONST(1)))), D(L));
                }
                return MUL(C(node), ADD(MUL(D(R), LN(C(L))), MUL(DIV(D(L), C(L)), C(R))));
            }
            case Operator::NONE:
            default:
                LOG_ERROR("Inappropriate operator");
                return NULL;
            }
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