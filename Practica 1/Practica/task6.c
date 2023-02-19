#include <stdio.h>
#include <string.h>

int contar_vocales(char* cadena) {
    int i, count = 0;
    char vocales[] = "aeiouAEIOU";
    
    for (i = 0; i < strlen(cadena); i++) {
        if (strchr(vocales, cadena[i]) != NULL) {
            count++;
        }
    }
    
    return count;
}

int main() {
    char cadena1[] = "Esta es una cadena de texto";
    char cadena2[] = "aeiouAEIOU";
    char cadena3[] = "hmmmm... no hay vocales aquí";
    
    printf("Cadena 1: %d vocales\n", contar_vocales(cadena1));
    printf("Cadena 2: %d vocales\n", contar_vocales(cadena2));
    printf("Cadena 3: %d vocales\n", contar_vocales(cadena3));
    
    return 0;
}
