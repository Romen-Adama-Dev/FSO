// sala.h
#ifndef SALA_H
#define SALA_H

void crea_sala(int capacidad);
int reserva_asiento(int id);
int libera_asiento(int asiento);
int estado_asiento(int asiento);
int asientos_libres();
int asientos_ocupados();
int capacidad();
void elimina_sala();
#define ASIENTO_LIBRE 0
#define ASIENTO_OCUPADO 1

#endif // SALA_H