// File: sala.c
// Created: 31-03-2023 11:00:00
// Author:  Romen Adama Caetano Ramirez

//Practica 1 Librerias
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../cabecera/sala.h"

//Practica 2 Librerias
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Estructura de la sala
typedef struct {
    int *asientos; // Puntero al array de asientos
    int capacidad; // Capacidad de la sala
    int libres; // Número de asientos libres
    int ocupados; //Número de asientos ocupados
} Sala;

static Sala *sala = NULL; // Puntero a la sala global

typedef enum {
    NO_ERROR = 0,
    ERROR = -1,
    ASIENTO_SIN_RESERVAR = 0,
    ERR_CAPACIDAD_INVALIDA = -1,
    ERR_ID_CLIENTE_YA_EXISTE = -1,
    ERR_SIN_ASIENTOS_LIBRES = -1,
    ERR_ASIENTO_OCUPADO = -1,
    ERR_ASIENTO_NO_VALIDO = -1,
    ERR_MEMORIA_ASIGNADA = -1,
    ERR_ASIENTO_YA_LIBRE = -1,
    ERR_SALA_NO_EXISTE = -1,
    ERR_ABRIR_ARCHIVO = -1,
    ERR_ESCRIBRIR_ARCHIVO = -1,
    ERR_SALA_NO_CREADA = -1,
    ERR_LEER_ARCHIVO = -1,
    ERR_NUM_ASIENTOS = -1,
    ERR_CARRAR_ARCHIVO = -1,
} sala_error;

// Crea una sala con la capacidad indicada
void crea_sala(int capacidad) {
    // Comprobar si ya existe una sala creada
    if (sala != NULL) {
        // Liberar la memoria de la sala anterior
        elimina_sala();
    }
    
    // Comprobar que la capacidad de la sala sea válida
    if (capacidad <= 0) {
        // La capacidad de la sala no es válida, salir con un código de error
        exit(ERR_CAPACIDAD_INVALIDA);
    }

    // Asignar memoria para la estructura de la sala
    sala = (Sala*) malloc(sizeof(Sala));
    if (sala == NULL) {
        // No se pudo asignar memoria, salir con un código de error
        exit(ERR_MEMORIA_ASIGNADA);
    }

    // Asignar memoria para el array de asientos
    sala->asientos = (int*) malloc(capacidad * sizeof(int));
    if (sala->asientos == NULL) {
        // No se pudo asignar memoria, liberar la memoria de la sala y salir con un código de error
        free(sala);
        exit(ERR_MEMORIA_ASIGNADA);
    }

    // Inicializar la capacidad, número de asientos libres y ocupados
    sala->capacidad = capacidad;
    sala->libres = capacidad;
    sala->ocupados = 0;

    // Inicializar todos los asientos como libres
    for (int i = 0; i < capacidad; i++) {
        sala->asientos[i] = 0;
    }
}

// Elimina la sala y libera la memoria
void elimina_sala() {
    // Comprobar que la sala exista
    if (sala == NULL) {
        return;
    }
    // Liberar todos los asientos ocupados antes de eliminar la sala
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] != ASIENTO_SIN_RESERVAR) {
            libera_asiento(i + 1);
        }
    }
    // Liberar la memoria de la sala
    free(sala->asientos);
    free(sala);
    sala = NULL;
}

// Reserva un asiento y devuelve el número de asiento reservado o un código de error
int reserva_asiento(int id) {
    // Contstantes internas para los valores de retorno
    const int ASIENTO_SIN_RESERVAR = 0; // Ejemplo de valor de retorno para cuando el asiento está libre

    // Comprobar si hay una sala creada
    if (!sala) {
        return ERR_SALA_NO_CREADA;
    }

    // Comprobar si hay asientos libres
    if (sala->libres == 0) {
        return ERR_SIN_ASIENTOS_LIBRES;
    }

    // Bucle para buscar un asiento libre
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_SIN_RESERVAR) {
            // Comprobar si el asiento está disponible
            if (sala->asientos[i] != ASIENTO_SIN_RESERVAR) {
                return ERR_ASIENTO_OCUPADO;
            }
            // Reservar el asiento
            sala->asientos[i] = id;
            // Actualizar el número de asientos libres y ocupados
            sala->libres--;
            sala->ocupados++;
            return i + 1;
        }
    }
// No se encontró ningún asiento libre
return ERR_SIN_ASIENTOS_LIBRES;
}

