#include "mistring.h"
#include <stdlib.h>
#include <stddef.h>

int mi_strlen (char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* mi_strcpy (char* s1, char* s2) {
    int i = 0;
    while ((s1[i] = s2[i]) != '\0') {
        i++;
    }
    return s1;
}

char* mi_strcat (char* s1, char* s2) {
    int i = mi_strlen(s1);
    int j = 0;
    while ((s1[i++] = s2[j++]) != '\0');
    return s1;
}

char* mi_strdup (char* str) {
    char* dup = (char*) malloc((mi_strlen(str) + 1) * sizeof(char));


    if (dup != NULL) {
        mi_strcpy(dup, str);
    }
    return dup;
}

int mi_strequals (char* s1, char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 - *s2) == 0 ? 1 : 0;
}

// Ejecutar con:
// gcc -o test_mistring test_mistring.c mistring.c
//./test_mistring