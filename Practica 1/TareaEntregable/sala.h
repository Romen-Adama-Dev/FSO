#ifndef SALA_H  
#define SALA_H


// Definición de la estructura de datos
typedef struct {
  int* asientos;
  int capacidad;
} Sala;

void crea_sala(int capacidad);
void elimina_sala();
void estado_sala();
int reserva_asiento(int id);
int libera_asiento(int asiento);
int estado_asiento(int asiento);
int asientos_libres();
int asientos_ocupados();
int capacidad();
int sentarse(int id);
void levantarse(int id);
int reserva_multiple(int npersonas, int* lista_id);

#endif
