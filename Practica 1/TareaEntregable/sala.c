#include "sala.h"
#include <stdlib.h>
#include <stdio.h>

static Sala* sala = NULL;

void crea_sala(int capacidad) {
  sala = (Sala*)malloc(sizeof(Sala));
  sala->capacidad = capacidad;
  sala->asientos = (int*)malloc(capacidad * sizeof(int));
  for (int i = 0; i < capacidad; i++) {
    sala->asientos[i] = 0;
  }
}

void elimina_sala() {
  free(sala->asientos);
  free(sala);
  sala = NULL;
}

void estado_sala() {
  if (sala == NULL) {
    printf("La sala no ha sido creada\n");
    return;
  }

  printf("Resumen:\n");
  printf("  Aforo: %d\n", sala->capacidad);
  printf("  Asientos ocupados: %d\n", asientos_ocupados());
  printf("  Asientos libres: %d\n", asientos_libres());
  printf("\nEstado de los asientos:\n");

  for (int i = 0; i < sala->capacidad; i++) {
    printf("Asiento %d: %d\n", i, estado_asiento(i));
  }
}

void sentarse(int id) {
  if (sala == NULL) {
    printf("La sala no ha sido creada\n");
    return;
  }

  int asiento = reserva_asiento(id);
  if (asiento == -1) {
    printf("Lo siento, la sala está llena\n");
  } else {
    printf("El asiento %d está asignado a la persona con id %d\n", asiento, id);
  }
}

int reserva_asiento(int id) {
  if (sala == NULL) {
    return -1;
  }

  for (int i = 0; i < sala->capacidad; i++) {
    if (sala->asientos[i] == 0) {
      sala->asientos[i] = id;
      return i;
    }
  }

  return -1;
}

int libera_asiento(int asiento) {
  if (sala == NULL || asiento < 0 || asiento >= sala->capacidad) {
    return -1;
  }

  if (sala->asientos[asiento] == 0) {
    return -1;
  }

  int id = sala->asientos[asiento];
  sala->asientos[asiento] = 0;
  return id;
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
    if (sala->asientos[i] == 0) {
      libres++;
    }
  }

  return libres;
}

int asientos_ocupados() {
  if (sala == NULL) {
    return -1;
  }

  return sala->capacidad - asientos_libres();
}

int capacidad() {
  if (sala == NULL) {
    return -1;
  }

  return sala->capacidad;
}

