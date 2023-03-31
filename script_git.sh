#!/bin/bash

# Agrega todos los archivos al área de staging de Git
git add .

# Realiza un commit con los cambios
git commit -m "Actualización automática del repositorio"

# Sube los cambios al repositorio remoto
git push origin main
