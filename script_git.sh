#!/bin/bash

# Obtener la fecha y hora actuales
fecha=$(date "+%Y-%m-%d")
hora=$(date "+%H:%M:%S")

# Obtener los nombres de los archivos modificados
archivos_modificados=$(git status --porcelain | awk '{print $2}' | tr '\n' ' ')

# Verificar si hay archivos modificados para continuar
if [ -z "$archivos_modificados" ]; then
    echo "No hay archivos modificados para hacer commit."
    exit 0
fi

# Agrega todos los archivos al área de staging de Git
git add .

# Realiza un commit con los cambios incluyendo la fecha, hora y archivos modificados
git commit -m "Actualización automática del repositorio fecha $fecha hora $hora de los archivos $archivos_modificados"

# Sube los cambios al repositorio remoto en la rama correspondiente
git push origin main
