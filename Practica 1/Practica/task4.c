#include <stdio.h>

int main() {
    // Declaration of variables
    printf("Valor decimal | Valor hexadecimal | Representacion grafica\n");
    
    // Output of data
    for (int i = 0; i < 128; i++) {
        // The remainder of the division is stored in the array
        printf("%13d | %17X | %c\n", i, i, (char) i);
    }
    
    return 0;
}

/*
 * @author @Romen-Adama-Dev
 * @date 19/02/2023    
*/ 