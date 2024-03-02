// test_sala.c
// ===============
// Batería de pruebas de la biblioteca "sala.h/sala.c"
//
#include "sala.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

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
	DebeSerCierto(capacidad_sala()==CAPACIDAD_CUYAS);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	DebeSerCierto(libera_asiento(mi_asiento)==ID_1);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD_CUYAS);
	elimina_sala();
	FIN_TEST("Reserva básica");
}

void test_ReservaFallida() 
{
    int mi_asiento;

    INICIO_TEST("Reserva fallida");

    // Escenario 1: Intento de reserva sin crear sala
    mi_asiento = reserva_asiento(ID_1);
    DebeSerFalso(mi_asiento >= 0); // Debería fallar ya que la sala no está creada

    // Preparación para los siguientes escenarios
    crea_sala(CAPACIDAD_CUYAS);

    // Escenario 2: Reserva con ID de persona inválido (0 o negativo)
    DebeSerFalso(reserva_asiento(0) >= 0); // ID inválido
    DebeSerFalso(reserva_asiento(-1) >= 0); // ID inválido

    // Escenario 3: Reserva múltiple más allá de la capacidad
    for (int i = 0; i < CAPACIDAD_CUYAS; i++) {
        reserva_asiento(ID_1); // Llenamos la sala
    }
    DebeSerFalso(reserva_asiento(ID_1) >= 0); // Esta reserva debería fallar, ya no hay asientos disponibles

    elimina_sala();

    FIN_TEST("Reserva fallida");
}

void test_Reserva_Libera() {
    int mi_asiento;
    int id_persona = 100; // ID de ejemplo para la persona que reserva
    int asientosLibresAntes, asientosOcupadosAntes, asientosLibresDespues, asientosOcupadosDespues;

    INICIO_TEST("Reserva y Libera Asiento");

    crea_sala(CAPACIDAD_CUYAS);
    asientosLibresAntes = asientos_libres();
    asientosOcupadosAntes = asientos_ocupados();

    // Reservar un asiento y verificar
    mi_asiento = reserva_asiento(id_persona);
    DebeSerCierto(mi_asiento >= 0);
    DebeSerCierto(estado_asiento(mi_asiento) == id_persona);

    // Verificar la actualización de asientos libres y ocupados
    asientosLibresDespues = asientos_libres();
    asientosOcupadosDespues = asientos_ocupados();
    DebeSerCierto(asientosLibresAntes - 1 == asientosLibresDespues);
    DebeSerCierto(asientosOcupadosAntes + 1 == asientosOcupadosDespues);

    // Liberar el asiento reservado y verificar
    DebeSerCierto(libera_asiento(mi_asiento) == id_persona);
    DebeSerCierto(estado_asiento(mi_asiento) == -1);

    // Verificar nuevamente la actualización de asientos libres y ocupados
    DebeSerCierto(asientos_libres() == asientosLibresAntes);
    DebeSerCierto(asientos_ocupados() == asientosOcupadosAntes);

    elimina_sala();

    FIN_TEST("Reserva y Libera Asiento");
}

void test_reserva_multiple(int npersonas, int* lista_id) {
    INICIO_TEST("Reserva Múltiple de Asientos");
    crea_sala(CAPACIDAD_CUYAS);
    bool exito = true;
    for (int i = 0; i < npersonas && exito; i++) {
        if (reserva_asiento(lista_id[i]) < 0) {
            exito = false;
            // Liberar los asientos reservados hasta ahora en caso de fallo
            for (int j = 0; j < i; j++) {
                libera_asiento(lista_id[j]);
            }
        }
    }
    DebeSerCierto(exito); // Verificar que todos los asientos han sido reservados exitosamente
    elimina_sala();
    FIN_TEST("Reserva Múltiple de Asientos");
}


void test_estado_sala() {
    INICIO_TEST("Estado de la Sala");
    crea_sala(CAPACIDAD_CUYAS);
    reserva_asiento(1);
    reserva_asiento(2);
    // Validar asientos específicos
    DebeSerCierto(estado_asiento(1) > 0); // Ocupado
    DebeSerCierto(estado_asiento(2) > 0); // Ocupado
    // No imprimimos el estado de cada asiento ni el resumen
    elimina_sala();
    FIN_TEST("Estado de la Sala");
}


void test_sentarse(int id) {
    INICIO_TEST("Sentarse en un Asiento");
    crea_sala(CAPACIDAD_CUYAS);
    int asiento = reserva_asiento(id);
    DebeSerCierto(asiento >= 0); // Verificar que la reserva fue exitosa
    elimina_sala();
    FIN_TEST("Sentarse en un Asiento");
}


void test_levantarse(int id_persona) {
    INICIO_TEST("Levantarse de un Asiento");
    crea_sala(CAPACIDAD_CUYAS);

    // Primero, reserva un asiento para la persona
    int id_asiento = reserva_asiento(id_persona);
    DebeSerCierto(id_asiento >= 0); // Verifica que la reserva fue exitosa

    // Luego, libera el asiento y verifica que el id_persona devuelto sea correcto
    DebeSerCierto(libera_asiento(id_asiento) == id_persona); // Ajuste aquí

    elimina_sala();
    FIN_TEST("Levantarse de un Asiento");
}



void ejecuta_tests ()
{
	test_ReservaBasica();
    test_ReservaFallida();
	test_Reserva_Libera();
    test_estado_sala();
    test_sentarse(123); // Ejemplo con ID 123
    test_levantarse(123); // Ejemplo con el mismo ID
    int lista_id[] = {1, 2, 3, 4, 5}; // Ejemplo de IDs para reserva múltiple
    test_reserva_multiple(5, lista_id);
}

int main()
{
	puts("Iniciando tests...");
	
	ejecuta_tests();
	
	puts("Batería de test completa.");
}