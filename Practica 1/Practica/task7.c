#include <stdio.h>
#include <string.h>

int main() {
    char linea[100];  // Definimos un buffer para leer la línea de texto
    printf("Introduce una línea de texto: ");
    fgets(linea, 100, stdin);  // Leemos la línea de texto desde el teclado

    char *palabra = strtok(linea, " ");  // Separamos la primera palabra
    while (palabra != NULL) {
        printf("%s\n", palabra);  // Imprimimos la palabra
        palabra = strtok(NULL, " ");  // Separamos la siguiente palabra
    }

    return 0;
}
