// File: multihilos.c
// Created: 09-06-2023 12:00:00
// Author:  Romen Adama Caetano Ramirez
// Programa que reserva y libera asientos de forma concurrente
// Programa que reserva y libera asientos de forma concurrente

// Librerias de C 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "sala.h"
#include "retardo.h"

// Variables globales definidas
#define NUM_ASIENTOS 30
#define MAX_RESERVAS 3

// Variables globales declaradas
int asientos[NUM_ASIENTOS] = {0};
// Variable mutex
pthread_mutex_t mutex;

// Funcion para reservar y liberar asientos
void *reserva_y_libera(void *vargp) {
    // Variables locales
    int id = 0;
    int asientos_reservados[MAX_RESERVAS];

    // Bucle para reservar y liberar asientos
    for (int i = 0; i < MAX_RESERVAS; i++) {
        // Generar un id aleatorio
        id = rand() % 100;

        // Bloquear el mutex
        pthread_mutex_lock(&mutex); // Bloquear el mutex

        // Bucle para reservar asientos
        while (asientos_libres() == 0) {
            // Desbloquear el mutex
            pthread_mutex_unlock(&mutex);
            pausa_aleatoria(1.0);
            // Bloquear el mutex
            pthread_mutex_lock(&mutex);
        }

        // Reservar un asiento
        int asiento = reserva_asiento(id);

        // Desbloquear el mutex
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex

        // Comprobar si se ha reservado el asiento
        if (asiento != -1) {
            printf("Hilo %d reservó el asiento %d\n", id, asiento);
            // Si los asientos reservados son diferentes de -1 igualamos la variable asientos_reservados[i] a asiento
            asientos_reservados[i] = asiento;
        } else {
            printf("Hilo %d no pudo reservar un asiento\n", id);
        }

        pausa_aleatoria(1.0);
    }

    // Bucle para liberar asientos
    for (int i = 0; i < MAX_RESERVAS; i++) {
        pthread_mutex_lock(&mutex); // Bloquear el mutex

        // Bucle para liberar asientos
        while (asientos_ocupados() == 0) {
            // Desbloquear el mutex
            pthread_mutex_unlock(&mutex);
            pausa_aleatoria(1.0);
            // Bloquear el mutex
            pthread_mutex_lock(&mutex);
        }

        // Liberar un asiento
        id = libera_asiento(asientos_reservados[i]);

        // Desbloquear el mutex
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex

        // Comprobar si se ha liberado el asiento
        if (id == -1) {
            printf("Error no se pudo liberar el asiento \n");
        } else {
            printf("Hilo %d liberó el asiento \n", id);
        }

        pausa_aleatoria(1.0);
    }

    return NULL;
}

// Funcion para mostrar el estado de la sala
void *muestra_estado_sala(void *vargp) {
    // Bucle mediante para mostrar el estado de la sala
    while (1) {
        // Bloquear el mutex
        pthread_mutex_lock(&mutex); // Bloquear el mutex

        // Mostrar el estado de la sala
        printf("Estado de la sala: \n");
        // Recorremos el numero de asientos
        for (int i = 1; i < NUM_ASIENTOS; i++) {
            printf("%d ", estado_asiento(i));
        }
        printf("\n");

        // Desbloquear el mutex
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex

        pausa_aleatoria(1.0);
    }

    // Retornar NULL
    return NULL;
}

// Funcion principal
int main(int argc, char *argv[]) {
    // Inicializar la semilla aleatoria, si no se hace, siempre se generan los mismos numeros
    srand(getpid());

    // Comprobar el numero de argumentos
    if (argc != 2) {
        // Mostrar el uso correcto del programa
        printf("Uso: multihilos n\n");
        return 1;
    }
    
    // Convertir el argumento a entero
    int n = atoi(argv[1]);

    // Crear el array de hilos
    pthread_t threads[n];
    // Crear la sala
    crea_sala(NUM_ASIENTOS);

    // Inicializar el mutex
    pthread_mutex_init(&mutex, NULL);

    // Crear el hilo para mostrar el estado de la sala
    pthread_t thread_estado_sala;
    // Crear el hilo
    pthread_create(&thread_estado_sala, NULL, muestra_estado_sala, NULL);

    // Crear los hilos
    for (int i = 0; i < n; i++) {
        // Creamos los hilos y llamamos a la funcion reserva_y_libera
        pthread_create(&threads[i], NULL, reserva_y_libera, NULL);
    }

    // Esperar a que terminen los hilos
    for (int i = 0; i < n; i++) {
        // Hacemos un join para que los hilos terminen
        pthread_join(threads[i], NULL);
    }

    // Cancelar el hilo que muestra el estado de la sala
    pthread_cancel(thread_estado_sala);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    // Eliminar la sala, finalizar el programa
    elimina_sala();

    return 0;
}
