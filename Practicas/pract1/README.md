# Sala API
This is an API for managing seats in a room. The API provides functions for creating a room with a given capacity, reserving and releasing seats, and getting information about the room and the seats.

## File Structure
The sala.h header file contains function declarations and constant definitions.
The sala.c source file contains function implementations.

## Functions
- `crea_sala(int capacidad)`
This function creates a room with the specified capacity. It allocates memory for the room and initializes all the seats as available.

- `elimina_sala()`
This function deletes the room and frees the memory allocated for it. It also releases all the seats that are reserved.

- `capacidad()`
This function returns the capacity of the room.

- `asientos_libres()`
This function returns the number of available seats in the room.

- `asientos_ocupados()`
This function returns the number of reserved seats in the room.

- `reserva_asiento(int id)`
This function reserves a seat in the room with the specified ID. If the seat is available, it is reserved and the function returns 0. If the seat is already reserved or the room is full, the function returns an error code.

- `libera_asiento(int asiento)`
This function releases the seat with the specified ID. If the seat is reserved, it is released and the function returns 0. If the seat is not reserved, the function returns an error code.

- `estado_asiento(int asiento)`
This function returns the status of the seat with the specified ID. If the seat is available, it returns 0. If the seat is reserved, it returns the ID of the client who reserved it.

## Error codes
The API returns the following error codes:
- `ERR_CAPACIDAD_INVALIDA`: the room capacity is not valid
- `ERR_ID_CLIENTE_YA_EXISTE`: the client ID already exists
- `ERR_SIN_ASIENTOS_LIBRES`: there are no available seats in the room
- `ERR_ASIENTO_OCUPADO`: the seat is already reserved
- `ERR_ASIENTO_NO_VALIDO`: the seat ID is not valid
- `ERR_MEMORIA_ASIGNADA`: there was an error allocating memory
- `ERR_ASIENTO_YA_LIBRE`: the seat is already available
- `ERR_SALA_NO_EXISTE`: the room does not exist

## Author
This API was created by Romen Adama Caetano Ramirez on March 31, 2023. The API is licensed under the MIT License. See the LICENSE file for details.