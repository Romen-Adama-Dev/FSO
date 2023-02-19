#include <stdio.h>

int main() {

    // Declaration of variables
    int numero;
    // Array of 32 positions
    int binario[32];
    int i = 0;
    
    // Input of data
    printf("Ingrese un numero: ");
    scanf("%d", &numero);
    
    // Algorithm
    while (numero > 0) {
        // The remainder of the division is stored in the array
        binario[i] = numero % 2;
        numero = numero / 2;
        i++;
    }
    
    // Output of data
    printf("El numero en binario es: ");
    
    // The array is printed from the last position to the first
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binario[j]);
    }
    
    // End of line
    printf("\n");
    
    return 0;
}

/*
 * @author @Romen-Adama-Dev
 * @date 19/02/2023    
*/ 