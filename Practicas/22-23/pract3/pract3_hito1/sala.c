// File: sala.c
// Created: 09-06-2023 12:00:00
// Author:  Romen Adama Caetano Ramirez

//Practica 1 Librerias
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sala.h"

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
