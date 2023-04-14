#!/bin/bash

gcc -c lib/salas/sala.c -o lib/salas/sala.o

ar -crs lib/libsala.a lib/salas/sala.o

gcc fuentes/test_sala.c -lsala -Llib -o fuentes/test_sala

fuentes/test_sala