## README - Implementación Thread-Safe de API de Gestión de Asientos

Este repositorio contiene el código fuente y los archivos necesarios para la implementación de una API de gestión de asientos thread-safe. La API permite realizar reservas y liberaciones de asientos en una sala de forma concurrente, evitando cualquier inconsistencia en los datos debido al acceso concurrente por parte de múltiples hilos.

El proyecto se divide en tres hitos, cada uno abordando diferentes aspectos y mejoras en la implementación de la API. A continuación se detalla cada hito y los pasos necesarios para ejecutar y probar el código.

### Hito 1 - Problema de la Sección Crítica

En este hito, se debe evidenciar el problema de la sección crítica en la implementación actual de la API, donde múltiples hilos acceden concurrentemente a la estructura de datos que guarda el estado de los asientos de la sala.

#### Ejecución
```
multihilos n
```
Donde `n` es un entero positivo que indica el número de hilos a lanzar. Cada hilo realizará 3 reservas y a continuación liberará los 3 asientos reservados. Además, se lanzará un hilo adicional para mostrar el estado de la sala cada cierto tiempo.

#### Resultado Esperado
La ejecución de este programa sin la implementación thread-safe de la API debería manifestar problemas en la consistencia de los datos. Por ejemplo, se podrían realizar múltiples reservas del mismo asiento o producirse errores cuando un hilo intenta liberar un asiento que no está ocupado.

### Hito 2 - API Thread-Safe

En este hito, se debe modificar la implementación de la API para que sea thread-safe, es decir, garantizar que no se generen inconsistencias en los datos debido al acceso concurrente por parte de múltiples hilos.

#### Modificaciones
Para lograr una implementación thread-safe, se emplearán mutex para controlar el acceso a la estructura de datos que guarda el estado de los asientos. Se realizarán las modificaciones necesarias en la API para garantizar la exclusión mutua durante las operaciones de reserva y liberación de asientos.

#### Ejecución
```
multihilos n
```
Se debe ejecutar nuevamente el programa multihilos con el mismo comando utilizado en el Hito 1.

#### Resultado Esperado
Después de aplicar las modificaciones thread-safe a la API, la ejecución del programa multihilos debería ser consistente y no se deberían observar las inconsistencias previamente manifestadas en el Hito 1.

### Hito 3 - Espera Condicional

En este hito, se modificará el programa para que las acciones de reserva o liberación de asientos queden en espera si un hilo no puede ejecutarlas hasta que se den las circunstancias adecuadas.

#### Modificaciones
Se utilizarán mutex y variables de condición para implementar la espera condicional en los hilos. Si un hilo intenta realizar una reserva y no hay asientos disponibles, deberá esperar hasta que se libere un asiento. Del mismo modo, si un hilo intenta liberar un asiento y no hay asientos ocupados, deberá esperar hasta que exista una reserva que pueda ser liberada.

#### Ejecución
```
multihilos n m
```
Donde `n` es el número de hilos positivo que realizarán reservas de asientos y `m` es el número de hilos positivo que se encargarán de realizar liberaciones de asientos. Cada hilo de reserva realizará 3 reservas, introduciendo un retardo aleatorio entre cada reserva. Por otro lado, cada hilo de liberación de asientos liberará 3 asientos, también con retardo aleatorio entre cada liberación. Además, se lanzará un hilo adicional para mostrar el estado de la sala cada cierto tiempo.

#### Resultado Esperado
Después de implementar la espera condicional utilizando mutex y variables de condición, se espera que los hilos se comporten correctamente, esperando cuando sea necesario y realizando las operaciones de reserva y liberación de asientos de manera consistente. No deberían ocurrir las inconsistencias observadas en los hitos anteriores.

### Entregables

Cada hito requiere entregar lo siguiente:

- Código desarrollado para cada hito.
- Test con su correspondiente traza, como capturas de pantalla, para demostrar los resultados observados y evidenciar las inconsistencias antes de aplicar las mejoras y después de aplicarlas.
- Comentarios explicando los resultados observados y resaltando que después de las modificaciones pertinentes, los resultados son adecuados y las inconsistencias anteriores han sido solucionadas.

**Nota:** Asegúrate de proporcionar la información y capturas de pantalla necesarias para respaldar tus comentarios sobre los resultados observados antes y después de cada hito.

### Requisitos del Sistema

- Compilador de C compatible con las librerías pthread.
- Biblioteca de retardo (retardo.c/h) proporcionada en el material de la práctica.