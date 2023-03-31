// test_mistring.c
// ===============
// Batería de pruebas de la biblioteca "mistring"
//
// Podrás empezar a usarla cuando hayas hecho un fichero "mistring.h"
// al menos con los cuerpos vacíos de las funciones mi_strlen, mi_strcat 
// y mi_strequals

#ifndef TEST
#include "mistring.h"
#endif

#include <assert.h>
#include <stdio.h>

// Este bloque es para probar que "test_mistring.c" está correctamente
// escrito, sin necesidad de que aún esté implementado "mi_string.c"
// En ese caso, compilar con la opción -DTEST
#ifdef TEST
#include <string.h>
#define mi_strlen strlen
#define mi_strcat strcat
#define mi_strequals(a,b) (strcmp(a,b)==0)
#endif

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))

void INICIO_TEST (const char* titulo_test)
{
  printf("- batería de pruebas para %s: ", titulo_test); 
 	// fflush fuerza que se imprima el mensaje anterior
	// sin necesidad de utilizar un salto de línea
	fflush(stdout);
}

void FIN_TEST (const char* titulo_test)
{
  printf ("hecho\n");
}

void test_mi_strlen()
{
  char cadena_1000 [1000];
  int i;
  #define LONGITUD_TEST 500
  for (i=0; i<LONGITUD_TEST; i++) 
  {
    cadena_1000[i] = 32+(rand()%96);
  }
  cadena_1000[LONGITUD_TEST] = '\0'; 
  
  INICIO_TEST("mi_strlen");
	DebeSerCierto(mi_strlen("hola mundo") == 10);
	DebeSerCierto(mi_strlen("") == 0);
	DebeSerCierto(mi_strlen(cadena_1000) == LONGITUD_TEST);
	FIN_TEST("mi_strlen");
}

void test_mi_strcat()
{
	char cadena1[15] = { 'h','o','l','a', '\0' };
	char* resultado = mi_strcat(cadena1," mundo");
	INICIO_TEST("mi_strcat");
	DebeSerCierto(cadena1 == resultado);
	DebeSerCierto(mi_strlen(cadena1)==10);
	DebeSerCierto(mi_strequals(cadena1,"hola mundo"));
	DebeSerFalso(mi_strequals(cadena1,"hola"));
	FIN_TEST("mi_strcat");
}

void test_mi_strdup()
{
  INICIO_TEST("mi_strdup");
  char* str1 = "Hello";
  char* str2 = mi_strdup(str1);
  DebeSerCierto(mi_strlen(str2) == mi_strlen(str1));
  DebeSerCierto(mi_strequals(str1, str2));
  free(str2);
  char* str3 = "";
  char* str4 = mi_strdup(str3);
  DebeSerCierto(mi_strlen(str4) == mi_strlen(str3));
  DebeSerCierto(mi_strequals(str3, str4));
  free(str4);
  FIN_TEST("mi_strdup");
}

void test_mi_strcpy()
{
  INICIO_TEST("mi_strcpy");
  char dest[15];
  char* src = "hello";
  char* result = mi_strcpy(dest, src);
  DebeSerCierto(mi_strequals(dest, src));
  DebeSerCierto(result == dest);
  FIN_TEST("mi_strcpy");
}

void test_mi_strequals()
{
  INICIO_TEST("mi_strequals");
  char* str1 = "Hello";
  char* str2 = "Hello";
  char* str3 = "World";
  DebeSerCierto(mi_strequals(str1, str2));
  DebeSerFalso(mi_strequals(str1, str3));
  FIN_TEST("mi_strequals");
}

void test_mi_strcat_vacio()
{
  char cadena1[15] = { 'h','o','l','a', '\0' };
  char* resultado = mi_strcat(cadena1,"");
  INICIO_TEST("mi_strcat vacio");
  DebeSerCierto(cadena1 == resultado);
  DebeSerCierto(mi_strlen(cadena1)==4);
  DebeSerCierto(mi_strequals(cadena1,"hola"));
  FIN_TEST("mi_strcat vacio");
}

void ejecuta_tests ()
{
  test_mi_strlen();
  test_mi_strcat();
  test_mi_strdup();
  test_mi_strcpy();
  test_mi_strequals();
  test_mi_strcat_vacio();
}


int main() /*"type specifier missing" We add int because there isnt a type of return specied */
{
	puts("Iniciando tests...");
	
	ejecuta_tests();
	
	puts("Batería de test completa.");
}