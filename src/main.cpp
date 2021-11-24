#include <stdio.h>
#include "ExpTree/Tree.h"
#include "ExpTree/TreeOptimizer.h"
#include "ExpTree/TreeTex.h"
#include "Differntiator.h"
#include "File.h"
#include "Logger.h"
#include "utils.h"
#include <string.h>

const char* names[] = {
    "\\alpha", "\\beta", "p", "a", "b", "c", "d", "e", "f", "g", "h", "j", "k", "l'", "\\gamma", "\\mu", "A", "B", "C", "D", "E", "F"
};
const char* getName();
const char* getName(){
    static int i = 0;
    return names[i++];
}

int main(){
    TEX_Start();
    setGlobalLabelNS(createLabelNS());
    setNamingFunction(&getName);

    FILE* inFile = fopen("func.txt", "r");
    LOG_ASSERT(inFile != NULL);

    size_t file_sz = getFileSize(inFile);

    char* string = (char*)mgk_calloc(file_sz + 1, sizeof(char));
    fread(string, sizeof(char), file_sz, inFile);

    fclose(inFile);

    ExprNode* root = growTree(string);

    ExprNode* diff = fullDifferntial(root);


    free(graphTree(diff));
    free(string);
    deleteNode(root);
    deleteNode(diff);
    deleteLabelNS(getGlobalLabelNS());
    TEX_Finish();
    return 0;
}