#include <stdio.h>

int main() {
    printf("Valor decimal | Valor hexadecimal | Representacion grafica\n");
    
    for (int i = 0; i < 128; i++) {
        printf("%13d | %17X | %c\n", i, i, (char) i);
    }
    
    return 0;
}