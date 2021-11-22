#include <string.h>
#include "Tree.h"
#include "Logger.h"
#include "utils.h"
#include <ctype.h>

// #define AUTO_OPEN_PHOTO

const char* spaceSyms = " \t\n";

const size_t MAX_TOKEN_LEN     = 10;
const size_t GRAPH_FILENAME_SZ = 25;
const size_t COMMAND_SZ        = 50;

void skipSpaces(const char** str){
    LOG_ASSERT(str != NULL);
    LOG_ASSERT(*str != NULL);

    *str += strspn(*str, spaceSyms);
}

//--------------------------------------------------------------------------------------------------------------------

opr_t getOperator(const char* string){
    LOG_ASSERT(string != NULL);

    opr_t curOpr = {};
    curOpr.opr = Operator::NONE;

    if(strlen(string) > sizeof(opr_t)){
        return curOpr;
    }

    strncpy(curOpr.str, string, sizeof(opr_t));

    #define OP_DEF(name, ...) case Operator::name:
    
    switch (curOpr.opr)
    {
    case Operator::NONE:
    #include OPERATORS_H
        return curOpr;
    default:
        curOpr.opr = Operator::NONE;
        return curOpr;
        break;
    }
    #undef OP_DEF
}

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

ExprNode* growTree(const char* str, size_t *nRead){
    LOG_ASSERT(str != NULL);

    const char* curChr = str;
    ExprNode localNode = {}; 

    skipSpaces(&curChr);

    if(*curChr != '('){
        LOG_ERROR("Inapprorpitate Expression format: %10s, Expected '('\n"
                  "                                ~~^~~", curChr);
        return NULL;
    } 
    curChr++;
    skipSpaces(&curChr);

    if(*curChr == '('){
        size_t toSkip = 0;
        localNode.left = growTree(curChr, &toSkip);
        if(!localNode.left) return NULL;
        curChr += toSkip;
    }

    skipSpaces(&curChr);
    
    if(*curChr == '(' || *curChr == ')' || !isprint(*curChr)){
         LOG_ERROR("Inapprorpitate Expression format: %10s, Operator expected\n"
                  "                                ~~^~~", curChr);
        return NULL;
    }

    curChr += parceNodeData(curChr, &localNode);

    if(localNode.type == ExprNodeType::NONE) return NULL;

    skipSpaces(&curChr);

    if(*curChr == '('){
        size_t toSkip = 0;
        localNode.right = growTree(curChr, &toSkip);
        if(!localNode.right) return NULL;
        curChr += toSkip;
    }

    skipSpaces(&curChr);

    if(*curChr != ')'){
        LOG_ERROR("Inapprorpitate Expression format: %10s, ')' expected\n"
                  "                                ~~^~~", curChr);
        return NULL;
    }
    curChr++;

    ExprNode* node = (ExprNode*)mgk_calloc(1, sizeof(ExprNode));
    mempcpy(node, &localNode, sizeof(ExprNode));
    
    if(nRead != NULL){
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wsign-conversion"
        *nRead = curChr - str;
        #pragma GCC diagnostic pop
    }
    return node;
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

void writeTree(ExprNode* tree, FILE* file){
    LOG_ASSERT(tree != NULL);

    treeSearch(tree, TreeSearchType::_PIS, &writeNodeData, file);
    fprintf(file, "\n");
}

//--------------------------------------------------------------------------------------------------------------------

void writeNodeData(TreeSearchData* data){
    LOG_ASSERT(data != NULL);

    FILE* file = (FILE*)data->extra;

    if(data->type == TreeSearchType::PREFIX){
        fprintf(file, "(");
    }

    if(data->type == TreeSearchType::INFIX){
        fwriteNodeStr(data->node, file);
    }

    if(data->type == TreeSearchType::SUFFIX){
        fprintf(file, ")");
    }
}

//--------------------------------------------------------------------------------------------------------------------

void fwriteNodeStr(ExprNode* node, FILE* file){
    LOG_ASSERT(node != NULL);
    LOG_ASSERT(file != NULL);

    switch (node->type)
    {
    case ExprNodeType::NUMBER:
        fprintf(file, "%d", node->value.num);
        break;
    case ExprNodeType::VARIABLE:
        fprintf(file, "%c", node->value.var);
        break;
    case ExprNodeType::OPERATOR:
        fprintf(file, "%s", node->value.opr.str);
        break;
    case ExprNodeType::NONE:
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------------------------------------

char* graphTree(ExprNode* tree){
    LOG_ASSERT(tree != NULL);

    static int nDump = 0;

    FILE* dotFile = fopen("log/graph.dot", "w");
    LOG_ASSERT(dotFile != NULL);

    fprintf(dotFile, "digraph G{");

    treeSearch(tree, TreeSearchType::_PS, &writeGraphData, dotFile);

    fprintf(dotFile, "}");
    fclose(dotFile);

    char* imgFileName = (char*)mgk_calloc(GRAPH_FILENAME_SZ, sizeof(char));

    sprintf(imgFileName, "log/GRAPH_DUMP_%d.png", nDump++);
    
    char command[COMMAND_SZ] = "";

    sprintf(command, "dot log/graph.dot -T png -o %s", imgFileName);
    LOG_INFO("Executing command: '%s'\n", command);
    system(command);
    #ifdef AUTO_OPEN_PHOTO
    sprintf(command, "eog %s &", imgFileName);
    LOG_INFO("Executing command: '%s'\n", command);
    system(command);
    #endif
    return imgFileName;
}


//--------------------------------------------------------------------------------------------------------------------

void writeGraphData(TreeSearchData* data){
    LOG_ASSERT(data != NULL);

    FILE* file = (FILE*)data->extra;

    if(data->type == TreeSearchType::PREFIX){
        fprintf(file, "N%p[label=\"", data->node);
        fwriteNodeStr(data->node, file);
    #ifdef DEBUG_GRAPH
        fprintf(file, "[%p]", data->node);
    #endif
        fprintf(file, "\"];\n");
    }

    if(data->type == TreeSearchType::SUFFIX){
        if(data->node->left)
            fprintf(file, "N%p->N%p[label=\"left \"];\n", data->node, data->node->left);

        if(data->node->right)
            fprintf(file, "N%p->N%p[label=\"right\"];\n", data->node, data->node->right);
    }
}

//--------------------------------------------------------------------------------------------------------------------

int parceNodeData(const char* str, ExprNode* node){
    LOG_ASSERT(str  != NULL);
    LOG_ASSERT(node != NULL);

    LOG_INFO("Getting node data from: \"%s\"\n", str);

    int nRead = 0;

    if(sscanf(str, "%d%n", &node->value.num, &nRead)){
        LOG_INFO("\t Found number: %d\n", node->value.num);
        node->type = ExprNodeType::NUMBER;
        return nRead;
    }

    char token[MAX_TOKEN_LEN] = "";

    sscanf(str, "%[^() \n\t]%n", token, &nRead);
    LOG_INFO("\t Excluded token \"%s\"\n", token);

    node->value.opr = getOperator(token);
    if(node->value.opr.opr != Operator::NONE){
        LOG_INFO("\t Found operator: \"%s\"\n", node->value.opr.str);
        node->type = ExprNodeType::OPERATOR;
        return nRead;
    }

    if(isalpha(token[0]) && token[0] != '\0'){
        node->type = ExprNodeType::VARIABLE;
        node->value.var = token[0];
        LOG_INFO("\t Found variable: \"%c\"\n", node->value.var);
        return 1;
    }

    LOG_ERROR("Incorrect node data: \"%10s\".\n", str);
    return 0;
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