#!/bin/bash

# Función para calcular el tiempo medio de espera
calcular_tiempo_medio() {
  local tiempos=("$@")
  local espera=0
  local total_espera=0
  local n=${#tiempos[@]}

  for ((i = 1; i < n; i++)); do
    espera=$((espera + tiempos[i-1]))
    total_espera=$((total_espera + espera))
  done

  echo "scale=2; $total_espera / $n" | bc
}

# Tiempos de atención para cada escenario
tiempos_fcfs=(10 12 20 8 1)
tiempos_esteban_adelante=(1 10 12 20 8)
tiempos_sjf=(1 8 10 12 20)

# Cálculo de los tiempos medios de espera
tiempo_medio_fcfs=$(calcular_tiempo_medio "${tiempos_fcfs[@]}")
tiempo_medio_esteban_adelante=$(calcular_tiempo_medio "${tiempos_esteban_adelante[@]}")
tiempo_medio_sjf=$(calcular_tiempo_medio "${tiempos_sjf[@]}")

# Mostrar los resultados
echo "Tiempo medio de espera FCFS: $tiempo_medio_fcfs minutos"
echo "Tiempo medio de espera con Esteban adelantado: $tiempo_medio_esteban_adelante minutos"
echo "Tiempo medio de espera SJF: $tiempo_medio_sjf minutos"
