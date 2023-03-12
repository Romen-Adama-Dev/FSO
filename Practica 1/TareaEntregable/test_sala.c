//test_sala.c
// test_sala.c
// ===============
// Batería de pruebas de la biblioteca "sala.h/sala.c"
//

#include <assert.h>
#include "sala.h"
#include <stdio.h>
#include <stdlib.h>

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))
#define DebeSerIgual(x,y)	assert((x) == (y))

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
    DebeSerCierto(capacidad() == CAPACIDAD_CUYAS);
    DebeSerCierto((mi_asiento = reserva_asiento(ID_1)) >= 0);
    DebeSerCierto((asientos_libres() + asientos_ocupados()) == CAPACIDAD_CUYAS);
    DebeSerCierto(estado_asiento(mi_asiento) > 0);
    DebeSerCierto(libera_asiento(mi_asiento) == ID_1);
    DebeSerCierto(asientos_ocupados() == 0);
    DebeSerCierto(asientos_libres() == CAPACIDAD_CUYAS);
    elimina_sala();
    FIN_TEST("Reserva básica");
}

void test_EstadoSala()
{
  INICIO_TEST("Estado de la sala");
  crea_sala(10);
  int asiento1 = reserva_asiento(1);
  int asiento2 = reserva_asiento(2);
  int asiento3 = reserva_asiento(3);

  estado_sala();

  libera_asiento(asiento1);
  estado_sala();

  elimina_sala();
  FIN_TEST("Estado de la sala");
}

void test_SentarseYLevantarse()
{
  INICIO_TEST("Sentarse y levantarse");
  crea_sala(10);
  int asiento1 = reserva_asiento(1);
  int asiento2 = reserva_asiento(2);
  int asiento3 = reserva_asiento(3);

  sentarse(asiento1);
  sentarse(asiento2);
  sentarse(asiento3);

  estado_sala();

  levantarse(asiento1);
  levantarse(asiento2);
  levantarse(asiento3);

  estado_sala();

  elimina_sala();
  FIN_TEST("Sentarse y levantarse");
}

void test_ReservaMultiple()
{
  INICIO_TEST("Reserva múltiple");

  // Definimos los datos de entrada para el test
  #define CAPACIDAD_SALA 10
  int lista_id[] = {1, 2, 3};
  int npersonas = 3;

  // Creamos la sala con la capacidad especificada
  crea_sala(CAPACIDAD_SALA);

  // Realizamos la reserva múltiple
  DebeSerCierto(reserva_multiple(npersonas, lista_id) == 0);

  // Verificamos que los asientos estén ocupados
  DebeSerCierto(asientos_ocupados() == npersonas);
  DebeSerCierto(asientos_libres() == CAPACIDAD_SALA - npersonas);

  // Liberamos los asientos reservados
  for (int i = 0; i < npersonas; i++) {
    libera_asiento(i);
  }

  // Verificamos que los asientos estén libres nuevamente
  DebeSerCierto(asientos_ocupados() == 0);
  DebeSerCierto(asientos_libres() == CAPACIDAD_SALA);

  // Eliminamos la sala
  elimina_sala();

  FIN_TEST("Reserva múltiple");
}

void test_verificar_reservasCapacidad() {
  // Crear una sala con capacidad 5
  crea_sala(5);

  // Reservar todos los asientos disponibles
  int lista_ids[5] = {1, 2, 3, 4, 5};
  int resultado = reserva_multiple(5, lista_ids);
  if (resultado != 0) {
    printf("Error al reservar asientos\n");
    exit(1);
  }

  // Intentar reservar un asiento más
  int id_nuevo = 6;
  resultado = reserva_asiento(id_nuevo);
  if (resultado != ERR_NO_CAPACITY) {
    printf("Error: Se reservó un asiento cuando ya no hay capacidad disponible\n");
    exit(1);
  }

  // Eliminar la sala creada
  elimina_sala();

  printf("El test 'test_verificar_reservasCapacidad' pasó correctamente\n");
}

void test_ReservaAsientoInexistente()
{
    int mi_asiento;
    #define CAPACIDAD_CUYAS 500
    #define ID_1 1500
    #define ID_INEXISTENTE 2500

    INICIO_TEST("Reserva de asiento inexistente");
    crea_sala(CAPACIDAD_CUYAS);
    DebeSerCierto(capacidad() == CAPACIDAD_CUYAS);
    DebeSerCierto((mi_asiento = reserva_asiento(ID_1)) >= 0);

    // Verificar que el ID del asiento proporcionado sea válido
    if (ID_INEXISTENTE < 0 || ID_INEXISTENTE >= CAPACIDAD_CUYAS) {
        printf("Error: el ID del asiento proporcionado no es válido\n");
        exit(1);
    }

    int resultado = reserva_asiento(ID_INEXISTENTE);
    // Imprimir el valor devuelto por reserva_asiento para ayudarte a depurar el problema
    printf("Valor devuelto por reserva_asiento: %d\n", resultado);

    DebeSerIgual(resultado, ERR_INVALID_SEAT);
    DebeSerCierto((asientos_libres() + asientos_ocupados()) == CAPACIDAD_CUYAS);
    DebeSerCierto(estado_asiento(mi_asiento) > 0);
    DebeSerCierto(libera_asiento(mi_asiento) == ID_1);
    DebeSerCierto(asientos_ocupados() == 0);
    DebeSerCierto(asientos_libres() == CAPACIDAD_CUYAS);
    elimina_sala();
    FIN_TEST("Reserva de asiento inexistente");
}


void ejecuta_tests ()
{
	test_ReservaBasica();
	test_EstadoSala();
	test_SentarseYLevantarse();
	test_ReservaMultiple();
	test_verificar_reservasCapacidad();
	test_ReservaAsientoInexistente();
}

int main()
{
	puts("Iniciando tests...");
	ejecuta_tests();
	puts("Batería de test completa.");
}