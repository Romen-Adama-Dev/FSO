//test_sala.c
// test_sala.c
// ===============
// Batería de pruebas de la biblioteca "sala.h/sala.c"
//

#include <assert.h>
#include "sala.h"
#include <stdio.h>

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

void ejecuta_tests ()
{
	test_ReservaBasica();
	test_EstadoSala();
	test_SentarseYLevantarse();
	// Añadir nuevos tests 
}

int main()
{
	puts("Iniciando tests...");
	
	ejecuta_tests();
	
	puts("Batería de test completa.");
}