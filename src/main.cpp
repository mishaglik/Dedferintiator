#include <stdio.h>
#include "ExpTree/Tree.h"
#include "Differntiator.h"
#include <string.h>
int main(){
    char* string = strdup("(((2) ^ (-1)) + ((5) * (ln(x))))");
    ExprNode* root = growTree(string);

    ExprNode* diff = diffentiate(root, 'x');
    writeTree(diff);

    free(graphTree(root));
    free(graphTree(diff));
    free(string);
    deleteNode(root);
    deleteNode(diff);
    return 0;
}