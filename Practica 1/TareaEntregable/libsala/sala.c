// Author Romen-Adama-Dev
// Date: 12-03-2023

#include "sala.h" // Incluimos las constantes definidas en sala.h
#include <stdlib.h> // Incluimos la biblioteca estándar de C para usar malloc() y free()
#include <stdio.h> // Incluimos la biblioteca estándar de C para usar printf()

// Data structure definition
typedef struct {
  int* asientos;
  int capacidad;
} Sala;

static Sala* sala = NULL; // Declaramos una variable estática de tipo Sala* llamada sala e inicializamos su valor a NULL

void crea_sala(int capacidad) {
  sala = (Sala*)malloc(sizeof(Sala)); // Reservamos memoria para una variable de tipo Sala usando malloc() y lo casteamos a Sala*
  sala->capacidad = capacidad; // Asignamos el valor de capacidad al miembro capacidad de la estructura Sala apuntada por sala
  sala->asientos = (int*)malloc(capacidad * sizeof(int)); // Reservamos memoria para un array de capacidad elementos de tipo int usando malloc() y lo casteamos a int*
  for (int i = 0; i < capacidad; i++) { // Inicializamos todos los elementos del array asientos a 0
    sala->asientos[i] = 0;
  }
}

void elimina_sala() {
  free(sala->asientos); // Liberamos la memoria reservada para el array asientos de la estructura Sala apuntada por sala usando free()
  free(sala); // Liberamos la memoria reservada para la estructura Sala apuntada por sala usando free()
  sala = NULL; // Asignamos el valor NULL a la variable estática sala
}

void estado_sala() {
  if (sala == NULL) { // Comprobamos si sala es NULL
    //printf("La sala no ha sido creada\n"); // Si es así, imprimimos un mensaje de error y retornamos
    return;
  }
  /*
  printf("Resumen:\n");
  printf("  Aforo: %d\n", sala->capacidad); // Imprimimos la capacidad de la sala
  printf("  Asientos ocupados: %d\n", asientos_ocupados()); // Imprimimos el número de asientos ocupados llamando a la función asientos_ocupados()
  printf("  Asientos libres: %d\n", asientos_libres()); // Imprimimos el número de asientos libres llamando a la función asientos_libres()
  printf("\nEstado de los asientos:\n");
  */

  for (int i = 0; i < sala->capacidad; i++) { // Recorremos todos los elementos del array asientos de la estructura Sala apuntada por sala
    //printf("Asiento %d: %d\n", i, estado_asiento(i)); // Imprimimos el estado del asiento i llamando a la función estado_asiento()
  }
}

int reserva_asiento(int id) {
  if (sala == NULL) { // Comprobamos si sala es NULL
    return -1; // Si es así, retornamos -1
  }

  for (int i = 0; i < sala->capacidad; i++) { // Recorremos todos los elementos del array asientos de la estructura Sala apuntada por sala
    if (sala->asientos[i] == 0) { // Comprobamos si el asiento i está libre
      sala->asientos[i] = id; // Si es así, reservamos el asiento i para el cliente con id id
      return i; // Retornamos el número de asiento reservado
    }
  }

  return ERR_NO_CAPACITY; // Si no hay asientos libres, retornamos ERR_NO_CAPACITY
}

int libera_asiento(int asiento) {   
  if (sala == NULL || asiento < 0 || asiento >= sala->capacidad) { // Comprobamos si sala es NULL o si el asiento es válido
    return -1; // Si es así, retornamos -1
  }

  if (sala->asientos[asiento] == 0) { // Comprobamos si el asiento está libre
    return -1; // Si es así, retornamos -1
  }

  int id = sala->asientos[asiento]; // Guardamos el id del cliente que tenía reservado el asiento 
  sala->asientos[asiento] = 0; // Liberamos el asiento 
  return id; // Retornamos el id del cliente que tenía reservado el asiento 
}

int estado_asiento(int asiento) { 
  if (sala == NULL || asiento < 0 || asiento >= sala->capacidad) { // Comprobamos si sala es NULL o si el asiento es válido
    return -1;
  }

  return sala->asientos[asiento]; // Retornamos el id del cliente que tiene reservado el asiento o 0 si está libre
}

int asientos_libres() {
  if (sala == NULL) {
    return -1;
  }

  int libres = 0; // Contador de asientos libres
  for (int i = 0; i < sala->capacidad; i++) { // Recorremos todos los elementos del array asientos de la estructura Sala apuntada por sala
    if (sala->asientos[i] == 0) { // Comprobamos si el asiento i está libre
      libres++;
    }
  }

  return libres; // Retornamos el número de asientos libres
}

int asientos_ocupados() {
  if (sala == NULL) {
    return -1;
  }

  return sala->capacidad - asientos_libres(); // Retornamos la capacidad de la sala menos el número de asientos libres
}

int capacidad() {
  if (sala == NULL) {
    return -1;
  }

  return sala->capacidad; // Retornamos la capacidad de la sala
}

int sentarse(int id) {
  for (int i = 0; i < capacidad(); i++) { // Recorremos todos los asientos de la sala
    if (estado_asiento(i) == 0) { // Comprobamos si el asiento i está libre
      reserva_asiento(id); // Reservamos el asiento i para el cliente con id id
      return i;
    }
  }
  return -1;
}

void levantarse(int id) {
  if (sala == NULL) {
    //printf("La sala no ha sido creada\n");
    return;
  }

  int asiento = -1; // Variable para guardar el número de asiento del cliente con id id
  for (int i = 0; i < sala->capacidad; i++) { // Recorremos todos los elementos del array asientos de la estructura Sala apuntada por sala
    if (sala->asientos[i] == id) { // Comprobamos si el cliente con id id tiene reservado el asiento i
      asiento = i; // Guardamos el número de asiento del cliente con id id
      break;
    }
  }

  if (asiento == -1) { // Comprobamos si el cliente con id id tiene un asiento asignado
    //printf("Lo siento, la persona con id %d no tiene un asiento asignado\n", id);
    return;
  }

  int result = libera_asiento(asiento); // Liberamos el asiento del cliente con id id
  if (result == -1) { // Comprobamos si se ha producido algún error
    //printf("Error: El asiento %d ya está libre\n", asiento);
    return;
  } else {
    //printf("El asiento %d ha sido liberado\n", asiento);
  }
}

int reserva_multiple(int npersonas, int* lista_id) {
  if (sala == NULL) {
    //printf("La sala no ha sido creada\n");
    return -1;
  }

  int num_asientos_libres = asientos_libres(); // Guardamos el número de asientos libres
  if (num_asientos_libres < npersonas) { // Comprobamos si hay suficientes asientos libres para todas las personas
    //printf("No hay suficientes asientos libres para todas las personas\n"); 
    return -1;
  }

  int asientos_reservados = 0; // Contador de asientos reservados
  int asientos_por_liberar = 0; // Contador de asientos por liberar

  for (int i = 0; i < npersonas; i++) { // Recorremos todas las personas
    int asiento = reserva_asiento(lista_id[i]); // Reservamos un asiento para la persona con id lista_id[i]
    if (asiento == -1) {
      //printf("Error al reservar asiento para la persona con id %d\n", lista_id[i]);

      asientos_por_liberar = asientos_reservados; // Guardamos el número de asientos reservados para liberarlos después
      break;
    }
    asientos_reservados++; // Incrementamos el contador de asientos reservados
  }

  if (asientos_reservados < npersonas) {
    for (int i = 0; i < asientos_por_liberar; i++) {
      libera_asiento(i);
    }
    return -1;
  }

  return 0;
}

