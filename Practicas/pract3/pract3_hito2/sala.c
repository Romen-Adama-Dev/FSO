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

    sala->capacidad = capacidad;
    sala->libres = capacidad;
    sala->ocupados = 0;
    
    pthread_mutex_init(&(sala->mutex), NULL);

    for (int i = 0; i < capacidad; i++) {
        sala->asientos[i] = 0;
    }
}

void elimina_sala() {
    if (sala == NULL) {
        return;
    }

    pthread_mutex_lock(&(sala->mutex));

    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] != ASIENTO_SIN_RESERVAR) {
            libera_asiento(i + 1);
        }
    }

    pthread_mutex_unlock(&(sala->mutex));
    pthread_mutex_destroy(&(sala->mutex));

    free(sala->asientos);
    free(sala);
    sala = NULL;

}

int reserva_asiento(int id) {
    if (!sala) {
        return ERR_SALA_NO_CREADA;
    }

    pthread_mutex_lock(&(sala->mutex));

    if (sala->libres == 0) {
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_SIN_ASIENTOS_LIBRES;
    }

    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_SIN_RESERVAR) {
            sala->asientos[i] = id;
            sala->libres--;
            sala->ocupados++;
            pthread_mutex_unlock(&(sala->mutex));
            return i + 1;
        }
    }

    pthread_mutex_unlock(&(sala->mutex));
    return ERR_ASIENTO_OCUPADO;

}

int libera_asiento(int asiento) {
    if (sala == NULL) {
        return ERR_SALA_NO_EXISTE;
    }

    if (asiento < 1 || asiento > sala->capacidad) {
    return ERR_ASIENTO_NO_VALIDO;
    }

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

    pthread_mutex_unlock(&(sala->mutex));

    return id;

}

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

int asientos_libres() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->libres;
}

int asientos_ocupados() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->ocupados;
}

int capacidad() {
    if (sala == NULL) {
        return NO_ERROR;
    }
    return sala->capacidad;
}

int guarda_estado_sala(const char* ruta_fichero) {
    int fd = open(ruta_fichero, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    char* puntero_caracteres_auxiliar = malloc(sizeof(int));
    sprintf(puntero_caracteres_auxiliar, "%d", sala->capacidad);
    write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
    write(fd, "\n", sizeof(char));

    for (int i = 0; i < sala->capacidad; i++) {
        sprintf(puntero_caracteres_auxiliar, "%d", sala->asientos[i]);
        write(fd, puntero_caracteres_auxiliar, strlen(puntero_caracteres_auxiliar));
        write(fd, "\n", sizeof(char));
    }

    free(puntero_caracteres_auxiliar);
    close(fd);
    return NO_ERROR;
}

int recupera_estado_sala(const char* ruta_fichero) {
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    char* palabra = malloc(sizeof(int));
    int capacidad, pos = 0;

    while (0 < read(fd, palabra + pos, sizeof(char))) {
        if (*(palabra + pos) == '\0') break;
        if (*(palabra + pos) == '\n') break;
        pos++;
    }

    capacidad = atoi(palabra);
    crea_sala(capacidad);

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
    free(palabra);

    close(fd);
    return NO_ERROR;
}

int guarda_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {
    int fd = open(ruta_fichero, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    ssize_t num_escritos = write(fd, &num_asientos, sizeof(size_t));
    if (num_escritos != sizeof(size_t)) {
        close(fd);
        return ERR_ESCRIBRIR_ARCHIVO;
    }

    num_escritos = write(fd, id_asientos, sizeof(int) * num_asientos);
    close(fd);
    if (num_escritos != sizeof(int) * num_asientos) {
        return ERR_ESCRIBRIR_ARCHIVO;
    }

    return NO_ERROR;
}

int recupera_estadoparcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {
    int fd = open(ruta_fichero, O_RDONLY);
    if (fd == -1) {
        return ERR_ABRIR_ARCHIVO;
    }

    size_t num_leidos;
    ssize_t lectura = read(fd, &num_leidos, sizeof(size_t));
    if (lectura != sizeof(size_t)) {
        close(fd);
        return ERR_LEER_ARCHIVO;
    }

    if (num_leidos != num_asientos) {
        close(fd);
        return ERR_NUM_ASIENTOS;
    }

    ssize_t num_leidos_total = 0;
    while (num_leidos_total < sizeof(int) * num_asientos) {
        ssize_t lectura_actual = read(fd, id_asientos + num_leidos_total, sizeof(int) * num_asientos - num_leidos_total);
        if (lectura_actual <= 0) {
            break;
        }
        num_leidos_total += lectura_actual;
    }

    close(fd);

    if (num_leidos_total != sizeof(int) * num_asientos) {
        return ERR_LEER_ARCHIVO;
    }

    return NO_ERROR;
}