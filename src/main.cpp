#include <stdio.h>
#include "ExpTree/Tree.h"

int main(){
    char* str = "sin";
    printf("%#08X\n", *(int*)str);
    printf("%#08X\n", Operator::SIN);
    printf("%#08X\n", 's');
    return 0;
}