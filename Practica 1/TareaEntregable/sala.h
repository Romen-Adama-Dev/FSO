#ifndef SALA_H
#define SALA_H

typedef struct {
  int* asientos;
  int capacidad;
} Sala;

void crea_sala(int capacidad);
void elimina_sala();
void estado_sala();
void sentarse(int id);

int reserva_asiento(int id);
int libera_asiento(int asiento);
int estado_asiento(int asiento);
int asientos_libres();
int asientos_ocupados();
int capacidad();
int levantarse(int id);

#endif
