#include "TreeLabeler.h"
#include "Logger.h"
#include "utils.h"
#include "TreeTex.h"

LabelNS* LABELS = NULL;

const char*(*getNameF)() = NULL;

void setNamingFunction(nameGen_f func){
    getNameF = func;
}

//------------------------------------------------------------------------------------------------------------

LabelNS* createLabelNS(size_t capacity){
    LabelNS* ptr = (LabelNS*) malloc(sizeof(LabelNS) + capacity * sizeof(Label));
    LOG_ASSERT(ptr != NULL);
    ptr->capacity = capacity;
    ptr->size = 0;
    return ptr;
}

//------------------------------------------------------------------------------------------------------------

void deleteLabelNS(LabelNS* labelNS){
    LOG_ASSERT(labelNS != NULL);
    labelNS->capacity = 0;
    labelNS->size = 0;
    free(labelNS);
}

//------------------------------------------------------------------------------------------------------------

const char* copyRegisterLabel(const ExprNode* node){
    LOG_ASSERT(LABELS != NULL);
    LOG_ASSERT(node   != NULL);
    if(LABELS->size >= LABELS->capacity){
        // LOG_ERROR("LabelNS overflow\n");
        return NULL;
    }

    for(size_t i = 0; i < LABELS->size; ++i){
        if(LABELS->labels[i].node == node) return NULL;
    }

    for(size_t i = 0; i < LABELS->size; ++i){
        if(LABELS->labels[i].node == NULL) continue;
        if(isTreeEq(LABELS->labels[i].node, node)){
            LABELS->labels[LABELS->size++] = {node, LABELS->labels[i].name};
            return LABELS->labels[LABELS->size -1].name;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------------------------------------

const char* registerLabel(const ExprNode* node){
    LOG_ASSERT(LABELS != NULL);
    LOG_ASSERT(node   != NULL);
    if(getNameF == NULL) return NULL;

    const char* str = copyRegisterLabel(node);
    if(str) return str;

    if(LABELS->size >= LABELS->capacity) return NULL;

    // LOG_DEBUG("New Register:\n");
    if(getNameF != NULL){
        LABELS->labels[LABELS->size++] = {node, getNameF()};
        return LABELS->labels[LABELS->size -1].name;
    }
    return NULL;
}

//------------------------------------------------------------------------------------------------------------

void deleteLabel(const ExprNode* node){
    LOG_ASSERT(LABELS != NULL);
    LOG_ASSERT(node   != NULL);

    for(size_t i = 0; i < LABELS->size; ++i){
        if(LABELS->labels[i].node == node) {
            LABELS->labels[i].node = NULL;
            LABELS->labels[i].name = NULL;
        }
    }
}

//------------------------------------------------------------------------------------------------------------

const char* getLabelName(const ExprNode* node){
    LOG_ASSERT(LABELS != NULL);
    LOG_ASSERT(node   != NULL);
    if(getNameF == NULL) return NULL;

    for(size_t i = 0; i < LABELS->size; ++i){
        if(LABELS->labels[i].node == node) return LABELS->labels[i].name;
    }
    return NULL;
}

//------------------------------------------------------------------------------------------------------------

void forEachUnique(forEach_f actFunc){
    LOG_ASSERT(LABELS != NULL);
    if(getNameF == NULL) return;

    for(size_t cur = 0; cur < LABELS->size; ++cur){
        if(LABELS->labels[cur].node == NULL) continue;
        int isUnique = 1;
        for(size_t prev = 0; prev < cur; ++prev){
            if(LABELS->labels[cur].name == LABELS->labels[prev].name){
                isUnique = 0;
                break;
            }
        }
        if(isUnique) actFunc(LABELS->labels[cur].node);
    }
}

//------------------------------------------------------------------------------------------------------------

size_t buildTreeLabeling(ExprNode* tree, int depth){
    LOG_ASSERT(tree     != NULL);
    LOG_ASSERT(LABELS   != NULL);
    LOG_ASSERT(getNameF != NULL);
    if(getNameF == NULL) return 0;
    if(getLabelName(tree)) return 1;

    if(tree->left )buildTreeLabeling(tree->left , depth + 1);
    if(tree->right)buildTreeLabeling(tree->right, depth + 1);


    size_t subSz = getSize(tree);

    if(subSz > REQUIRE_SUBTREE_SIZE && depth > 1){
        if(copyRegisterLabel(tree)) return 1;
        registerLabel(tree);
        TEX_Phrase(TEX_PLACE::MarkAs, tree);
        return 1;
    }
    return subSz;
}

//------------------------------------------------------------------------------------------------------------

size_t getSize(const ExprNode* tree){
    LOG_ASSERT(tree   != NULL);
    LOG_ASSERT(LABELS != NULL);

    if(getLabelName(tree)) return 1;

    size_t selfSz  = 0;
    size_t leftSz  = 0;
    size_t rightSz = 0;

    if(tree->left ) leftSz  = getSize(tree->left);
    if(tree->right) rightSz = getSize(tree->right);


    switch (tree->type)
    {
    case ExprNodeType::OPERATOR:
        {   
            #define OP_DEF(name, flags, strVal, Diff, TeX, Weight,...) case Operator::name: selfSz = Weight; break;
            #define L leftSz
            #define R rightSz
            switch(tree->value.opr.opr){
                #include OPERATORS_H
                case Operator::NONE:
                default:
                    LOG_ERROR("Unknown operator\n");
            }
            #undef L
            #undef R
            #undef OP_DEF
        }   
        break;
    case ExprNodeType::NUMBER:
    case ExprNodeType::NONE:
    case ExprNodeType::VARIABLE:
    default:
        return 1;
    }
    // LOG_DEBUG("Counted weight = %u\n", selfSz);
    return selfSz;

}

//------------------------------------------------------------------------------------------------------------

void setGlobalLabelNS(LabelNS* labelNS){
    LABELS = labelNS;
}

//------------------------------------------------------------------------------------------------------------

LabelNS* getGlobalLabelNS(){
    return LABELS;
}

//------------------------------------------------------------------------------------------------------------

void supressTree(const ExprNode* tree){
    LOG_ASSERT(tree   != NULL);
    LOG_ASSERT(LABELS != NULL);

    if(copyRegisterLabel(tree)) return;

    if(tree->left)  supressTree(tree->left);
    if(tree->right) supressTree(tree->right);
}