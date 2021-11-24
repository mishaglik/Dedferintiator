#ifndef DEDFTR_TREE_TREELABELER_H
#define DEDFTR_TREE_TREELABELER_H
#include "Node.h"

const size_t LABELS_INIT_CAPACITY = 100;
const size_t REQUIRE_SUBTREE_SIZE = 300;

struct Label
{
    const ExprNode* node  = NULL;
    const char*     name  = NULL;
};


struct LabelNS{
    size_t size     = 0;
    size_t capacity = 0;
    Label labels[];
};

void setGlobalLabelNS(LabelNS* labelNS);

LabelNS* getGlobalLabelNS();

LabelNS* createLabelNS(size_t capacity = LABELS_INIT_CAPACITY);

void deleteLabelNS(LabelNS* labelNS);

typedef const char* (*nameGen_f)();

typedef void (*forEach_f)(const ExprNode* node);

void setNamingFunction(nameGen_f func);

const char* registerLabel(const ExprNode* node);

const char* copyRegisterLabel(const ExprNode* node);

void deleteLabel(const ExprNode* node);

const char* getLabelName(const ExprNode* node);

void forEachUnique(forEach_f actFunc);

size_t buildTreeLabeling(ExprNode* tree,  int depth = 0);

size_t getSize(const ExprNode* tree);

void supressTree(const ExprNode* tree);
#endif