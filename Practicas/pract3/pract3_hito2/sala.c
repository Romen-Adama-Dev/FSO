// File: sala.c
// Created: 02-05-2023 11:00:00
// Author: Romen Adama Caetano Ramirez

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sala.h"
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

// Estructura para almacenar los datos de la sala
typedef struct {
    int *asientos;
    int capacidad;
    int libres;
    int ocupados;
    pthread_mutex_t mutex;
} Sala;

static Sala *sala = NULL;

// Constantes para los valores de retorno de error o de éxito
typedef enum {
    NO_ERROR = 0,
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
} sala_error;


// Funciones para la gestión de la sala
void crea_sala(int capacidad) {
    if (sala != NULL) {
        elimina_sala();
    }

    if (capacidad <= 0) {
        exit(ERR_CAPACIDAD_INVALIDA);
    }

    sala = (Sala*) malloc(sizeof(Sala));
    if (sala == NULL) {
        exit(ERR_MEMORIA_ASIGNADA);
    }

    sala->asientos = (int*) malloc(capacidad * sizeof(int));
    if (sala->asientos == NULL) {
        free(sala);
        exit(ERR_MEMORIA_ASIGNADA);
    }

    // Inicializar los datos de la sala
    sala->capacidad = capacidad;
    sala->libres = capacidad;
    sala->ocupados = 0;
    
    // Inicializar el mutex
    pthread_mutex_init(&(sala->mutex), NULL);

    for (int i = 0; i < capacidad; i++) {
        sala->asientos[i] = 0;
    }
}

// Función para eliminar la sala
void elimina_sala() {
    if (sala == NULL) {
        return;
    }

    // Bloquear el mutex antes de acceder a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Liberar los asientos
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] != ASIENTO_SIN_RESERVAR) {
            libera_asiento(i + 1);
        }
    }

    // Desbloquear el mutex después de acceder a la sala
    pthread_mutex_unlock(&(sala->mutex));
    // Destruir el mutex
    pthread_mutex_destroy(&(sala->mutex));

    // Liberar la memoria de la sala
    free(sala->asientos);
    free(sala);
    sala = NULL;

}

// Función para realizar una reserva de asiento
int reserva_asiento(int id) {
    if (!sala) {
        return ERR_SALA_NO_CREADA;
    }

    pthread_mutex_lock(&(sala->mutex)); // Bloquear el mutex antes de acceder a la sala

    
    if (sala->libres == 0) {
        // Desbloquear el mutex después de acceder a la sala
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_SIN_ASIENTOS_LIBRES;
    }

    // Comprobar si el id ya existe
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_SIN_RESERVAR) {
            sala->asientos[i] = id;
            sala->libres--;
            sala->ocupados++;
            pthread_mutex_unlock(&(sala->mutex));
            return i + 1;
        }
    }

    // Desbloquear el mutex después de acceder a la sala
    pthread_mutex_unlock(&(sala->mutex));
    return ERR_ASIENTO_OCUPADO;

}

// Función para realizar la liberación de asientos
int libera_asiento(int asiento) {
    if (sala == NULL) {
        return ERR_SALA_NO_EXISTE;
    }

    if (asiento < 1 || asiento > sala->capacidad) {
    return ERR_ASIENTO_NO_VALIDO;
    }

    // Bloquear el mutex antes de acceder a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si el asiento está ocupado
    if (estado_asiento(asiento) == ASIENTO_SIN_RESERVAR) {
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_ASIENTO_YA_LIBRE; // El asiento ya está libre
    }

    // Liberar el asiento
    int id = sala->asientos[asiento-1];
    sala->asientos[asiento-1] = ASIENTO_SIN_RESERVAR;
    sala->libres++;
    sala->ocupados--;

    // Desbloquear el mutex después de acceder a la sala
    pthread_mutex_unlock(&(sala->mutex));

    // Devolver el id del cliente que tenía reservado el asiento
    return id;

}

// Función para consultar el estado de un asiento
int estado_asiento(int asiento) {
    if (asiento < 1 || asiento > sala->capacidad) {
        return ERR_ASIENTO_NO_VALIDO;
    }

    if (sala->asientos[asiento - 1] == 0) {
        return NO_ERROR;
    } else {
        return sala->asientos[asiento - 1];
    }
}

// Función para consultar el estado de la sala
int asientos_libres() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->libres;
}

