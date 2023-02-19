#include <stdio.h>

int main() {
    int numero;
    int binario[32];
    int i = 0;
    
    printf("Ingrese un numero: ");
    scanf("%d", &numero);
    
    while (numero > 0) {
        binario[i] = numero % 2;
        numero = numero / 2;
        i++;
    }
    
    printf("El numero en binario es: ");
    
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binario[j]);
    }
    
    printf("\n");
    
    return 0;
}