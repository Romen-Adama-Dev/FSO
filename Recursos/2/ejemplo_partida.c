#include<stdio.h>
#include<stdlib.h>

//Puntero global que apuntara al vector
int *asientos;
int MAX;

//Reserva memoria para el vector
void Crea_Vector(void);

// Inicializa los elementos del vector a -1
void Inicializa_Vector(void);

// Imprime la ocupación del cine
void Imprime_Vector(void);

// Modifica un elemento del vector devolviendo 0 si toda ha ido bien o -1 en caso de error
int Modifica_Vector(int pos, int valor);

void Crea_Vector(void)
{
printf("Introduce el tamaño del vector:");
scanf("%d",&MAX);
asientos=(int*)malloc(MAX*sizeof(int));
if (asientos==NULL) {
	printf("Error crítico al reservar memoria");
	exit(-1);
}
}

void Inicializa_Vector(void)
{
int cont;

for(cont=0;cont<MAX;cont++)
	*(asientos+cont)=-1;
}

int Modifica_Vector(int pos, int valor)
{
if (pos>=0 && pos<MAX) {
	*(asientos+pos)=valor;
	return 0;
}
else return -1;
}

void Imprime_Vector()
{
int cont;

for(cont=0;cont<MAX;cont++)
	printf("asientos[%d]=%d\n",cont,*(asientos+cont));
}

main()
{

Crea_Vector();
Inicializa_Vector();
Imprime_Vector();
Modifica_Vector(0,5);
Modifica_Vector(1,6);
Imprime_Vector();
}
