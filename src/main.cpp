#include <stdio.h>
#include "ExpTree/Tree.h"
#include "ExpTree/TreeOptimizer.h"
#include "Differntiator.h"
#include <string.h>
int main(){
    char* string = strdup("(((2) ^ (x)) + ((5) * (ln((7) / (x)))))");
    ExprNode* root = growTree(string);

    free(graphTree(root));
    ExprNode* diff = diffentiate(root, 'x');
    free(graphTree(diff));
    treeOptimize(diff);

    writeTree(diff);

    free(graphTree(diff));
    free(string);
    deleteNode(root);
    deleteNode(diff);
    return 0;
}