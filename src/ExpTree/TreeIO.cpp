#include "TreeIO.h"

#include <ctype.h>
#include <string.h>

#include "Logger.h"
#include "utils.h"

const char* OP_COLOR = "blue";
const char* CO_COLOR = "green";
const char* VA_COLOR = "black";

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
        const char* color = NULL;
        switch (data->node->type)
        {
        case ExprNodeType::VARIABLE:
            color = VA_COLOR;
            break;
        case ExprNodeType::OPERATOR:
            color = OP_COLOR;
            break;
        case ExprNodeType::NUMBER:
            color = CO_COLOR;
            break;
        case ExprNodeType::NONE:
        default:
            LOG_ERROR("Incorrect node type\n");
            break;
        }
        fprintf(file, "N%p[color = \"%s\",label=\"",data->node, color);
        fwriteNodeStr(data->node, file);
    #ifdef DEBUG_GRAPH
        fprintf(file, "%p", data->node);
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
