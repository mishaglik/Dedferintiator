#include <stdio.h>
#include "ExpTree/Tree.h"
#include "ExpTree/TreeOptimizer.h"
#include "ExpTree/TreeTex.h"
#include "Differntiator.h"
#include "File.h"
#include "Logger.h"
#include "utils.h"
#include <string.h>

void CreateTex(const ExprNode* expr);

int main(){
    logger_set_log_level(FATAL);

    setGlobalLabelNS(createLabelNS());
    setNamingFunction(&TEX_GetVarName);

    FILE* inFile = fopen("func.txt", "r");
    LOG_ASSERT(inFile != NULL);

    size_t file_sz = getFileSize(inFile);

    char* string = (char*)mgk_calloc(file_sz + 1, sizeof(char));
    fread(string, sizeof(char), file_sz, inFile);

    fclose(inFile);

    ExprNode* root = growTree(string);

    CreateTex(root);

    free(string);
    deleteNode(root);
    deleteLabelNS(getGlobalLabelNS());
    return 0;
}

void CreateTex(const ExprNode* node){
    LOG_ASSERT(node != NULL);
    TEX_Start();

    TEX_Part(0);
    
    TEX_DLib();
    
    TEX_Part(1);
    TEX_Formula(node, "f");
    TEX_Part(2);
    ExprNode* diff = fullDifferntial(node);

    TEX("И собирая всё по формуле полного дифференциала получим:\n");
    TEX_Formula(diff);
    TEX("Где, \n");
    forEachUnique(&TEX_Replace);
    deleteNode(diff);

    TEX_Part(3);
    ExprNode* taylor = growTree("((x)* (sin(x)))");
    TEX_Pause();
    ExprNode* test = taylorSeries(taylor, 'x', 0, 6);
    treeOptimize(test);
    buildTreeLabeling(test);
    TEX_Resume();

    TEX("$$\n");
    TEX("f = ");
    TEX_Node(test,0);
    TEX(" + o(x^5)");
    TEX("$$\n");
    
    TEX("Где, \n");
    forEachUnique(&TEX_Replace);

    deleteNode(test);
    deleteNode(taylor);
    TEX_Part(4);
    TEX_Finish();
}

