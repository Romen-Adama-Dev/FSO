#include <stdio.h>
#include <stdlib.h>
#include "sala.h"

// Estructura que representa la sala de teatro
struct Sala {
    int capacidad; // Capacidad de la sala
    int *asientos; // Vector que representa los asientos de la sala
};

// Puntero a la estructura que representa la sala
static struct Sala *sala = NULL;

void crea_sala(int capacidad) {
    sala = (struct Sala *) malloc(sizeof(struct Sala));
    sala->capacidad = capacidad;
    sala->asientos = (int *) calloc(capacidad, sizeof(int));
}

void elimina_sala() {
    free(sala->asientos);
    free(sala);
    sala = NULL;
}

int reserva_asiento(int id) {
    if (sala == NULL || id < 0 || id >= sala->capacidad) {
        return -1;
    }
    if (sala->asientos[id] == ASIENTO_LIBRE) {
        sala->asientos[id] = ASIENTO_OCUPADO;
        return id;
    } else {
        return -1;
    }
}

int libera_asiento(int asiento) {
    if (sala == NULL || asiento < 0 || asiento >= sala->capacidad) {
        return -1;
    }
    if (sala->asientos[asiento] == ASIENTO_OCUPADO) {
        sala->asientos[asiento] = ASIENTO_LIBRE;
        return 0;
    } else {
        return -1;
    }
}

int estado_asiento(int asiento) {
    if (sala == NULL || asiento < 0 || asiento >= sala->capacidad) {
        return -1;
    }
    return sala->asientos[asiento];
}

int asientos_libres() {
    if (sala == NULL) {
        return -1;
    }
    int libres = 0;
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_LIBRE) {
            libres++;
        }
    }
    return libres;
}

int asientos_ocupados() {
    if (sala == NULL) {
        return -1;
    }
    int ocupados = 0;
    for (int i = 0; i < sala->capacidad; i++) {
        if (sala->asientos[i] == ASIENTO_OCUPADO) {
            ocupados++;
        }
    }
    return ocupados;
}

int capacidad() {
    if (sala == NULL) {
        return -1;
    }
    return sala->capacidad;
}
