// File: sala.c
// Created: 31-03-2023 11:00:00
// Author:  Romen Adama Caetano Ramirez

// Practica 1 Librerias
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Practica 2 Librerias
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../lib/cabecera/sala.h"

// Constante para que la ruta del fichero no supere los 32 caracteres
#define MAX_RUTA_FICHERO 32
// Constante para definir el error
#define ERROR -1

// Opciones válidas
enum OpcionValida {
    OP_CREAR_SALA = 'c',
    OP_FICHERO = 'f',
    OP_SOBREESCRIBIR = 'o',
    OP_RESERVA = 'r',
    OP_ANULA = 'a',
};

// Funcion de error
int comprueba_error()
{
    if (errno != 0)
    {
        fprintf(stderr, "Hubo un error con código %d : %s", errno, strerror(errno));
        fflush(stderr);
        errno = 0;
        return ERROR;
    }

    return 0;
}

// Funcion que imprime la ayuda para en caso de fallo o introduccion inesperada de parametros
void imprime_ayuda()
{
    printf("Opción no válida. Las opciones válidas son:\n");
    printf("-c: crea una sala con la capacidad indicada y guarda el estado inicial en un fichero.\n");
    printf("-f: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
    printf("-o: sobreescribe el fichero especificado en '-f', si ya existe.\n");
    printf("-r: realiza la reserva de un número de asientos para las personas cuyos identificadores se especifican en la orden.\n");
}

// Funcion que crea una sala con la capacidad indicada, modifica mediante una reserva y guarda el estado en un fichero
int main(int argc, char *argv[])
{
    // Variables con las que se trabajara
    char caracter;
    // Flags para saber que modo se ha introducido
    // Flag crea sala
    int modo_crea_sala = 0;
    // Flag fichero
    int modo_fichero = 0;
    // Flag sobreescribir
    int modo_sobreescribir = 0;
    // Flag reserva
    int modo_reserva = 0;
    // Flag anula
    int modo_anula = 0;
    // Flag estado
    int modo_estado = 0;
    // Capacidad de la sala
    int cap = 0;
    // Ruta del fichero
    char ruta_fichero[MAX_RUTA_FICHERO];
    // Numero de asientos
    int num_asientos = 0;
    // Numero de personas
    int num_personas = 0;
    // Array de personasS
    int i = 0;
    int* personas = malloc(sizeof(int));

    // Comprobamos que se ha introducido un modo de los solicitados
    if(strcmp(argv[1],"crea") == 0)
    {
        modo_crea_sala = 1;
    }
    else if (strcmp(argv[1],"reserva") == 0)
    {
        modo_reserva = 1;
    }
    else if (strcmp(argv[1],"anula") == 0)
    {
        modo_anula = 1;
    }
    else if (strcmp(argv[1],"estado") == 0)
    {
        modo_estado = 1;
    }

    // Comprobamos que se ha introducido un fichero, parametro obligatorio y otras opciones

    while ((caracter = getopt(argc, argv, ":c:f:n:a:o")) != -1)
    {
        switch (caracter)
        {
        // Opcion crea sala
        case 'c':
            modo_crea_sala = 1;
            cap = atoi(optarg);
            break;

        // Opcion fichero    
        case 'f':
            modo_fichero = 1;
            strcpy(ruta_fichero, optarg);
            break;

        // Opcion sobreescribir    
        case 'o':
            modo_sobreescribir = 1;
            break;

        // Opcion reserva
        case 'r':
            modo_reserva = 1;
            break;

        // Opcion numero de asientos
        case 'n':
            num_asientos = atoi(optarg);
            personas = realloc(personas,sizeof(int) * num_asientos);
            for (size_t i = 0; i < num_asientos; i++)
            {
                *(personas + i) = atoi(argv[optind + i]);
                num_personas++;
            }
            
            break;

        // Opcion anula
        case 'a':
            num_personas = argc - optind + 1;
            personas = realloc(personas,sizeof(int) * num_personas);
            for (size_t i = 0; i < num_personas; i++)
            {
                *(personas + i) = atoi(argv[optind + i]);
            }
            break;

        // Opcion estado
        case ':':
            printf("Falta argumento para la opción '%c'\n", optopt);
            return ERROR;

        // Opcion no valida
        case '?':
            printf("Opción inválida '%c'\n", optopt);
            imprime_ayuda();
            return ERROR;

        default:
            break;
        }
    }


    // Comprobamos que se ha introducido un modo de los solicitados
    // Modo crea sala
    if (modo_crea_sala && modo_fichero && cap > 0)
    {
        int result = 0;
        if (access(ruta_fichero, F_OK) == 0 && !modo_sobreescribir)
        {
            printf("El fichero especificado ya existe y no se ha especificado la opción '-o'.\n");
            return ERROR;
        }
        else
        {
            crea_sala(cap);
            result = guarda_estado_sala(ruta_fichero);
            if (result == -1)
            {
                printf("Error al guardar el estado de la sala en el fichero especificado.\n");
                return ERROR;
            }
        }
    }

    // Modo reserva
    else if (modo_reserva && modo_fichero)
    {
        int result = 0;

        if (num_asientos <= 0)
        {
            fprintf(stderr, "El número de asientos debe ser un entero positivo mayor que cero.\n");
            return ERROR;
        }
        if (num_personas != num_asientos)
        {
            fprintf(stderr, "El número de personas indicado no coincide con el número de asientos.\n");
            return ERROR;
        }
        if (access(ruta_fichero, F_OK) != 0)
        {
            fprintf(stderr, "La ruta especificada para el fichero no es válida o no se tienen los permisos adecuados.\n");
            return ERROR;
        }
        result = recupera_estado_sala(ruta_fichero);
        if (result == -1)
        {
            fprintf(stderr, "Ha ocurrido un error al recuperar el estado de la sala.\n");
            return ERROR;
        }
        
        for (i = 0; i < num_personas; i++)
        {
            result = reserva_asiento(personas[i]);
            if (result == -1)
            {   
                fprintf(stderr, "Ha ocurrido un error al reservar el asiento para la persona con identificador %d.\n", personas[i]);
                return ERROR;
            }
        }
        
        result = guarda_estado_sala(ruta_fichero);
        if (result == -1)
        {
            fprintf(stderr, "Ha ocurrido un error al guardar el estado de la sala después de realizar la reserva.\n");
            return ERROR;
        }
    return 0;

    }

    // Modo estado
    else if (modo_estado && modo_fichero)
    {
        int result = 0;

        if (access(ruta_fichero, F_OK) != 0)
        {
            fprintf(stderr, "La ruta especificada para el fichero no es válida o no se tienen los permisos adecuados.\n");
            return ERROR;
        }

        result = recupera_estado_sala(ruta_fichero);

        if (result == -1)
        {
            fprintf(stderr, "Ha ocurrido un error al recuperar el estado de la sala.\n");
            return ERROR;
        }

        recupera_estado_sala(ruta_fichero);

        printf("Estado de la sala:\n");
        for (int i = 0; i < capacidad(); i++)
        {
            printf("\n");
            printf("%d", estado_asiento(i));
        }
        printf("\n");
        
    }
    // Prints de ayuda por si no se sabe usar el programa mediante CLI
    else
    {
        printf("Modo de uso:\n");
        printf("%s crea -c <capacidad> -f <ruta_fichero> [-o]\n", argv[0]);
        printf("  -c <capacidad>: crea una sala con la capacidad indicada.\n");
        printf("  -f <ruta_fichero>: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
        printf("  -o: sobreescribe el fichero especificado en '-f', si ya existe.\n");
        printf("\n");
        printf("%s reserva -f <ruta_fichero> -n <numero_de_asientos> <id_persona1> <id_persona2> ... <id_personaN>\n", argv[0]);
        printf("  -f <ruta_fichero>: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
        printf("  -n <numero_de_asientos>: número de asientos que se reservarán.\n");
        printf("  <id_persona1> <id_persona2> ... <id_personaN>: identificadores de las personas que reservarán los asientos. El número de identificadores debe ser igual al número de asientos a reservar.\n");
        printf("\n");
        printf("%s anula -f <ruta_fichero> <id_persona>\n", argv[0]);
        printf("  -f <ruta_fichero>: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
        printf("  <id_persona>: identificador de la persona cuya reserva se anulará.\n");
        printf("\n");
        printf("%s estado -f <ruta_fichero> \n", argv[0]);
        printf("  -f <ruta_fichero>: indica el fichero donde se guardará o se recuperará el estado de la sala.\n");
        printf("  <cade de texto con saltos de linea> Imprimir lo almacenado en el archivo\n");
        printf("\n");
        return 1;
    }
    free(personas);
}
    
