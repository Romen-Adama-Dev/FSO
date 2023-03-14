//Conocer la rama
git branch
//Añadir los cambios
git add .
//Comentario
git commit -m "Modificacion del archivo sala.h debido a que la definicion de la estructura debia estar en la API y no en la cabecera, ademas como guia del profesor, comentamos los prints de la API, y evitamos redundancias de testeo a la hora de realizar los test"
//Cambios remotos y fusionarlos
git pull origin main
//Actualizacion del repositorio
git push origin main
