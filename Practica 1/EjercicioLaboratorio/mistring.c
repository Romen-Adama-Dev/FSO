/* Autor: Romen-Adama-Dev */
/* Repositorio: https://github.com/Romen-Adama-Dev */

#include "mistring.h"

#include <stdlib.h>  // necesario para malloc() y free()

// Devuelve la longitud de la cadena str
int mi_strlen(char* str) {
    int len = 0;   // Inicializamos la longitud en 0.

    // Iteramos sobre la cadena hasta encontrar el carácter nulo.
    while (*str != '\0') {
        len++;   // Incrementamos la longitud en 1.
        str++;   // Movemos el puntero a la siguiente posición de la cadena.
    }

    return len;   // Devolvemos la longitud calculada.
}

// Copia la cadena s2 en s1 y devuelve la dirección de s1
char* mi_strcpy(char* s1, char* s2) {
    char* p = s1;   // Guardamos la dirección de s1.

    // Copiamos los caracteres de s2 en s1 hasta encontrar el carácter nulo de s2.
    while (*s2 != '\0') {
        *s1 = *s2;   // Copiamos el carácter actual de s2 en s1.
        s1++;        // Movemos el puntero de s1 a la siguiente posición.
        s2++;        // Movemos el puntero de s2 a la siguiente posición.
    }

    *s1 = '\0';   // Añadimos el carácter nulo al final de s1.

    return p;   // Devolvemos la dirección de s1.
}

// Concatena la cadena s2 al final de la cadena s1 y devuelve la dirección de s1
char* mi_strcat(char* s1, char* s2) {
    char* p = s1;   // Guardamos la dirección de s1.

    // Iteramos sobre la cadena s1 hasta encontrar el carácter nulo.
    while (*s1 != '\0') {
        s1++;   // Movemos el puntero de s1 a la siguiente posición.
    }

    // Copiamos los caracteres de s2 en s1 hasta encontrar el carácter nulo de s2.
    while (*s2 != '\0') {
        *s1 = *s2;   // Copiamos el carácter actual de s2 en s1.
        s1++;        // Movemos el puntero de s1 a la siguiente posición.
        s2++;        // Movemos el puntero de s2 a la siguiente posición.
    }

    *s1 = '\0';   // Añadimos el carácter nulo al final de s1.

    return p;   // Devolvemos la dirección de s1.
}

// Crea una copia de la cadena str mediante memoria dinámica y devuelve la dirección de la copia
char* mi_strdup(char* str) {
    int len = mi_strlen(str);   // Calculamos la longitud de la cadena.
    char* p = (char*) malloc((len + 1) * sizeof(char));   // Reservamos memoria para la nueva cadena.

    // Copiamos los caracteres de str en la nueva cadena.
    for (int i = 0; i < len; i++) {
        p[i] = str[i];
    }

    p[len] = '\0';   // Añadimos el carácter nulo al final de la nueva cadena.

    return p;   // Devolvemos la dirección de la nueva cadena.
}

// Compara las cadenas s1 y s2. Si son idénticas, devuelve 1; si son diferentes, devuelve 0
int mi_strequals(char* s1, char* s2) {
    while (*s1 == *s2) { // Mientras los caracteres sean iguales
        if (*s1 == '\0') { // Si hemos llegado al final de ambas cadenas
            return 1; // Las cadenas son iguales
        }
        s1++; // Avanzamos al siguiente carácter de s1
        s2++; // Avanzamos al siguiente carácter de s2
    }
    return 0; // Si los caracteres no son iguales, las cadenas son diferentes
}
