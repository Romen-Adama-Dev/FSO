#ifndef MISTRING_H // Si MISTRING_H no ha sido definido aún
#define MISTRING_H // entonces lo definimos para evitar la inclusión repetida del archivo.
#include <stdlib.h>


/* Autor: Romen-Adama-Dev /
/ Repositorio: https://github.com/Romen-Adama-Dev */

// Declaración de las funciones que se implementarán en la biblioteca.
int mi_strlen(char* str);
char* mi_strcpy(char* s1, char* s2);
char* mi_strcat(char* s1, char* s2);
char* mi_strdup(char* str);
int mi_strequals(char* s1, char* s2);

#endif
