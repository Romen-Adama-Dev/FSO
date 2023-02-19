#include <stdio.h>

int main() {
    // Print the title of the table & the separator
    printf("DecHexChar\t|\tDecHexChar\t|\tDecHexChar\n");
    printf("-----------------------------------------------\n");
    
    // Creation of a table with the ASCII characters
    for (int i = 32; i < 128; i += 3) {
        printf("%3d %3X %4c\t|\t", i, i, i);

        // If the character is not in the ASCII table, it is replaced with a space
        if (i + 1 < 128) printf("%3d %3X %4c\t|\t", i + 1, i + 1, i + 1);
        if (i + 2 < 128) printf("%3d %3X %4c\t", i + 2, i + 2, i + 2);
        printf("\n");
    }
    
    return 0;
}

/*
 * @author @Romen-Adama-Dev
 * @date 19/02/2023    
*/ 