// File: sala.c
// Created: 09-06-2023 12:00:00
// Author:  Romen Adama Caetano Ramirez

// Practica 1 Librerias
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sala.h"

// Practica 2 Librerias
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Practica 3 Librerias
// Incluir la biblioteca de hilos
#include <pthread.h>
#include <stdbool.h>

// Enums y constantes de error y éxito
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

// Estructura de la sala
typedef struct {
    int *asientos; // Puntero al array de asientos
    int capacidad; // Capacidad de la sala
    int libres; // Número de asientos libres
    int ocupados; // Número de asientos ocupados
    pthread_mutex_t mutex; // Mutex para sincronizar el acceso a la sala
} Sala;

static Sala *sala = NULL; // Puntero a la sala global

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
    sala = (Sala*)malloc(sizeof(Sala));
    if (sala == NULL) {
        // No se pudo asignar memoria, salir con un código de error
        exit(ERR_MEMORIA_ASIGNADA);
    }

    // Asignar memoria para el array de asientos
    sala->asientos = (int*)malloc(capacidad * sizeof(int));
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

    // Inicializar el mutex de la sala
    if (pthread_mutex_init(&(sala->mutex), NULL) != 0) {
        // Error al inicializar el mutex, liberar la memoria de la sala y salir con un código de error
        free(sala->asientos);
        free(sala);
        exit(ERR_MEMORIA_ASIGNADA);
    }
}

// Elimina la sala y libera la memoria
void elimina_sala() {
    // Comprobar que la sala exista
    if (sala != NULL) {
        pthread_mutex_lock(&(sala->mutex));
        pthread_mutex_unlock(&(sala->mutex));
        pthread_mutex_destroy(&(sala->mutex));
        // Liberar la memoria de la sala
        free(sala->asientos);
        free(sala);
        sala = NULL;
    }
}

// Reserva un asiento y devuelve el número de asiento reservado o un código de error
int reserva_asiento(int id) {
    // Contstantes internas para los valores de retorno
    const int ASIENTO_SIN_RESERVAR = 0;
    int asiento_reservado = ERR_SIN_ASIENTOS_LIBRES; // Variable para mantener el asiento reservado

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (sala == NULL) {
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_SALA_NO_CREADA;
    }

    // Comprobar si hay asientos libres
    if (sala->libres == 0) {
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_SIN_ASIENTOS_LIBRES;
    }

    // Recorre el array de asientos de "sala" buscando un asiento libre
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_SIN_RESERVAR) {
            sala->asientos[i] = id;
            sala->libres--;
            sala->ocupados++;
            asiento_reservado = i + 1;
            break;
        }
    }

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    // Devolvemos el asiento reservado
    return asiento_reservado;
}

// Libera un asiento y devuelve el id del cliente que lo tenía reservado
int libera_asiento(int asiento) {
    // Identificador del cliente que tenía reservado el asiento
    int id;

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (sala == NULL) {
        // No hay una sala creada, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_SALA_NO_EXISTE;
    }

    // Comprobar si el asiento es válido
    if (asiento < 1 || asiento > sala->capacidad) {
        // El asiento no es válido, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_ASIENTO_NO_VALIDO;
    }

    // Comprobar si el asiento está libre
    if (sala->asientos[asiento - 1] == 0) {
        // El asiento está libre, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_ASIENTO_YA_LIBRE;
    }

    // Liberar el asiento
    id = sala->asientos[asiento - 1];
    sala->asientos[asiento - 1] = 0;
    sala->libres++;
    sala->ocupados--;

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    return id;
}

// Devuelve el id del cliente que tiene reservado el asiento o 0 si el asiento está libre
int estado_asiento(int asiento) {
    // Variable para mantener el estado del asiento
    int estado;

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (asiento < 1 || asiento > sala->capacidad) {
        // No hay una sala creada, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return ERR_ASIENTO_NO_VALIDO;
    }

    // Comprobar si el asiento es válido
    estado = (sala->asientos[asiento - 1] == 0) ? NO_ERROR : sala->asientos[asiento - 1];

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    return estado;
}

// Devuelve el número de asientos libres
int asientos_libres() {
    // Variable para mantener el número de asientos libres
    int libres;

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (sala == NULL) {
        // No hay una sala creada, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return NO_ERROR;
    }

    // Devolvemos el número de asientos libres
    libres = sala->libres;

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    return libres;
}

// Devuelve el número de asientos ocupados
int asientos_ocupados() {
    // Variable para mantener el número de asientos ocupados
    int ocupados;

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (sala == NULL) {
        // No hay una sala creada, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return NO_ERROR;
    }

    // Devolvemos el número de asientos ocupados
    ocupados = sala->ocupados;

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    return ocupados;
}
// Devuelve la capacidad de la sala
int capacidad() {
    // Variable para mantener la capacidad de la sala
    int capacidad;

    // Bloqueamos con un mutex el acceso a la sala
    pthread_mutex_lock(&(sala->mutex));

    // Comprobar si hay una sala creada
    if (sala == NULL) {
        // No hay una sala creada, desbloqueamos el mutex y devolvemos un código de error
        pthread_mutex_unlock(&(sala->mutex));
        return NO_ERROR;
    }
    // Devolvemos la capacidad de la sala
    capacidad = sala->capacidad;

    // Desbloqueamos el mutex
    pthread_mutex_unlock(&(sala->mutex));
    return capacidad;
}
