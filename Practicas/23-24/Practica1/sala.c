#include "sala.h"
#include <stdlib.h>

static int *asientos = NULL;
static int capacidad_actual = 0;
static int asientosOcupados = 0;

int crea_sala(int capacidad) {
    if (asientos != NULL) {
        return -1; // Error si ya existe una sala
    }
    asientos = (int *)malloc(capacidad * sizeof(int));
    if (asientos == NULL) {
        return -1; // Error de asignación de memoria
    }
    for (int i = 0; i < capacidad; i++) {
        asientos[i] = -1; // Inicializa todos los asientos como libres
    }
    capacidad_actual = capacidad;
    asientosOcupados = 0;
    return capacidad;
}

int elimina_sala() {
    if (asientos == NULL) {
        return -1; // Error si no hay una sala para eliminar
    }
    free(asientos);
    asientos = NULL;
    capacidad_actual = 0;
    asientosOcupados = 0;
    return 0;
}

int reserva_asiento(int id_persona) {
    if (asientos == NULL || id_persona <= 0) {
        return -1; // Error si no hay sala o id_persona no válido
    }
    for (int i = 0; i < capacidad_actual; i++) {
        if (asientos[i] == -1) { // Asiento libre
            asientos[i] = id_persona;
            asientosOcupados++;
            return i + 1; // Devuelve el número de asiento (index + 1)
        }
    }
    return -1; // No hay asientos libres
}

int libera_asiento(int id_asiento) {
    if (asientos == NULL || id_asiento < 1 || id_asiento > capacidad_actual) {
        return -1; // Error si no hay sala o id_asiento no válido
    }
    int index = id_asiento - 1; // Ajuste para índice de arreglo
    if (asientos[index] == -1) {
        return -1; // El asiento ya estaba libre
    }
    int id_persona = asientos[index];
    asientos[index] = -1;
    asientosOcupados--;
    return id_persona;
}

int estado_asiento(int id_asiento) {
    if (asientos == NULL || id_asiento < 1 || id_asiento > capacidad_actual) {
        return -1; // Error si no hay sala o id_asiento no válido
    }
    int index = id_asiento - 1; // Ajuste para índice de arreglo
    return asientos[index]; // Devuelve el id_persona o -1 si está libre
}

int asientos_libres() {
    if (asientos == NULL) {
        return -1; // Error si no hay sala
    }
    return capacidad_actual - asientosOcupados;
}

int asientos_ocupados() {
    if (asientos == NULL) {
        return -1; // Error si no hay sala
    }
    return asientosOcupados;
}

int capacidad_sala() {
    return capacidad_actual;
}
