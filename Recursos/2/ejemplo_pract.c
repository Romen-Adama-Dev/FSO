#include <unistd.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int comprueba_error()
{
    if (errno !=0)
    {
        fprintf(stderr, "Hubo un error con codigo %d : %s", errno, strerror(errno));
        fflush(stderr);
        errno=0;
        return -1;

    }

    return 0;

}


int informacion(char *fichero)
{
    struct stat info;

    if (stat(fichero, &info)==-1)
    {
        comprueba_error();
        return -1;
    }

    printf("Nombre de fichero: %s \n", fichero);

    printf("Tamano del fichero: %ld \n", info.st_size);

    printf( (S_ISDIR(info.st_mode)) ? "d" : "-");

    printf((info.st_mode & S_IRUSR) ? "r" : "-");
    printf((info.st_mode & S_IWUSR) ? "w" : "-");
    printf((info.st_mode & S_IXUSR) ? "x" : "-");

    printf((info.st_mode & S_IRGRP) ? "r" : "-");
    printf((info.st_mode & S_IWGRP) ? "w" : "-");
    printf((info.st_mode & S_IXGRP) ? "x" : "-");

    printf((info.st_mode & S_IROTH) ? "r" : "-");
    printf((info.st_mode & S_IWOTH) ? "w" : "-");

    printf((info.st_mode & S_IXOTH) ? "x" : "-");

    printf("\n");

    return 0;


}

int sobreescribir_fichero(char *nombrefichero)
{



    int fid = open(nombrefichero, O_RDWR);
    if (fid == -1)
    {
        comprueba_error();
        return -1;
    }


    char caracter;
    while (1)
    {
        ssize_t bytes_leidos = read(fid,&caracter, sizeof(char));
        comprueba_error();
        if (bytes_leidos == 0)
        {
            break;
        }

        if ((caracter >= 65) && (caracter <= 90))
        {
            caracter = caracter + 32;
        }
        else if ((caracter >= 'a') && (caracter <= 'z'))
        {
            caracter = caracter - 32;
        }

        lseek(fid,-1*sizeof(char), SEEK_CUR);
        ssize_t bytes_escritos = write(fid, &caracter, sizeof(char));
        comprueba_error();
        if (bytes_escritos == 0)
        {
            break;
        }

    }

    if (close(fid)==-1)
    {
        comprueba_error();
        return -1;
    }

    return 0;





}

int transformar(char *nombre_entrada,char *nombre_salida )
{
    int salida = 0;

    int fid_entrada = open(nombre_entrada, O_RDONLY);
    if (fid_entrada == -1)
    {
        comprueba_error();
        salida = -1;
    }

    int fid_salida = open(nombre_salida, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fid_salida == -1)
    {


        comprueba_error();

        if (close(fid_entrada) ==-1)
        {
            comprueba_error();
        }

        salida = -1;
    }

    if (salida == 0)
    {
        char caracter;
        while (1)
        {
            ssize_t bytes_leidos = read(fid_entrada,&caracter, sizeof(char));
            comprueba_error();
            if (bytes_leidos == 0)
            {
                break;
            }

            if ((caracter >= 65) && (caracter <= 90))
            {
                caracter = caracter + 32;
            }
            else if ((caracter >= 'a') && (caracter <= 'z'))
            {
                caracter = caracter - 32;
            }


            ssize_t bytes_escritos = write(fid_salida, &caracter, sizeof(char));
            comprueba_error();
            if (bytes_escritos == 0)
            {
                break;
            }



        }


    }


    if (close(fid_salida)==-1)
    {
        comprueba_error();
        salida = -1;
    }

    if (close(fid_entrada) ==-1)
    {
        comprueba_error();
        salida = -1;
    }

    return salida;
}

int main(int argc, char *argv[])
{

    if (!((argc==5) || (argc==3)))
    {
        printf("Error catastrofico en parametros!");
        return -1;
    }

    char caracter;

    int flag_i = 0;
    int flag_o = 0;
    int flag_t = 0;
    int flag_s = 0;

    char nombre_entrada[32];
    char nombre_salida[32];
    char nombre[32];


    while ((caracter=getopt(argc, argv, ":i:o:t:s:"))!=-1)
    {

        switch (caracter)
        {
        case 'i':
            flag_i=1;
            strcpy(nombre_entrada, optarg);

            break;

        case 'o':
            flag_o=1;
            strcpy(nombre_salida, optarg);
            break;

        case 't':
            flag_t=1;
            strcpy(nombre, optarg);
            break;

        case 's':
            flag_s=1;
            strcpy(nombre, optarg);
            break;

        case ':':
            printf("Falta argumento! : %c \n", optopt);
            break;

        case '?':
            printf("Opcion inesperada! : %c \n", optopt);
            break;

        default:
            break;



        }

    }

    if ((flag_i==1) && (flag_o==1))
    {
        transformar(nombre_entrada, nombre_salida);
    }
    else if (flag_t==1)
    {
        sobreescribir_fichero(nombre);
    }
    else if (flag_s==1)
    {
        informacion(nombre);
    }
    else
    {
        printf("Ninguna opcion valida. \n");
    }






    return 0;
}
