#include <stdio.h>

int main() {
    printf("DecHexChar\t|\tDecHexChar\t|\tDecHexChar\n");
    printf("-----------------------------------------------\n");
    
    for (int i = 32; i < 128; i += 3) {
        printf("%3d %3X %4c\t|\t", i, i, i);
        if (i + 1 < 128) printf("%3d %3X %4c\t|\t", i + 1, i + 1, i + 1);
        if (i + 2 < 128) printf("%3d %3X %4c\t", i + 2, i + 2, i + 2);
        printf("\n");
    }
    
    return 0;
}
