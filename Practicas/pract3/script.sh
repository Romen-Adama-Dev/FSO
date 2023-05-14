#!/bin/bash

# Compila el programa hito1
cd pract3_hito1
gcc -o hito1 multihilos.c sala.c retardo.c -lpthread
cd ..

# Compila el programa hito 2
cd pract3_hito2
gcc -o hito2 multihilos.c sala.c retardo.c -lpthread
cd ..

# Compila el programa hito 3
cd pract3_hito3
gcc -o hito3 multihilos.c sala.c retardo.c -lpthread
cd ..

# Ejecuta el programa
#./hito1 n
#./hito2 n
#./hito3 n m