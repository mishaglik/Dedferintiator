#include "TreeTex.h"
#include "Logger.h"
#include <stdarg.h>

#include "TexPhrases.h"
#define TEX(format, ...) TEX_Printf(format, ## __VA_ARGS__)
#define nTab(n) for(int _ = 0; _ < n; ++_) TEX("\t");

FILE* TEX_File = NULL;

void TEX_Start(){
    TEX_File = fopen("Diff.tex", "w");
    LOG_ASSERT(TEX_File != NULL);
    TEX(TEX_INIT);
}

void TEX_Finish(){

    TEX(TEX_FINAL);
    fclose(TEX_File);
    system("pdflatex Diff.tex");
}

void TEX_Printf(const char* format, ...){
    LOG_ASSERT(format != NULL);

    va_list args;
    va_start(args, format);
    vfprintf(TEX_File, format, args);
    va_end(args);
}

void TEX_Formula(ExprNode* node){
    LOG_ASSERT(node != NULL);
    TEX("$$\n");
    TEX_Node(node, 0);
    TEX("$$\n");
}

void TEX_Node(ExprNode* node, int depth){
    LOG_ASSERT(node != NULL);

    nTab(depth);

    switch (node->type)
    {
    case ExprNodeType::NUMBER:
        TEX("{%d}\n", node->value.num);
        break;
    case ExprNodeType::VARIABLE:
        TEX("{%c}\n", node->value.var);
        break;
    case ExprNodeType::OPERATOR:
        {
            switch (node->value.opr.opr)
            {
            case Operator::ADD:
            case Operator::MUL:
            case Operator::SUB:
                LOG_ASSERT(node->left);
                LOG_ASSERT(node->right);

                TEX("{\n");

                if(isOperator(node, Operator::MUL) && (isOperator(node->left, Operator::ADD) || isOperator(node->left, Operator::SUB))) TEX("\\left(");
                TEX_Node(node->left, depth + 1);
                if(isOperator(node, Operator::MUL) && (isOperator(node->left, Operator::ADD) || isOperator(node->left, Operator::SUB))) TEX("\\right)");

                nTab(depth);
                TEX("} %s {\n", node->value.opr.str);

                if(isOperator(node, Operator::MUL) && (isOperator(node->right, Operator::ADD) || isOperator(node->right, Operator::SUB))) TEX("\\left(");
                TEX_Node(node->right, depth + 1);
                if(isOperator(node, Operator::MUL) && (isOperator(node->right, Operator::ADD) || isOperator(node->right, Operator::SUB))) TEX("\\right)");

                nTab(depth);
                TEX("}\n");
                break;
            case Operator::POW:
                LOG_ASSERT(node->left);
                LOG_ASSERT(node->right);

                if(isOperator(node->right, Operator::DIV) && isNodeOne(node->right->left) && node->right->right->type == ExprNodeType::NUMBER){
                    int nRoot = node->right->right->value.num;
                    TEX("\\sqrt");
                    if(nRoot != 2){
                        TEX("[%d]", nRoot);
                    }
                    TEX("{\n");
                    TEX_Node(node->left, depth + 1);
                    nTab(depth);
                    TEX("}\n");
                    break;
                }


                TEX("\\left({\n");
                TEX_Node(node->left, depth + 1);

                nTab(depth);
                TEX("}\\right) %s {\n", node->value.opr.str);
                TEX_Node(node->right, depth + 1);

                nTab(depth);
                TEX("}\n");
                break;

            case Operator::DIV:
                LOG_ASSERT(node->left);
                LOG_ASSERT(node->right);

                TEX("\\frac{\n");
                TEX_Node(node->left, depth + 1);

                nTab(depth);
                TEX("}{\n");
                TEX_Node(node->right, depth + 1);
                nTab(depth);
                TEX("}\n");
                break;

            case Operator::ABS:
                LOG_ASSERT(node->right);
                TEX("\\left|\n");
                TEX_Node(node->right, depth + 1);
                nTab(depth);
                TEX("\\right|\n");
                break;

            case Operator::SIN:
            case Operator::COS:
            case Operator::TAN:
            case Operator::COT:
            case Operator::LN:
                LOG_ASSERT(node->right);
                TEX("%s\\left({\n", getOpTexStr(node->value.opr.opr));
                TEX_Node(node->right, depth + 1);
                nTab(depth);
                TEX("}\\right)\n");
                break;
            case Operator::NONE:
            default:
                LOG_ERROR("Unknow tex or incorrect operator\n");
                break;
            }
        }
        break;
    case ExprNodeType::NONE:
    default:
        LOG_ERROR("Incorrent node type\n");
        return;
        break;
    }
    
}

#define OP_DEF(name, flags, strVal, diff, texStr, ...) case Operator::name: return texStr;
const char* getOpTexStr(const Operator opr){
    switch (opr)
    {
    #include OPERATORS_H
    case Operator::NONE:
    default:
        LOG_ERROR("Error operator.\n");
        return NULL;
    }
}
#undef OP_DEF