// Función para consultar el estado de la sala
int asientos_ocupados() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->ocupados;
}

// Función para consultar el estado de la sala
int capacidad() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->capacidad;
}

// Función para consultar el estado de la sala
int guarda_estado_sala(const char* ruta_fichero) {
    // Abrir el fichero
    int fd = open(ruta_fichero, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    // Escribir los datos de la sala en el fichero
    char* puntero_caracteres_auxiliar = malloc(sizeof(int));
    sprintf(puntero_caracteres_auxiliar, "%d", sala->capacidad);
    write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
    write(fd, "\n", sizeof(char));

    // Escribir los datos de los asientos en el fichero
    for (int i = 0; i < sala->capacidad; i++) {
        sprintf(puntero_caracteres_auxiliar, "%d", sala->asientos[i]);
        write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
        write(fd, "\n", sizeof(char));
    }

    // Cerrar el fichero
    free(puntero_caracteres_auxiliar);
    close(fd);
    return NO_ERROR;
}

// Función para consultar el estado de la sala
int recupera_estado_sala(const char* ruta_fichero) {
    // Abrir el fichero
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    // Leer los datos de la sala del fichero
    char* palabra = malloc(sizeof(int));
    int capacidad, pos = 0;

    // Leer la capacidad de la sala
    while (0 < read(fd, palabra + pos, sizeof(char))) {
        if (*(palabra + pos) == '\0') break;
        if (*(palabra + pos) == '\n') break;
        pos++;
    }

    // Comprobar si se ha leído correctamente
    capacidad = atoi(palabra);
    crea_sala(capacidad);

    // Leer los asientos de la sala
    for (int i = 0; i < capacidad; i++) {
        palabra = realloc(palabra, sizeof(int));
        pos = 0;
        while (0 < read(fd, palabra + pos, sizeof(char))) {
            if (*(palabra + pos) == '\0') break;
            if (*(palabra + pos) == '\n') break;
            pos++;
        }

        // Comprobar si se ha leído correctamente
        sala->asientos[i] = atoi(palabra);
    }
    // Cerrar el fichero
    free(palabra);

    // Cerrar el fichero
    close(fd);
    return NO_ERROR;
}

// Función para consultar el estado de la sala
int guarda_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {
    // Abrir el fichero
    int fd = open(ruta_fichero, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    // Escribir los datos de la sala en el fichero
    ssize_t num_escritos = write(fd, &num_asientos, sizeof(size_t));
    // Comprobar si se ha escrito correctamente
    if (num_escritos != sizeof(size_t)) {
        close(fd);
        return ERR_ESCRIBRIR_ARCHIVO;
    }

    // Escribir los datos de los asientos en el fichero
    num_escritos = write(fd, id_asientos, sizeof(int) * num_asientos);
    close(fd);
    // Comprobar si se ha escrito correctamente
    if (num_escritos != sizeof(int) * num_asientos) {
        return ERR_ESCRIBRIR_ARCHIVO;
    }

    return NO_ERROR;
}

// Función para consultar el estado de la sala
int recupera_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {
    // Abrir el fichero
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    // Leer los datos de la sala del fichero
    size_t num_leidos;
    ssize_t lectura = read(fd, &num_leidos, sizeof(size_t));
    // Comprobar si se ha leído correctamente
    if (lectura != sizeof(size_t)) {
        close(fd);
        return ERR_LEER_ARCHIVO;
    }

    // Leer los asientos de la sala
    if (num_leidos != num_asientos) {
        close(fd);
        return ERR_NUM_ASIENTOS;
    }

    // Leer los asientos de la sala
    ssize_t num_leidos_total = 0;
    
    // Leer los asientos de la sala
    while (num_leidos_total < sizeof(int) * num_asientos) {
        // Leer los asientos de la sala
        ssize_t lectura_actual = read(fd, id_asientos + num_leidos_total, sizeof(int) * num_asientos - num_leidos_total);
        if (lectura_actual <= 0) {
            break;
        }
        // Comprobar si se ha leído correctamente
        num_leidos_total += lectura_actual;
    }

    close(fd);

    // Comprobar si se ha leído correctamente
    if (num_leidos_total != sizeof(int) * num_asientos) {
        return ERR_LEER_ARCHIVO;
    }

    return NO_ERROR;
}