// Libera un asiento y devuelve el id del cliente que lo tenía reservado
int libera_asiento(int asiento) {
    // Comprobar que la sala exista
    if (sala == NULL) {
        return ERR_SALA_NO_EXISTE; // La sala no ha sido creada
    }
    // Comprobar que el asiento sea válido
    if (asiento < 1 || asiento > sala->capacidad) {
        return ERR_ASIENTO_NO_VALIDO; // Asiento no válido
    }
    // Comprobar que el asiento no esté ya libre
    if (estado_asiento(asiento) == 0) {
        return ERR_ASIENTO_YA_LIBRE; // El asiento ya está libre
    }
    // Liberar el asiento
    int id = sala->asientos[asiento-1];
    // Actualizar el número de asientos libres y ocupados
    sala->asientos[asiento-1] = 0;
    sala->libres++;
    sala->ocupados--;
    return id;
}

// Devuelve el id del cliente que tiene reservado el asiento o 0 si el asiento está libre
int estado_asiento(int asiento) {
    // Comprobar que la sala exista
    if (asiento < 1 || asiento > sala->capacidad) {
        return ERR_ASIENTO_NO_VALIDO; // Asiento no válido
    }
    // Comprobar que el asiento no esté ya libre
    if (sala->asientos[asiento-1] == 0) {
        return NO_ERROR; // asiento libre
    } else {
        return sala->asientos[asiento-1]; // asiento ocupado
    }
}
// Devuelve el número de asientos libres
int asientos_libres() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->libres;
}
// Devuelve el número de asientos ocupados
int asientos_ocupados() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->ocupados;
}
// Devuelve la capacidad de la sala
int capacidad() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->capacidad;
    
}

// Funcion para guardar el estado de la sala en un fichero
int guarda_estado_sala(const char* ruta_fichero) {
    // Comprobar que la sala exista
    int fd = open(ruta_fichero, O_RDWR | O_CREAT | O_TRUNC, 0666);

    // Comprobar que se ha abierto correctamente el fichero
    if (fd == -1) {
        perror("Error al abrir el fichero");
        return ERR_ABRIR_ARCHIVO;
    }

    // reservamos memoria para el buffer
    char buffer[10];
    
    /// Escribir la capacidad de la sala
    sprintf(buffer, "%d", capacidad());
    ssize_t written = write(fd, buffer, 10);

    // Comprobar que se haya podido escribir en el fichero
    if (written == -1) {
        perror("Error al escribir en el fichero");
        close(fd);
        return errno;
    }

    // Comprobar que se haya podido abrir el fichero
    for (int i = 0; i < sala->capacidad; i++) {
        sprintf(buffer, "%d", sala->asientos[i]);
        written = write(fd, buffer, 10);

        // Comprobar que se haya podido escribir en el fichero
        if (written == -1) {
            perror("Error al escribir en el fichero");
            close(fd);
            return errno;
        }
    }

    // Condicion de si no hay error, se cierra el fichero
    if (close(fd) == -1) {
        perror("Error al cerrar el fichero");
        return errno;
    }

    return NO_ERROR;
}

// Funcion para recuperar el estado de la sala desde un fichero
int recupera_estado_sala(const char* ruta_fichero) {
    // Comprobar que el fichero exista
    int fd = open(ruta_fichero, O_RDONLY);

    // Comprobar que se ha abierto correctamente el fichero
    if (fd == -1) {
        perror("Error al abrir el fichero");
        return errno;
    }

     // reservamos memoria para el buffer
    char buffer[10];
    // Leer la capacidad de la sala
    ssize_t read_bytes = read(fd, buffer, 10);

    // Comprobar que se haya podido leer del fichero
    if (read_bytes == -1) {
        perror("Error al leer el fichero");
        close(fd);
        return errno;
    }

    // Crear la sala
    crea_sala(atoi(buffer));

    // Leer el estado de los asientos
    for (int i = 0; i < sala->capacidad; i++) {
        // Leer el estado de un asiento
        read_bytes = read(fd, buffer, 10);

        // Comprobar que se haya podido leer del fichero
        if (read_bytes == -1) {
            perror("Error al leer el fichero");
            close(fd);
            return errno;
        }

        // Guardar el estado del asiento
        sala->asientos[i] = atoi(buffer);
    }

    // Condicion de si no hay error, se cierra el fichero
    if (close(fd) == -1) {
        perror("Error al cerrar el fichero");
        return errno;
    }

    return NO_ERROR;
}

