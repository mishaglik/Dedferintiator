#include <string.h>
#include "Tree.h"
#include "Logger.h"
#include "utils.h"
#include <ctype.h>

const char* spaceSyms = " \t\n";
const size_t MAX_TOKEN_LEN = 10;

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

    switch (curOpr.opr)
    {
    case Operator::NONE:
    case Operator::ADD:
    case Operator::SUB:
    case Operator::MUL:
    case Operator::DIV:
    case Operator::SIN:
    case Operator::COS:
    case Operator::TAN:
    case Operator::COT:
    case Operator::ABS:
        return curOpr;
    default:
        curOpr.opr = Operator::NONE;
        return curOpr;
        break;
    }
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

void treeSearch(ExprNode* node, search_action_f func);

//--------------------------------------------------------------------------------------------------------------------

void writeTree(const ExprNode* tree);

//--------------------------------------------------------------------------------------------------------------------

char* graphTree(ExprNode* tree);

//--------------------------------------------------------------------------------------------------------------------

int parceNodeData(const char* str, ExprNode* node){
    LOG_ASSERT(str  != NULL);
    LOG_ASSERT(node != NULL);

    int nRead = 0;

    if(sscanf(str, "%d%n", &node->value.num, &nRead) == 2){
        node->type = ExprNodeType::NUMBER;
        return nRead;
    }

    char token[MAX_TOKEN_LEN] = "";

    sscanf(str, "%s%n", token, &nRead);

    node->value.opr = getOperator(token);
    if(node->value.opr.opr != Operator::NONE){
        node->type = ExprNodeType::OPERATOR;
        return nRead;
    }

    if(token[1] == '\0' && token[0] != '\0'){
        node->type = ExprNodeType::VARIABLE;
        node->value.var = token[0];
        return nRead;
    }

    LOG_ERROR("Incorrect node data: \"%10s\".\n", str);
    return 0;
}