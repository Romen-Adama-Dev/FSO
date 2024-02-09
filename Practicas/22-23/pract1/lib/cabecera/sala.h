// File: sala.h
// Created: 31-03-2023 11:00:00
// Author:  Romen Adama Caetano Ramirez
#ifndef SALA_H
#define SALA_H

void crea_sala(int capacidad);

void elimina_sala();

int capacidad();

int asientos_libres();

int asientos_ocupados();

int reserva_asiento(int id);

int libera_asiento(int asiento);

int estado_asiento(int asiento);

#endif