int guarda_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos){
    // Comprobar que la sala exista
    int fd = open(ruta_fichero, O_RDWR, 0666);

    // Verificar si open ha producido un error
    if (fd == -1) {
        perror("Error al abrir el fichero");
        return errno;
    }

    // reservamos memoria para el buffer
    char* buffer = malloc(10);

    // saltamos de 10 en 10 bytes hasta llegar al final del fichero
    off_t offset = lseek(fd, 10, SEEK_CUR);

    // Verificar si lseek ha producido un error
    if (offset == (off_t)-1) {
        perror("Error al saltar bytes en el fichero");
        close(fd);
        return errno;
    }

    // creamos una variable y la inicializamos a 0
    int parcial = 0;

    // Comprobar que se haya podido abrir el fichero
    for (int i = 0; i < sala->capacidad; i++)
    {
        // comprobamos si el asiento está en el array de asientos
        for (int j = 0; j < num_asientos; j++)
        {
            // si está, escribimos el id del cliente
            if ( i == *(id_asientos + j))
            {
                // escribimos el id del cliente
                parcial = 1;
                sprintf(buffer, "%d", sala->asientos[i]);
                ssize_t written = write(fd, buffer, 10);

                // Verificar si write ha producido un error
                if (written == -1) {
                    perror("Error al escribir en el fichero");
                    close(fd);
                    return errno;
                }
            }
        }

        // si no se ha escrito nada, escribimos un 0
        if (parcial == 0)
        {
            // saltamos 10 bytes
            offset = lseek(fd, 10, SEEK_CUR);

            // Verificar si lseek ha producido un error
            if (offset == (off_t)-1) {
                perror("Error al saltar bytes en el fichero");
                close(fd);
                return errno;
            }
        } else {
            // si se ha escrito algo, ponemos la variable a 0
            parcial = 0;
        }
    }
    
    // Cerrar el fichero
    if (close(fd) == -1) {
        perror("Error al cerrar el fichero");
        return errno;
    }

    return NO_ERROR;
}

// Funcion para recuperar el estado de la sala desde un fichero
int recupera_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos){
    // Comprobar que el fichero exista
    int fd = open(ruta_fichero, O_RDONLY);

    // Verificar si open ha producido un error
    if (fd == -1) {
        perror("Error al abrir el fichero");
        return errno;
    }

    // reservamos memoria para el buffer
    char* buffer = malloc(10);

    // saltamos de 10 en 10 bytes hasta llegar al final del fichero
    off_t offset = lseek(fd, 10, SEEK_CUR);

    // Verificar si lseek ha producido un error
    if (offset == (off_t)-1) {
        perror("Error al saltar bytes en el fichero");
        close(fd);
        return errno;
    }

    // creamos una variable y la inicializamos a 0
    int parcial = 0;

    // leemos los asientos de la sala
    for (int i = 0; i < sala->capacidad; i++) {
        // for anidado que comprueba si el asiento está en el array de asientos
        for (int j = 0; j < num_asientos; j++) {
            // si el asiento está en el array de asientos, lo leemos
            if ( i == *(id_asientos + j)) {
                // leemos el id del cliente
                ssize_t read_bytes = read(fd, buffer, 10);

                // Verificar si read ha producido un error
                if (read_bytes == -1) {
                    perror("Error al leer del fichero");
                    close(fd);
                    return errno;
                }

                sala->asientos[i] = atoi(buffer);
                parcial = 1;
            }
        }

        // si no se ha leido nada, saltamos 10 bytes
        if (parcial == 0) {
            // saltamos 10 bytes
            offset = lseek(fd, 10, SEEK_CUR);

            // Verificar si lseek ha producido un error
            if (offset == (off_t)-1) {
                perror("Error al saltar bytes en el fichero");
                close(fd);
                return errno;
            }
        } else {
            // si se ha leido algo, ponemos la variable a 0
            parcial = 0;
        }
    }

    // Cerrar el fichero
    if (close(fd) == -1) {
        perror("Error al cerrar el fichero");
        return errno;
    }

    return NO_ERROR;
}


// Codigo de recupera, guarda, recupera_parcial y guarda_parcial del primer intento mediante fichero de caracteres

