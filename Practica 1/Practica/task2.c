#include <stdio.h>

int main() {
    int numero;
    
    printf("Ingrese un numero: ");
    scanf("%d", &numero);
    
    printf("El numero en base hexadecimal es: %X\n", numero);
    
    return 0;
}