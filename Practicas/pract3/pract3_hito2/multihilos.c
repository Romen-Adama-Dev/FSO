#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sala.h" // Incluir la librería de la sala
#include "retardo.h" // Incluir la librería de retardo

#define NUM_RESERVAS 3

// Estructura para pasar argumentos al hilo de visualización del estado de la sala
struct VisualizacionArgs {
    int intervalo;
};

// Variable compartida para el mutex de la sala
pthread_mutex_t sala_mutex = PTHREAD_MUTEX_INITIALIZER;

// Función para realizar una reserva de asiento
void realizar_reservas(int id) {
    for (int i = 0; i < NUM_RESERVAS; i++) {
        pausa_aleatoria(0.05); // Retardo de tiempo aleatorio

        // Bloquear el mutex antes de acceder a la sala
        pthread_mutex_lock(&sala_mutex);

        int resultado = reserva_asiento(id);
        if (resultado == 0) {
            printf("Hilo %ld: Reservado asiento %d\n", pthread_self(), id);
        } else {
            printf("Hilo %ld: No se pudo reservar asiento %d. Sala llena.\n", pthread_self(), id);
            break;
        }

        // Desbloquear el mutex después de acceder a la sala
        pthread_mutex_unlock(&sala_mutex);
    }
}

// Función para realizar la liberación de asientos
void realizar_liberaciones(int id);

void realizar_liberaciones(int id) {
    for (int i = 0; i < NUM_RESERVAS; i++) {
        pausa_aleatoria(0.05); // Retardo de tiempo aleatorio

        // Bloquear el mutex antes de acceder a la sala
        pthread_mutex_lock(&sala_mutex);

        int resultado = libera_asiento(id);
        if (resultado == 0) {
            printf("Hilo %ld: Liberado asiento %d\n", pthread_self(), id);
        } else {
            printf("Hilo %ld: No se pudo liberar asiento %d. Asiento vacío.\n", pthread_self(), id);
            // No es necesario usar 'break' aquí ya que solo se imprimirá un mensaje de error y se continuará con las liberaciones restantes.
        }

        // Desbloquear el mutex después de acceder a la sala
        pthread_mutex_unlock(&sala_mutex);
    }
}

// Función auxiliar para envolver realizar_reservas
void* reservas_wrapper(void* arg) {
    int id = (intptr_t)arg;
    realizar_liberaciones(id);
    return NULL;
}

// Función para mostrar el estado de la sala cada cierto intervalo de tiempo
void* visualizar_estado(void* arg) {
    struct VisualizacionArgs* args = (struct VisualizacionArgs*)arg;
    int intervalo = args->intervalo;

    while (1) {
        usleep(intervalo * 1000);

        // Bloquear el mutex antes de acceder a la sala
        pthread_mutex_lock(&sala_mutex);

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
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s n\n", argv[0]);
        printf("Donde n es el número de hilos\n");
        return 1;
    }
    int num_hilos = atoi(argv[1]);
    pthread_t hilos[num_hilos];
    struct VisualizacionArgs visualizacion_args;
    visualizacion_args.intervalo = 5000; // Intervalo de visualización en milisegundos
    crea_sala(30); // Crear una sala con capacidad para 30 asientos

    // Crear hilo de visualización del estado de la sala
    pthread_t visualizacion_hilo;
    pthread_create(&visualizacion_hilo, NULL, visualizar_estado, (void*)&visualizacion_args);

    // Crear hilos de reserva y liberación de asientos
    for (int i = 0; i < num_hilos; i++) {
    pthread_create(&hilos[i], NULL, reservas_wrapper, (void*)(intptr_t)i);
    }
    // Esperar a que los hilos de reserva y liberación terminen
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Detener el hilo de visualización del estado de la sala
    pthread_cancel(visualizacion_hilo);
    pthread_join(visualizacion_hilo, NULL);

    elimina_sala(); // Eliminar la sala

    return 0;
}

