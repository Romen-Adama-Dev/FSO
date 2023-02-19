#include <stdio.h>
#include <string.h>

int main() {
    // Declaration of variables
    char linea[100];
    printf("Introduce una línea de texto: ");
    // Fgets reads a line from stdin and stores it in the variable
    fgets(linea, 100, stdin); 

    // The function strtok is used to separate the string into tokens
    char *palabra = strtok(linea, " ");  
    while (palabra != NULL) {
        printf("%s\n", palabra);  
        palabra = strtok(NULL, " ");  
    }

    return 0;
}

/*
 * @author @Romen-Adama-Dev
 * @date 19/02/2023    
*/ 