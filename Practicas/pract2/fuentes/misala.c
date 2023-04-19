#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../lib/cabecera/sala.h"

int comprueba_error()
{
    if (errno != 0)
    {
        fprintf(stderr, "Hubo un error con código %d : %s", errno, strerror(errno));
        fflush(stderr);
        errno = 0;
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char caracter;

    int flag_crea = 0;
    int flag_capacidad = 0;
    int flag_f = 0;
    int flag_o = 0;
    int capacidad = 0;
    char ruta_fichero[32];

    while ((caracter = getopt(argc, argv, ":c:f:")) != -1)
    {
        switch (caracter)
        {
        case 'c':
            flag_capacidad = 1;
            capacidad = atoi(optarg);
            break;

        case 'f':
            flag_f = 1;
            strcpy(ruta_fichero, optarg);
            break;

        case ':':
            printf("Falta argumento! : %c \n", optopt);
            break;

        case '?':
            printf("Opción inesperada! : %c \n", optopt);
            break;

        default:
            break;
        }
    }

    if (flag_capacidad == 1 &&flag_f == 1)
    {
        int result = 0;
        if (access(ruta_fichero, F_OK) == 0 && flag_o == 0)
        {
            printf("El fichero especificado ya existe y no se ha especificado la opción '-o'.\n");
            result = -1;
        }
        else
        {
            crea_sala(capacidad);
            result = guarda_estado_sala(ruta_fichero);
            if (result == -1)
            {
                printf("Ha ocurrido un error al guardar el estado inicial de la sala.\n");
            }
        }
        return result;
    }
    else
    {
        printf("Opción no válida. Las opciones válidas son:\n");
        printf("-c: crea una sala con la capacidad indicada y guarda el estado inicial en un fichero.\n");
        printf("-f: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
        printf("-o: sobreescribe el fichero especificado en '-f', si ya existe.\n");
        return -1;
    }
}
