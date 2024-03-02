#ifndef SALA_H
#define SALA_H

int crea_sala(int capacidad);
int elimina_sala(void);
int reserva_asiento(int id_persona);
int libera_asiento(int id_asiento);
int estado_asiento(int id_asiento);
int asientos_libres(void);
int asientos_ocupados(void);
int capacidad_sala(void);

#endif
