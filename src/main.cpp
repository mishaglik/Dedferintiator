#include <stdio.h>
#include "ExpTree/Tree.h"
#include <string.h>
int main(){
    char* string = strdup("(((2) ^ (-1)) + ((5) * (sin(x))))");
    ExprNode* root = growTree(string);

    writeTree(root);

    free(graphTree(root));
    free(string);
    deleteNode(root);
    return 0;
}