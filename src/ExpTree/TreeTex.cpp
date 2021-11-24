#include "TreeTex.h"
#include "Logger.h"
#include <stdarg.h>

#include "TexPhrases.h"


FILE* TEX_File = NULL;

int TO_TEX = 0;

void TEX_Start(){
    TEX_File = fopen("Diff.tex", "w");
    LOG_ASSERT(TEX_File != NULL);
    TO_TEX = 1;
    TEX(TEX_INIT);
    TEX(TEX_CHAPTER_1);
}

//------------------------------------------------------------------------------------------------

void TEX_Finish(){
    TEX(TEX_FINAL);
    fclose(TEX_File);
    TO_TEX = 0;
    system("pdflatex -halt-on-error Diff.tex");
}

//------------------------------------------------------------------------------------------------

void TEX_Pause(){
    TO_TEX = 0;
}

//------------------------------------------------------------------------------------------------

void TEX_Resume(){
    TO_TEX = 1;
}

//------------------------------------------------------------------------------------------------

void TEX_Printf(const char* format, ...){
    LOG_ASSERT(format != NULL);

    if(!TO_TEX) return;

    va_list args;
    va_start(args, format);
    vfprintf(TEX_File, format, args);
    va_end(args);
}

//------------------------------------------------------------------------------------------------

void TEX_Formula(const ExprNode* node, const char* str){
    LOG_ASSERT(node != NULL);

    size_t nVars = 0;
    var_t vars[MAX_VARS] = {};
    findVars(node, vars, &nVars);

    
    TEX("$$\n");
    if(str) TEX("%s = ", str);
    TEX_Node(node, 0);
    TEX("$$\n");
}

//------------------------------------------------------------------------------------------------

void TEX_Node(const ExprNode* node, int depth){
    LOG_ASSERT(node != NULL);
    nTab(depth);

    #ifdef USE_LABEL_SYSTEM
    const char* label = getLabelName(node);
    if(depth && label){
        TEX("{%s}\n", label);
        return;
    }
    #endif

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
                TEX("{");
                if(getSize(node->left) > 1){
                    TEX("\\left(\n");
                }
                TEX_Node(node->left, depth + 1);

                nTab(depth);
                if(getSize(node->left) > 1){
                    TEX("\\right)");
                }
                TEX("} %s {\n", node->value.opr.str);
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
            case Operator::SH:
            case Operator::CH:
            case Operator::SIN:
            case Operator::COS:
            case Operator::TAN:
            case Operator::COT:
            case Operator::LN:
            case Operator::CTH:
            case Operator::TH:
            case Operator::ASN:
            case Operator::ACN:
            case Operator::ATG:
            case Operator::ACT:
                LOG_ASSERT(node->right);
                TEX("%s\\left({\n", getOpTexStr(node->value.opr.opr));
                TEX_Node(node->right, depth + 1);
                nTab(depth);
                TEX("}\\right)\n");
                break;
            case Operator::DF:
                LOG_ASSERT(node->right);
                if(getSize(node->right) > 1){
                    TEX("\\left(\n");
                }
                TEX("{");
                TEX_Node(node->right,depth + 1);
                nTab(depth);
                TEX("}");
                if(getSize(node->right) > 1){
                    TEX("\\right)");
                }
                TEX("'\n");
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

//------------------------------------------------------------------------------------------------

void TEX_Phrase(TEX_PLACE place, ...){
    va_list ap;
    va_start(ap, place);
    

    switch (place)
    {
    case TEX_PLACE::SingleDiffStart:
        TEX("\\paragraph{");
        TEX(CHOOSE(TEX_DIF_VAR), (var_t)va_arg(ap, int));
        TEX("}\n");
        break;
    case TEX_PLACE::DiffStart:
        TEX(CHOOSE(TEX_DIFF_START));
        break;
    case TEX_PLACE::NoDiff:
        TEX("Здесь делать нечего:\n");
        break;
    case TEX_PLACE::SummUp:
        TEX("В итоге, после пары преобразований, легко получается\n");
        break;
    case TEX_PLACE::DiffOpt:
        TEX(CHOOSE(TEX_DIFF));
        break;
    case TEX_PLACE::MarkAs:
        TEX(CHOOSE(TEX_RENAME));
        TEX_Replace(va_arg(ap, ExprNode*));
    case TEX_PLACE::FindVars:
    default:
        break;
    }

    va_end(ap);
}

void TEX_D(const ExprNode* node, var_t var){
    LOG_ASSERT(node != NULL);

    if(node->type != ExprNodeType::OPERATOR) return;

    TEX_Phrase(TEX_PLACE::DiffOpt, node->value.opr.opr);
    TEX("$$\n");    
    TEX_RAW_D(node, var);
    TEX("$$\n");    
}

//------------------------------------------------------------------------------------------------

#define L nNode->left
#define R nNode->right
#define C(x) copyTree(x)
#define D(x) DF(C(x))

void TEX_RAW_D(const ExprNode* node, var_t var){
    LOG_ASSERT(node != NULL);

    if(node->type != ExprNodeType::OPERATOR) return;

    ExprNode* nNode = copyTree(node);
    ExprNode* oRoot = DF(nNode); 
    ExprNode* nRoot = NULL;

    #define OP_DEF(name, flags, strVal, diff, ...) case Operator::name: {nRoot = diff;} break;
    switch (nNode->value.opr.opr)
    {
    #include OPERATORS_H
    case Operator::NONE:
    default:
        LOG_ERROR("Wrong operator\n");
        break;
    }   

    supressTree(oRoot);
    supressTree(nRoot);

    TEX_Node(oRoot, 0);
    TEX(" = ");
    TEX_Node(nRoot, 0);

    deleteNode(oRoot);
    deleteNode(nRoot);
}
#undef OP_DEF
//------------------------------------------------------------------------------------------------

void TEX_Replace(const ExprNode* node){
    LOG_ASSERT(node != NULL);

    TEX("\\begin{equation} %s = ", getLabelName(node));
    TEX_Node(node, 0);
    TEX("\\end{equation}\n");
}

//------------------------------------------------------------------------------------------------

const char* TEX_GetVarName(){
    static int i = 0;
    return TEX_VARS[i++];
}

//------------------------------------------------------------------------------------------------

void TEX_Part(int n){
    TEX(TEX_PARTS[n]);
}

//------------------------------------------------------------------------------------------------


#define OP_DEF(name, flags, strVal, diff, ...)              \
    if(Operator::name != Operator::DF){                     \
        if(HAS_FLAG(flags, OperatorFlags::OF_BothArgs))     \
             node = name(VAR('f'), VAR('g'));               \
        else node = name(VAR('f'));                         \
        TEX("\\begin{equation}");                           \
        TEX_RAW_D(node, 'g');                               \
        TEX("\\end{equation}");                             \
        deleteNode(node);                                   \
        node = NULL;                                        \
    }                                                       \


void TEX_DLib(){
    ExprNode* node = NULL;
    #include OPERATORS_H
}
#undef OP_DEF
