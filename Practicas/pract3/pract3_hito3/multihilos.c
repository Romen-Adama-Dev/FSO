// File: multihilos.c
// Created: 02-05-2023 11:00:00
// Author:  Romen Adama Caetano Ramirez

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sala.h" // Incluir la librería de la sala
#include "retardo.h" // Incluir la librería de retardo
#include <stdbool.h>

// Variable global para controlar si se debe continuar la visualización
bool visualizacion_en_progreso = true;

// Constante para el número de reservas y liberaciones que realizarán los hilos
#define NUM_RESERVAS 3

// Estructura para pasar argumentos al hilo de visualización del estado de la sala
struct VisualizacionArgs {
    int intervalo;
};

// Variable compartida para el mutex de la sala
pthread_mutex_t sala_mutex = PTHREAD_MUTEX_INITIALIZER;
// Variable de condición para esperar a que haya un asiento disponible
pthread_cond_t asiento_disponible_cond = PTHREAD_COND_INITIALIZER;
// Variable de condición para esperar a que haya una reserva para liberar
pthread_cond_t reserva_para_liberar_cond = PTHREAD_COND_INITIALIZER;

// Contador de asientos ocupados
int asientos_ocupados_hilos = 0;

// Función para realizar una reserva de asiento
void realizar_reservas(int id) {
    for (int i = 0; i < NUM_RESERVAS; i++) {
        pausa_aleatoria(0.05); // Retardo de tiempo aleatorio

        // Bloquear el mutex antes de acceder a la sala
        pthread_mutex_lock(&sala_mutex);

        // Esperar hasta que haya un asiento disponible
        while (asientos_ocupados_hilos >= capacidad()) {
            pthread_cond_wait(&asiento_disponible_cond, &sala_mutex);
        }

        // Realizar la reserva
        int resultado = reserva_asiento(id);
        if (resultado == 0) {
            asientos_ocupados_hilos++;
            printf("Hilo %ld: Reservado asiento %d\n", pthread_self(), id);
        } else {
            printf("Hilo %ld: No se pudo reservar asiento %d. Sala llena.\n", pthread_self(), id);
            break;
        }

        // Desbloquear el mutex después de acceder a la sala
        pthread_mutex_unlock(&sala_mutex);

        // Señalar a un hilo que haya quedado en espera de una reserva para liberar
        pthread_cond_signal(&reserva_para_liberar_cond);
    }
}

// Función para realizar la liberación de asientos
void realizar_liberaciones(int id) {
    for (int i = 0; i < NUM_RESERVAS; i++) {
        pausa_aleatoria(0.05); // Retardo de tiempo aleatorio

        // Bloquear el mutex antes de acceder a la sala
        pthread_mutex_lock(&sala_mutex);

        // Esperar hasta que haya una reserva para liberar
        while (asientos_ocupados_hilos <= 0) {
            pthread_cond_wait(&reserva_para_liberar_cond, &sala_mutex);
        }

        // Realizar la liberación
        int resultado = libera_asiento(id);
        // Comprobar si el asiento estaba ocupado
        if (resultado == 0) {
            asientos_ocupados_hilos--;
            printf("Hilo %ld: Liberado asiento %d\n", pthread_self(), id);
        } else {
            printf("Hilo %ld: No se pudo liberar asiento %d. Asiento vacío.\n", pthread_self(), id);
            // No es necesario usar 'break' aquí ya que solo se imprimirá un mensaje de error y se continuará con las liberaciones restantes.
        }

        // Desbloquear el mutex después de acceder a la sala
        pthread_mutex_unlock(&sala_mutex);

        // Señalar a un hilo que haya quedado en espera de un asiento disponible
        pthread_cond_signal(&asiento_disponible_cond);
    }
}

// Función auxiliar para envolver realizar_reservas
void* reservas_wrapper(void* arg) {
    // El argumento es el id del hilo
    int id = (intptr_t)arg;
    realizar_liberaciones(id);
    return NULL;
}

// Función auxiliar para envolver realizar_liberaciones
void* liberaciones_wrapper(void* arg) {
    int id = (intptr_t)arg;
    realizar_liberaciones(id);
    return NULL;
}

// Función para mostrar el estado de la sala cada cierto intervalo de tiempo
void* visualizar_estado(void* arg) {
    struct VisualizacionArgs* args = (struct VisualizacionArgs*)arg;
    int intervalo = args->intervalo;

    // Bloquear el mutex antes de acceder a la sala
    pthread_mutex_lock(&sala_mutex);

    // Esperar hasta que se haya realizado al menos una reserva
    printf("Estado de la sala:\n");
    for (int i = 1; i <= capacidad(); i++) {
        int estado = estado_asiento(i);
        if (estado == 0) {
            printf("Asiento %d: LIBRE\n", i);
        } else {
            printf("Asiento %d: OCUPADO por hilo %d\n", i, estado);
        }
    }
    printf("\n");

    // Desbloquear el mutex después de acceder a la sala
    pthread_mutex_unlock(&sala_mutex);

    // Indicar que la visualización ha finalizado
    visualizacion_en_progreso = false;

    return NULL;
}

// Función principal
int main(int argc, char* argv[]) {
    // Comprobar el número de argumentos
    if (argc != 3) {
        printf("Uso: %s n m\n", argv[0]);
        printf("Donde n es el número de hilos de reserva y m es el número de hilos de liberación.\n");
        return 1;
    }

    // Crear la sala
    int num_hilos_reserva = atoi(argv[1]);
    int num_hilos_liberacion = atoi(argv[2]);
    // Crear los hilos de reserva y liberación
    pthread_t hilos_reserva[num_hilos_reserva];
    pthread_t hilos_liberacion[num_hilos_liberacion];
    // Crear argumentos para el hilo de visualización
    struct VisualizacionArgs visualizacion_args;
    visualizacion_args.intervalo = 5000; // Intervalo de visualización en milisegundos
    crea_sala(30); // Crear una sala con capacidad para 30 asientos

    // Crear hilo de visualización del estado de la sala
    pthread_t visualizacion_hilo;
    pthread_create(&visualizacion_hilo, NULL, visualizar_estado, (void*)&visualizacion_args);

    // Crear hilos de reserva y liberación de asientos
    for (int i = 0; i < num_hilos_reserva; i++) {
        pthread_create(&hilos_reserva[i], NULL, reservas_wrapper, (void*)(intptr_t)i);
    }
    for (int i = 0; i < num_hilos_liberacion; i++) {
        pthread_create(&hilos_liberacion[i], NULL, liberaciones_wrapper, (void*)(intptr_t)i);
    }

    // Esperar a que los hilos de reserva y liberación terminen
    for (int i = 0; i < num_hilos_reserva; i++) {
        pthread_join(hilos_reserva[i], NULL);
    }
    for (int i = 0; i < num_hilos_liberacion; i++) {
        pthread_join(hilos_liberacion[i], NULL);
    }

    // Detener el hilo de visualización del estado de la sala
    pthread_cancel(visualizacion_hilo);
    pthread_join(visualizacion_hilo, NULL);

    // Destruir el mutex y las variables de condición
    pthread_mutex_destroy(&sala_mutex);
    pthread_cond_destroy(&asiento_disponible_cond);
    pthread_cond_destroy(&reserva_para_liberar_cond);

    return 0;
}
