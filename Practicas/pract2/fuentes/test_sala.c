// test_sala.c
// ===============
// Batería de pruebas de la biblioteca "sala.h/sala.c"
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/cabecera/sala.h"

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))

void INICIO_TEST (const char* titulo_test)
{
  printf("********** batería de pruebas para %s: ", titulo_test); 
 	// fflush fuerza que se imprima el mensaje anterior
	// sin necesidad de utilizar un salto de línea
	fflush(stdout);
}

void FIN_TEST (const char* titulo_test)
{
  printf ("********** hecho\n");
}


void test_ReservaBasica()
{
	int mi_asiento;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500

	INICIO_TEST("Reserva básica");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	DebeSerCierto(libera_asiento(mi_asiento)==ID_1);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	elimina_sala();
	FIN_TEST("Reserva básica");
}

void test_ReservaConsecutiva()
{
	int mi_asiento1, mi_asiento2;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500
	#define ID_2 1501

	INICIO_TEST("Reserva consecutiva");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento1=reserva_asiento(ID_1))>=0);
	DebeSerCierto((mi_asiento2=reserva_asiento(ID_2))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento1)>0);
	DebeSerCierto(estado_asiento(mi_asiento2)>0);
	DebeSerCierto(libera_asiento(mi_asiento1)==ID_1);
	DebeSerCierto(libera_asiento(mi_asiento2)==ID_2);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	elimina_sala();
	FIN_TEST("Reserva consecutiva");
}

void test_salaVacia()
{
	#define CAPACIDAD_CUYAS 500

	INICIO_TEST("Sala vacía");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto(asientos_libres()==CAPACIDAD_CUYAS);
	DebeSerCierto(asientos_ocupados()==0);
	elimina_sala();
	FIN_TEST("Sala vacía");
}

void test_ReservaAsiento()
{
	int mi_asiento;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500

	INICIO_TEST("Reserva asiento");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	elimina_sala();
	FIN_TEST("Reserva asiento");
}

void test_LiberaAsiento()
{
	int mi_asiento;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500

	INICIO_TEST("Libera asiento");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	DebeSerCierto(libera_asiento(mi_asiento)==ID_1);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	elimina_sala();
	FIN_TEST("Libera asiento");
}

void test_EstadoAsiento()
{
	int mi_asiento;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500

	INICIO_TEST("Estado asiento");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	elimina_sala();
	FIN_TEST("Estado asiento");
}

void test_ReservaLiberaEstadoAsiento()
{
	int mi_asiento;
	#define CAPACIDAD_CUYAS 500
	#define ID_1 1500

	INICIO_TEST("Reserva, libera y estado asiento");
	crea_sala(CAPACIDAD_CUYAS);
	DebeSerCierto(capacidad()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	DebeSerCierto(libera_asiento(mi_asiento)==ID_1);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	elimina_sala();
	FIN_TEST("Reserva, libera y estado asiento");
}

void test_estadoSala()
{
#define CAPACIDAD_CUYAS 500
#define ID_1 1500
#define ID_2 1501
#define ID_3 1502

INICIO_TEST("Estado sala");
crea_sala(CAPACIDAD_CUYAS);

// Reserva algunos asientos
reserva_asiento(ID_1);
reserva_asiento(ID_2);
reserva_asiento(ID_3);

// Comprobar estado de los asientos y la sala
//printf("Estado de los asientos:\n");
for (int i = 0; i < CAPACIDAD_CUYAS; i++) {
	if (i % 10 == 0) {
		//printf("\n");
	}
	//printf("%c ", estado_asiento(i) ? 'X' : '_');
}

elimina_sala();
FIN_TEST("Estado sala");
}

int sentarse(int id) {
    int asiento = reserva_asiento(id);
    if (asiento > 0) {
        //printf("La persona con ID %d se ha sentado en el asiento %d\n", id, asiento);
    } else {
        //printf("No hay asientos disponibles para la persona con ID %d\n", id);
    }
    return asiento;
}


int levantarse(int id) {
    int estado = estado_asiento(id);
    if (estado == 0) {
        return -1; // Asiento no existe o no está ocupado
    } else {
        int asiento = libera_asiento(id);
        return asiento; // Retorna el número de asiento liberado
    }
}

int reserva_multiple(int npersonas, int* lista_id) {
    int num_asientos_libres = asientos_libres(); // Guardamos el número de asientos libres de la sala
    if (num_asientos_libres < npersonas) { // Comprobamos si hay suficientes asientos libres para todas las personas
        return -1;
    }

    int asientos_reservados[npersonas]; // Arreglo para guardar los asientos reservados
    int n_asientos_reservados = 0; // Contador de asientos reservados

    for (int i = 0; i < npersonas; i++) { // Recorremos todas las personas
        int asiento = reserva_asiento(lista_id[i]); // Reservamos un asiento para la persona con id lista_id[i]
        if (asiento == -1) {
            // Liberamos los asientos reservados en caso de error
            for (int j = 0; j < n_asientos_reservados; j++) {
                libera_asiento(asientos_reservados[j]);
            }
            return -1;
        }
        asientos_reservados[n_asientos_reservados++] = asiento; // Guardamos el asiento reservado en el arreglo
    }

    return 0;
}

void test_SentarseLevantarse()
{
#define CAPACIDAD_CUYAS 500
#define ID_1 1500
#define ID_2 1501
#define ID_3 1502

    INICIO_TEST("Sentarse y levantarse");
    crea_sala(CAPACIDAD_CUYAS);
    sentarse(ID_1);
    sentarse(ID_2);
    sentarse(ID_3);
    levantarse(ID_1);
    levantarse(ID_2);
    levantarse(ID_3);
    elimina_sala();
    FIN_TEST("Sentarse y levantarse");
#undef CAPACIDAD_CUYAS
#undef ID_1
#undef ID_2
#undef ID_3
}

void test_ReservaMultiple()
{
#define CAPACIDAD_CUYAS 10
#define ID_1 1001
#define ID_2 1002
#define ID_3 1003
#define ID_4 1004
    INICIO_TEST("Reserva múltiple");
    crea_sala(CAPACIDAD_CUYAS);
    int lista_id[4] = { ID_1, ID_2, ID_3, ID_4 };
    int resultado1 = reserva_multiple(4, lista_id);
    int resultado2 = reserva_multiple(7, lista_id);
    elimina_sala();
    FIN_TEST("Reserva múltiple");
#undef CAPACIDAD_CUYAS
#undef ID_1
#undef ID_2
#undef ID_3
#undef ID_4
}

void test_ReservaImposible()
{
#define CAPACIDAD_CUYAS 10
#define ID_1 1001
#define ID_2 1002
#define ID_3 1003
#define ID_4 1004
	INICIO_TEST("Reserva imposible");
	crea_sala(CAPACIDAD_CUYAS);
	int lista_id[4] = { ID_1, ID_2, ID_3, ID_4 };
	int resultado1 = reserva_multiple(4, lista_id);
	int resultado2 = reserva_multiple(7, lista_id);
	elimina_sala();
	FIN_TEST("Reserva imposible");
}

void ejecuta_tests ()
{
	test_ReservaBasica();
	test_ReservaConsecutiva();
	test_salaVacia();
	test_ReservaAsiento();
	test_LiberaAsiento();
	test_EstadoAsiento();
	test_ReservaLiberaEstadoAsiento();
	test_estadoSala();
	test_SentarseLevantarse();
	test_ReservaMultiple();
	test_ReservaImposible();
}

int main()
{
	puts("Iniciando tests...");
	
	ejecuta_tests();
	
	puts("Batería de test completa.");
}

