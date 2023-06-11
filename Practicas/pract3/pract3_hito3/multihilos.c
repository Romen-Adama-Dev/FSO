// File: multihilos.c
// Created: 09-06-2023 12:00:00
// Author: Romen Adama Caetano Ramirez
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
int reservas_pendientes = 0;
int liberaciones_pendientes = 0;

// Variable mutex y condiciones
extern pthread_mutex_t cerrojo;
pthread_cond_t cond_reserva, cond_libera;

void *reserva_asientos(void *arg) {
    // Código de la función reserva_asientos
    for (int i = 0; i < MAX_RESERVAS; i++) {
        // Bloqueamos el mutex
        pthread_mutex_lock(&cerrojo);
        // Aumentamos el número de reservas pendientes
        reservas_pendientes++;
        // Mientras no haya asientos libres o haya liberaciones pendientes
        while (asientos_libres() == 0 || liberaciones_pendientes > 0) {
            pthread_cond_wait(&cond_reserva, &cerrojo);
        }
        // Desbloqueamos el mutex
        pthread_mutex_unlock(&cerrojo);
    
        // Reservamos un asiento aleatorio
        while (reserva_asiento(rand() % 100) == -1);
        
        // Bloqueamos el mutex
        pthread_mutex_lock(&cerrojo);
        // Disminuimos el número de reservas pendientes
        reservas_pendientes--;
        // Enviamos una señal a la condición de liberación
        pthread_cond_signal(&cond_libera);
        // Desbloqueamos el mutex
        pthread_mutex_unlock(&cerrojo);

        pausa_aleatoria(1.0);
    }
    
    pthread_exit(NULL);
}

void *libera_asientos(void *arg) {
    // Código de la función libera_asientos
    for (int i = 0; i < MAX_RESERVAS; i++) {
        // Bloqueamos el mutex
        pthread_mutex_lock(&cerrojo);
        // Aumentamos el número de liberaciones pendientes
        liberaciones_pendientes++;
        // Mientras no haya asientos ocupados o haya reservas pendientes
        while (asientos_ocupados() == 0 || reservas_pendientes > 0) {
            pthread_cond_wait(&cond_libera, &cerrojo);
        }
        // Desbloqueamos el mutex
        pthread_mutex_unlock(&cerrojo);
        // Liberamos un asiento aleatorio
        while (libera_asiento(asientos_ocupados()) == -1);
        // Bloqueamos el mutex
        pthread_mutex_lock(&cerrojo);
        // Disminuimos el número de liberaciones pendientes
        liberaciones_pendientes--;
        // Enviamos una señal a la condición de reserva
        pthread_cond_signal(&cond_reserva);
        // Desbloqueamos el mutex
        pthread_mutex_unlock(&cerrojo);

        pausa_aleatoria(1.0);
    }

    pthread_exit(NULL);
}

void *muestra_estado_sala(void *arg) {
    // Código de la función muestra_estado_salas
    // Mientras haya reservas o liberaciones pendientes
    while (1) {
        pthread_mutex_lock(&cerrojo);
        // Imprimimos el estado de la sala
        printf("Estado de la sala: \n");
        // Imprimimos el estado de los asientos
        for (int i = 1; i <= NUM_ASIENTOS; i++) {
            printf("%d ", estado_asiento(i));
        }
        printf("\n");
        // Desbloqueamos el mutex
        pthread_mutex_unlock(&cerrojo);

        pausa_aleatoria(1.0);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Código de la función main    
    if (argc != 3) {
        printf("Uso: multihilos n m\n");
        return 1;
    }

    // Convertimos los argumentos a enteros
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    srand(time(NULL)); // Establecer la semilla aleatoria

    // Reservamos memoria para los hilos
    pthread_t* threads_reserva = malloc(n * sizeof(pthread_t));
    pthread_t* threads_libera = malloc(m * sizeof(pthread_t)); 

    // Inicializamos las condiciones
    crea_sala(NUM_ASIENTOS);
    
    // Inicializamos las condiciones
    pthread_t thread_estado_sala;
    pthread_create(&thread_estado_sala, NULL, muestra_estado_sala, NULL);

    // Creamos los hilos de reserva
    for (int i = 0; i < n; i++) {
        pthread_create(threads_reserva + i, NULL, reserva_asientos, NULL);
    }

    // Esperamos a que terminen los hilos de reserva
    for (int i = 0; i < n; i++) {
        pthread_join(*(threads_reserva + i), NULL);
    }

    // Creamos los hilos de liberación solo si n < m
    if (n < m) {
        for (int i = 0; i < m; i++) {
            pthread_create(threads_libera + i, NULL, libera_asientos, NULL);
        }
    }

    // Esperamos a que terminen los hilos de liberación solo si n < m
    if (n < m) {
        for (int i = 0; i < m; i++) {
            pthread_join(*(threads_libera + i), NULL);
        }
    }

    // Esperamos a que termine el hilo de muestra_estado_sala
    elimina_sala();
    
    // Liberamos la memoria de los hilos
    free(threads_reserva);
    free(threads_libera);
    return 0;
}

