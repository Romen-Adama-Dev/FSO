// File: misala.c
// Created: 15-04-2023 11:00:00
// Author:  Romen Adama Caetano Ramirez

// Practica 1 Librerias
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Practica 2 Librerias
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../lib/cabecera/sala.h"

void main() {
    // Creamos una sala de 10 asientos
    crea_sala(10);

    // Reservamos los asientos 1, 2, 3, 4, 5, 6, 7, 8, 9 y 10 de las 10 posiciones creadas anteriormente
    reserva_asiento(1);
    reserva_asiento(2);
    reserva_asiento(3);
    reserva_asiento(4);
    reserva_asiento(5);
    reserva_asiento(6);
    reserva_asiento(7);
    reserva_asiento(8);
    reserva_asiento(9);
    reserva_asiento(10);

    // Guardamos el estado de los asientos
    guarda_estado_sala("pruebasala");

    recupera_estado_sala("pruebasala");
    printf("Estado Original:\n");
    for (int i = 1; i <= capacidad(); i++) {
        printf("%d\n", estado_asiento(i));
    }

    // Liberamos los asientos 1, 2, 3, 4, 5, 6, 7, 8, 9 y 10 de las 10 posiciones creadas anteriormente
    int* asientos = malloc(sizeof(int)*3);

    // Reservamos los asientos 1, 2, 3, 4, 5, 6, 7, 8, 9 y 10 de las 10 posiciones creadas anteriormente
    *(asientos) = 1;
    *(asientos + 1) = 2;
    *(asientos + 2) = 5;

    // Recuperamos el estado de los asientos
    libera_asiento(1);
    libera_asiento(3);
    libera_asiento(5);

    // Modificamos los con nuevos valores
    reserva_asiento(20);
    reserva_asiento(40);
    reserva_asiento(60);

    // Recuperamos el estado de los asientos
    recupera_estadoparcial_sala("pruebasala", 3, asientos);
    printf("Tras modificado y recuperado parcial:\n");
    for (int i = 1; i <= capacidad(); i++) {
        printf("%d\n", estado_asiento(i));
    }

    // Liberamos los asientos 1, 2 y 3
    libera_asiento(1);
    libera_asiento(3);
    libera_asiento(5);

    // Modificamos los con nuevos valores
    reserva_asiento(20);
    reserva_asiento(40);
    reserva_asiento(60);

    // Guardamos el estado de los asientos
    guarda_estadoparcial_sala("pruebasala", 3, asientos);

    //recupera_estado_sala("prueba_sala");
    printf("Tras guardado parcial:\n");
    for (int i = 1; i <= capacidad(); i++) {
        printf("%d\n", estado_asiento(i));
    }
}