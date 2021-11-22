#include <stdio.h>
#include "ExpTree/Tree.h"
#include "ExpTree/TreeOptimizer.h"
#include "ExpTree/TreeTex.h"
#include "Differntiator.h"
#include <string.h>
int main(){
    TEX_Start();
    char* string = strdup("((((2) ^ (x)) + ((5) * (ln((7) / (x))))) + (y))");
    ExprNode* root = growTree(string);

    // TEX_Formula(root);
    free(graphTree(root));
    ExprNode* diff = fullDifferntial(root);
    free(graphTree(diff));
    treeOptimize(diff);

    TEX_Formula(diff);

    free(graphTree(diff));
    free(string);
    deleteNode(root);
    deleteNode(diff);
    TEX_Finish();
    return 0;
}