/*
// Funcion para guardar el estado de la sala en un fichero
int guarda_estado_sala(const char* ruta_fichero) {
    // Comprobar que la sala exista
    int fd = open(ruta_fichero, O_RDWR | O_CREAT | O_TRUNC, 0666);
    // Comprobar que se haya podido abrir el fichero
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }
    
    // Puntero auxiliar para convertir los enteros a caracteres
    char* puntero_caracteres_auxiliar = malloc(sizeof(int));
    // Comprobar que se haya podido asignar memoria
    sprintf(puntero_caracteres_auxiliar, "%d", sala->capacidad);
    // Escribir la capacidad de la sala
    write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
    write(fd, "\n", sizeof(char));
    
    // Recorrer el array de asientos y escribirlos en el fichero
    for (int i = 0; i < sala->capacidad; i++) {
        sprintf(puntero_caracteres_auxiliar, "%d", sala->asientos[i]);
        write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
        write(fd, "\n", sizeof(char));
    }
    
    // Liberar la memoria
    free(puntero_caracteres_auxiliar);
    // Cerrar el fichero
    close(fd);
    return NO_ERROR;
}
*/

/*
// Funcion para recuperar el estado de la sala desde un fichero
int recupera_estado_sala(const char* ruta_fichero) {
    // Comprobar que el fichero exista
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    // Punteror palabra para leer la capacidad de la sala
    char* palabra = malloc(sizeof(int)); // reservamos memoria para la palabra
    int capacidad, pos = 0;
    
    // leemos la capacidad de la sala
    while (0 < read(fd, palabra + pos, sizeof(char))) { // leemos carácter a carácter
        if (*(palabra + pos) == '\0') break;
        if (*(palabra + pos) == '\n') break; // si encontramos un espacio, paramos la lectura
        pos++;
    }
    
    capacidad = atoi(palabra); // convertimos la palabra a un entero
    crea_sala(capacidad); // creamos la sala
    //free(palabra); // liberamos la memoria reservada

    // leemos los asientos de la sala
    
    //palabra = malloc(sizeof(int)*capacidad);
    for (int i = 0; i < capacidad; i++) {
        palabra = realloc(palabra, sizeof(int));
        pos = 0;
        while (0 < read(fd, palabra + pos, sizeof(char))) {
            if (*(palabra + pos) == '\0') break;
            if (*(palabra + pos) == '\n') break;
            pos++;
        }
        
        sala->asientos[i] = atoi(palabra);
    }
    free(palabra); // liberamos la memoria reservada

    close(fd); // cerramos el archivo
    return NO_ERROR;
    
}
*/

/*
// Función para guardar el estado parcial de la sala en un fichero
int guarda_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos)
{
    int fd = open(ruta_fichero, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        return ERR_ABRIR_ARCHIVO; // error al abrir el fichero
    }

    // Escribir el número de asientos en el fichero
    ssize_t num_escritos = write(fd, &num_asientos, sizeof(size_t));
    if (num_escritos != sizeof(size_t))
    {
        close(fd);
        return ERR_ESCRIBRIR_ARCHIVO; // error al escribir en el fichero
    }

    // Escribir los identificadores de los asientos en el fichero
    num_escritos = write(fd, id_asientos, sizeof(int) * num_asientos);
    close(fd);

    if (num_escritos != sizeof(int) * num_asientos)
    {
        return ERR_ESCRIBRIR_ARCHIVO; // error al escribir en el fichero
    }

    return NO_ERROR; // todo ha ido bien
}

// Función para recuperar el estado parcial de la sala desde un fichero
int recupera_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos)
{
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1)
    {
        return ERR_ABRIR_ARCHIVO; // error al abrir el fichero
    }

    // Leer el número de asientos desde el fichero
    size_t num_leidos;
    ssize_t lectura = read(fd, &num_leidos, sizeof(size_t));
    if (lectura != sizeof(size_t))
    {
        close(fd);
        return ERR_LEER_ARCHIVO; // error al leer el fichero
    }

    // Verificar que el número de asientos coincida
    if (num_leidos != num_asientos)
    {
        close(fd);
        return ERR_NUM_ASIENTOS; // error: el número de asientos no coincide
    }

    // Leer los identificadores de los asientos desde el fichero
    ssize_t num_leidos_total = 0;
    while (num_leidos_total < sizeof(int) * num_asientos)
    {
        ssize_t lectura_actual = read(fd, id_asientos + num_leidos_total / sizeof(int), sizeof(int) * num_asientos - num_leidos_total);
        if (lectura_actual <= 0)
        {
            break; // se alcanzó el final del archivo o se produjo un error de lectura
        }
        num_leidos_total += lectura_actual;
    }

    close(fd); // cerrar el archivo

    if (num_leidos_total != sizeof(int) * num_asientos)
    {
        return ERR_LEER_ARCHIVO; // error al leer el fichero
    }

    return NO_ERROR; // todo ha ido bien
}
*/