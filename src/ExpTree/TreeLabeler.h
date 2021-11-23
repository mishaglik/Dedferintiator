#ifndef DEDFTR_TREE_TREELABELER_H
#define DEDFTR_TREE_TREELABELER_H
#include "Node.h"

struct Label
{
    ExprNode*   node  = NULL;
    const char* name  = NULL;
};


struct LabelNS{
    Label* labels = NULL;
};



#endif