// Include: stdio.h, string.h
#include <stdio.h>
#include <string.h>

// Function that counts the number of vowels in a string
int contar_vocales(char* cadena) {
    int i, count = 0;
    char vocales[] = "aeiouAEIOU";
    
    // For each character in the string, if it is a vowel, the counter is increased
    for (i = 0; i < strlen(cadena); i++) {
        if (strchr(vocales, cadena[i]) != NULL) {
            count++;
        }
    }
    
    return count;
}

int main() {
    // Declaration of variables
    // Each string is for the test of the main function
    char cadena1[] = "Esta es una cadena de texto";
    char cadena2[] = "aeiouAEIOU";
    char cadena3[] = "hmmmm... no hay vocales aquí";
    
    // Print the number of vowels in each string
    printf("Cadena 1: %d vocales\n", contar_vocales(cadena1));
    printf("Cadena 2: %d vocales\n", contar_vocales(cadena2));
    printf("Cadena 3: %d vocales\n", contar_vocales(cadena3));
    
    return 0;
}

/*
 * @author @Romen-Adama-Dev
 * @date 19/02/2023    
*/ 