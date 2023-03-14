// Author Romen-Adama-Dev
// Date: 12-03-2023
#ifndef SALA_H  
#define SALA_H
#define ERR_NO_CAPACITY -2
#define ERR_INVALID_SEAT -2

// Void and int functions declaration
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
