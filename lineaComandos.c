#include "lineaComandos.h"
#include "ext2.h"


void menuPrincipal()
{
    char comando[200];
    char comandoTMP[200];
    int z, entroMontar=0, entroDesmontar=0, soloDisco=0;
    cont=0;

    printf("***BIEVENIDO AL MENÚ PRINCIPAL.***\n");
    printf("Escriba el comando a operar:\n");
    scanf(" %[^\n]s",&comando);

    strcpy(comandoTMP,comando);
    //pasar lo entrante a minusculas
    for(z=0;comando[z];z++)
    {
        comando[z]=tolower(comando[z]);
    }
    char *token=strtok(comando," ");

    if(strcmp(token,"mkdisk")==0) //crear discos
    {
        soloDisco=1;
        crearDiscos(comandoTMP);

    }else if(strcmp(token,"rmdisk")==0) //eliminar discos
    {
        soloDisco=1;
        eliminarDiscos(comandoTMP);

    }else if(strcmp(token,"fdisk")==0) //crear particiones
    {
        soloDisco=1;
        crearParticiones(comandoTMP);

    }else if(strcmp(token,"mkfs")==0) //formateo completo de la partición
    {
        formateoParticion(comandoTMP);//método para crear la raíz

    }else if(strcmp(token,"mount")==0) //montar el sistema EXT2 en una partición
    {
        agregarIndicesTemporales(comandoTMP);//agregarIndices
        entroMontar=1;

    }else if(strcmp(token,"unmount")==0) //desmontar el sistema EXT2 en una partición
    {
        entroDesmontar=1;
    }
    if((entroMontar==1 || entroDesmontar==1) && soloDisco==0)
    {
        if(strcmp(token, "mkdir")==0) //crea carpetas
        {
            sistemaEXT2crearCarpetas(comandoTMP);
        }
    }
    menuPrincipal();
}

void eliminarDiscos(char *comando)
{
        char direccion[50];
        int z;
        for(z=0;comando[z];z++)
        {
            comando[z]=tolower(comando[z]);
        }
        char *token=strtok(comando," ");

        while(token!=NULL)
        {
            if (strcmp(token,"-path")==0 && token!=NULL)
            {
                token=strtok(NULL, " ");
                strcpy(direccion,token);
            }
            token=strtok(NULL,"::");
        }
        struct stat st={0};

        //verificar si esta dirección existe.
        if(stat(direccion, &st)==0)
        {
            char respuesta;
            printf("Desea eliminar el disco en está dirección?.\n [y=yes / n=no] \n");
            scanf(" %c",&respuesta);

            if(respuesta=='y')
            {
                remove(direccion);
                printf("Su archivo se eliminó con éxito.\n");

            }else if(respuesta=='n')
            {
                printf("Ha elegido no eliminar el archivo.\n");
            }
        }else
        {
            printf("El disco no EXISTE.\n");
        }

}

void crearDiscos(char* comando)
{
    char size[50];
    char unidad[50];
    char direccion[50];
    char nombre[50];
    int obligatorioSize=0;
    int obligatorioPath=0;
    int obligatorioName=0;
    int z;

    strcpy(size," ");
    strcpy(unidad," ");
    strcpy(direccion," ");
    strcpy(nombre," ");


    for(z=0;comando[z];z++)
    {
        comando[z]=tolower(comando[z]);
    }

    char *token=strtok(comando," ");
    while(token!=NULL)
    {
        if(strcmp(token,"-size")==0 && token!=NULL)
        {
            token=strtok(NULL," ");
            strcpy(size,token);
            obligatorioSize=1;
        }
        else if(strcmp(token,"+unit")==0 && token!=NULL)
        {
            token=strtok(NULL," ");
            strcpy(unidad,token);
        }
        else if(strcmp(token,"-path")==0 && token!=NULL)
        {
            token=strtok(NULL," ");
            strcpy(direccion,token);
            obligatorioPath=1;
        }
        else if(strcmp(token,"-name")==0 && token!=NULL)
        {
            token=strtok(NULL," ");
            strcpy(nombre,token);
            obligatorioName=1;
        }
        token=strtok(NULL,":");
    }

    if(obligatorioSize==1 && obligatorioPath==1 && obligatorioName==1 )
    {
        crearDiscosDatos(size, unidad, direccion, nombre);
    }else
    {
        printf("Es obligatorio que venga el tamaño, el path y el nombre.\n");
        printf("Es opcional la unidad que desea agregar.\n");
        printf("NO se puede crear por estos párametros, intentélo de nuevo.\n");
    }
}

void crearDiscosDatos(char *size, char *unidad, char *direccion, char *nombre)
{
    printf("Size %s, Unit %s, Path %s, Nombre %s\n",size,unidad,direccion,nombre);
    //verificar sí la carpeta existe si no crearla.
    char direccionTMP1[100];
    char direccionTMP2[100];
    char nombreDiscoTMP[100];
    strcpy(direccionTMP1,direccion);
    strcpy(direccionTMP2,direccion);

    int z;
    for(z=0;direccionTMP2[z];z++)
    {
        direccionTMP2[z]=tolower(direccionTMP2[z]);
    }

    char *tok=strtok(direccionTMP2,"/");
    int existe=0,existeDisco=0;
    char direccionTMP[100];

    //primero el nombre del disco
    while(tok!=NULL)
    {
        strcpy(nombreDiscoTMP,tok);
        tok=strtok(NULL,"/");
    }

    char *token=strtok(direccionTMP1,"/");

    while(token!=NULL)
    {
        struct stat st = {0};
        strcat(direccionTMP,"/");
        strcat(direccionTMP,token);
        if(strcmp(nombreDiscoTMP,token)==0)
        {
            existeDisco=1;
        }
        if(stat(direccionTMP, &st)==-1 && existeDisco!=1){ //no existe se crea.
            mkdir(direccionTMP,0777); //acceso a los permisos
            existe=1;
        }
       token=strtok(NULL,"/");
    }

    if(existe==1 || existeDisco==1) //ya se creó el directorio nuevo
    {
        int tamano;
        tamano= atoi(size);

        if(tamano>=0)//el tamaño tiene que ser mayor o igual a 0 positivo
        {
            FILE *crear;

            if(strcmp(unidad," ")==0) //si es NULL entonces es MEGABYTES
            {
                //crear disco
                crear=fopen(direccion,"wb");
                fseek(crear,tamano*1024*1024, SEEK_END);
                fputc('*',crear);
                fclose(crear);

            }else if(strcmp(unidad,"k")==0) // KILOBYTES
            {
                crear=fopen(direccion,"wb");
                fseek(crear,tamano*1024, SEEK_END);
                fputc('*',crear);
                fclose(crear);

            }else if(strcmp(unidad,"m")==0) //MEGABYTES
            {
                crear=fopen(direccion,"wb");
                fseek(crear,tamano*1024*1024, SEEK_END);
                fputc('*',crear);
                fclose(crear);
            }

            //crear el mbr del disco.
            struct MBR mbr;
            struct tm fecha=*localtime(&(time_t){time(NULL)});
            int i;

            mbr.tamano=tamano;
            strcpy(mbr.fechaCreacion,asctime(&fecha));
            mbr.numeroRandom=0;
            mbr.cantidadExtendida=0;
            mbr.cantidadPrimarias=0;
            for(i=0;i<4;i++)
            {
                mbr.particion[i].estado='0'; //0=no activa, 1=activa
                mbr.particion[i].tipo='P'; //P=primaria, E=Extendida
                mbr.particion[i].ajuste='W'; //B=Best, F=First, W=Worst
                mbr.particion[i].byteInicio=0;
                mbr.particion[i].byteFinal=0;
                strcpy(mbr.particion[i].nombreParticion,"NONE");
            }
            crear=fopen(direccion, "rb+");
            fseek(crear,0L, SEEK_SET);
            fwrite(&mbr,sizeof(struct MBR),1,crear);
            fclose(crear);

        }else{
            printf("NO se puede crear el disco porque el valor es menor que 0 y negativo.\n");
        }
    }
}

void crearParticiones(char *comando)
{
    char size[50];
    char unidad[50];
    char direccion[50];
    char tipo[50];
    char ajuste[50];
    char eliminar[50];
    char name[50];
    char add[50];

    int obligatorioSize=0;
    int obligatorioPath=0;
    int obligatorioNombre=0;

    int opcionalUnidad=0;
    int opcionalTipo=0;
    int opcionalAjuste=0;
    int opcionalEliminar=0;
    int opcionalAgregar=0;

    int contVerificadorS=0, contVerificadorU=0,contVerificadorP=0, contVerificadorT=0, contVerificadorF=0,
            contVerificadorD=0, contVerificadorA=0, contVerificadorN=0;
    int verificarRepetidos=0;

    strcpy(size," ");
    strcpy(unidad," ");
    strcpy(direccion," ");
    strcpy(tipo," ");
    strcpy(ajuste," ");
    strcpy(eliminar," ");
    strcpy(name," ");
    strcpy(add," ");

    int z;
    for(z=0;comando[z];z++)
    {
        comando[z]=tolower(comando[z]);
    }
    char *token=strtok(comando," ");
    while(token!=NULL)
    {
        if(strcmp(token,"-size")==0 && token!=NULL)
        {
            token=strtok(NULL," ");
            strcpy(size,token);
            obligatorioSize=1;
            contVerificadorS++;
        }
        else if(strcmp(token,"+unit")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(unidad,token);
            opcionalUnidad=1;
            contVerificadorU++;
        }
        else if(strcmp(token,"-path")==0 && token!=NULL)
        {
            token=strtok(NULL, " ");
            strcpy(direccion,token);
            obligatorioPath=1;
            contVerificadorP++;
        }
        else if(strcmp(token,"+type")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(tipo,token);
            opcionalTipo=1;
            contVerificadorT++;
        }
        else if(strcmp(token,"+fit")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(ajuste,token);
            opcionalAjuste=1;
            contVerificadorF++;
        }
        else if(strcmp(token,"+delete")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(eliminar,token);
            opcionalEliminar=1;
            contVerificadorD++;
        }
        else if(strcmp(token, "-name")==0 && token!=NULL)
        {
            token=strtok(NULL, " ");
            strcpy(name,token);
            obligatorioNombre=1;
            contVerificadorN++;
        }
        else if(strcmp(token, "+add")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(add,token);
            opcionalAgregar=1;
            contVerificadorA++;
        }
        token=strtok(NULL,":");
    }
    if(contVerificadorA<=1 || contVerificadorD <=1 || contVerificadorF <=1 || contVerificadorN<=1 || contVerificadorP<=1
            || contVerificadorS<=1 || contVerificadorT<=1 || contVerificadorU<=1)
    {
        verificarRepetidos=1;
    }else
    {
        printf("Existen parámetros repetidos no se puede hacer ninguna accion.\n Intentelo de nuevo.\n");
    }

    if(verificarRepetidos==1)
    {
        //verificar el disco ó directorio sí existe.

        char direccionTMP1[100];
        char direccionTMP2[100];
        char nombreDiscoTMP[100];
        strcpy(direccionTMP1,direccion);
        strcpy(direccionTMP2,direccion);

        char *tok=strtok(direccionTMP2,"/");
        int existe=0,existeDisco=0;
        char direccionTMP[100];

        //primero el nombre del disco
        while(tok!=NULL)
        {
            strcpy(nombreDiscoTMP,tok);
            tok=strtok(NULL,"/");
        }

        char *token=strtok(direccionTMP1,"/");

        while(token!=NULL)
        {
            struct stat st = {0};
            strcat(direccionTMP,"/");
            strcat(direccionTMP,token);
            if(strcmp(nombreDiscoTMP,token)==0)
            {
                existeDisco=1;
            }
            printf("disco %d\n",stat(direccionTMP, &st));
            //if(stat(direccionTMP, &st)==-1 && existeDisco==1){ //verificar si el directorio existe
            if(fopen(direccionTMP,"r") != NULL && existeDisco==1){
                existe=1;
            }
           token=strtok(NULL,"/");
        }
        if(existeDisco==1 && existe==1)
        {

            if(obligatorioSize==1 && obligatorioPath==1 && obligatorioNombre==1)
            {
                verificarTamanoParticiones(size, unidad, direccion,tipo, ajuste, eliminar,name,add);

            }else
            {
                printf("Es obligatorio que venga el tamaño, el path y nombre.\n");
                printf("Es opcional la unidad,tipo,ajuste,eliminar,agregar.\n");
                printf("NO se puede crear por está restricción de párametros, intentélo de nuevo.\n");
            }
        }else{
                printf("El disco ó el directorio NO existe.\n");
        }
    }

}

void crearParticionesAgregar(char *add, char *nombre, char *direccion, char *unidad)
{
    //abrir el disco y leer el MBR
    struct MBR mbrDisco;
    FILE *archivoMBR1=fopen(direccion,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);
    //recorrer el arreglo de particiones y verificar cúal no está activa
    int existeParticion=0;
    int k;

    for(k=0;k<4;k++)
    {
        if(strcmp(nombre,mbrDisco.particion[k].nombreParticion)==0) //si el nombre de la partición es igual a la entrante.
        {
            existeParticion=1;
            break;
        }
    }
    if(existeParticion==1) //tiene que existir el nombre de la particion
    {
        int tamano;
        tamano=atoi(add);
        if(tamano<=0) //reducir el espacio en la partición.
        {

        }else if(tamano>=0)//aumentar el espacio en la partición.
        {

        }
    }
}

void crearParticionesEliminar(char *delet, char *nombre, char *direccion)
{
    //abrir el disco y leer el MBR
    struct MBR mbrDisco;
    FILE *archivoMBR1=fopen(direccion,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);
    //recorrer el arreglo de particiones y verificar cúal no está activa
    int existeParticion=0;
    int k;
    for(k=0;k<4;k++)
    {
        if(strcmp(nombre,mbrDisco.particion[k].nombreParticion)==0) //si el nombre de la partición es igual a la entrante.
        {
            existeParticion=1;
            break;
        }
    }
    if(strcmp(delet,"full")==0 && existeParticion==1) //buscar en la tabla de particiones y rellenar la partición con \0
    {

    }else if(strcmp(delet,"fast")==0 && existeParticion==1) //buscar en la tabla de particiones y cambiar el estado a 0=eliminada
    {

    }else
    {
        printf("La partición a eliminar no existe ó escribió mal el nombre de la misma.\n");
        printf("Intentélo de nuevo.\n");
    }
}

void verificarTamanoParticiones(char *size, char *unidad, char *direccion, char *tipo, char *ajuste,
                           char *delet, char *name, char *add)
{
        int tamano;
        tamano= atoi(size);

        if(tamano>=0)//el tamaño tiene que ser mayor o igual a 0 positivo
        {
            if(strcmp(unidad," ")==0) //si es NULL entonces son KILOBYTES
            {
                crearParticionesDISCO(tamano*1024,direccion,tipo,ajuste,delet,name,add);

            }else if(strcmp(unidad,"k")==0) // KILOBYTES
            {
                crearParticionesDISCO(tamano*1024,direccion,tipo,ajuste,delet,name,add);

            }else if(strcmp(unidad,"m")==0) //MEGABYTES
            {
                crearParticionesDISCO(tamano*1024*1024,direccion,tipo,ajuste,delet,name,add);

            }else if(strcmp(unidad,"b")==0) //BYTES
            {
                crearParticionesDISCO(tamano/1024,direccion,tipo,ajuste,delet,name,add);

            }
        }else{
            printf("NO se puede crear el disco porque el valor es menor que 0 y negativo.\n");
        }
}

void crearParticionesDISCO(int tamano, char *direccion, char *tipo, char *ajuste, char *delet, char *name, char *add)
{
    //abrir el disco y leer el MBR
    struct MBR mbrDisco;
    FILE *archivoMBR1=fopen(direccion,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);
    //recorrer el arreglo de particiones y verificar cúal no está activa
    int existeParticion=0;
    int k;
    for(k=0;k<4;k++) //sólo para verificar las particiones primarias y extendida.
    {
        if(strcmp(name,mbrDisco.particion[k].nombreParticion)==0 && mbrDisco.particion[k].estado==1) //si el nombre de la partición es igual a la entrante.
        {
            existeParticion=1;
            break;
        }
    }

        int existe=0;

        if(strcmp(ajuste," ")==0) //si es vacío entonces es peor ajuste
        {
            strcpy(ajuste,"W");
            existe=1;
        }else if(strcmp(ajuste,"B") || strcmp(ajuste,"F") || strcmp(ajuste,"W"))
        {
            existe=1;

        }else
        {
            printf("Este ajuste NO existe ó está mal escrito.");
        }

        if(existeParticion!=1 && existe==1) //el nombre de la partición no sea existente.
        {
           // struct MBR mbr;
            int i;
            struct tm fecha=*localtime(&(time_t){time(NULL)});

           /* FILE *archivoMBR=fopen(direccion,"rb+");
            fseek(archivoMBR,0L, SEEK_SET);
            fread(&mbr,sizeof(struct MBR),1,archivoMBR);
            fclose(archivoMBR);
            */
            if((mbrDisco.cantidadExtendida+mbrDisco.cantidadPrimarias)<=4) //la suma de las dos sean menores que 4 ó igual
            {
                if(mbrDisco.cantidadExtendida==0 && mbrDisco.cantidadPrimarias<=3) //se puede crear una extendida y primarias
                {
            //verificar que tipo de partición es
                    if(strcmp(tipo,"E")==0) //si es extendida
                    {

                    }
                    else if(strcmp(tipo,"P")==0 || strcmp(tipo," ")==0) //si es primaria
                    {
                        //creamos el espacio en la partición como primaria.
                        //1. modificar el mbr del disco actualizando los datos.
                        mbrDisco.cantidadPrimarias=mbrDisco.cantidadPrimarias+1;
                        strcpy(mbrDisco.fechaCreacion,asctime(&fecha));
                        mbrDisco.numeroRandom=mbrDisco.numeroRandom+1;
                        for(i=0;i<4;i++) //verificar una partición vacía.
                        {
                            if(mbrDisco.particion[0].estado=='0') //el estado de la partición no esté activa.
                            {
                                mbrDisco.particion[i].ajuste=ajuste[0];
                                mbrDisco.particion[i].byteFinal=tamano; //tamaño total de la partición.
                                mbrDisco.particion[i].byteInicio=sizeof(struct MBR);
                                mbrDisco.particion[i].estado=1;
                                strcpy(mbrDisco.particion[i].nombreParticion,name);
                                mbrDisco.particion[i].tipo='P';
                                break;

                            }else if(mbrDisco.particion[i].estado=='0')
                            {
                                mbrDisco.particion[i].ajuste=ajuste[0];
                                mbrDisco.particion[i].byteFinal=tamano; //tamaño total de la partición.
                                mbrDisco.particion[i].byteInicio=mbrDisco.particion[i-1].byteFinal;
                                mbrDisco.particion[i].estado=1;
                                strcpy(mbrDisco.particion[i].nombreParticion,name);
                                mbrDisco.particion[i].tipo='P';
                                break;
                            }
                        }
                        FILE *actualizarArchivo=fopen(direccion,"rb+");
                        fseek(actualizarArchivo, 0L, SEEK_SET);
                        fwrite(&mbrDisco,sizeof(struct MBR),1,actualizarArchivo);
                        fclose(actualizarArchivo);

                    }else
                    {
                        printf("Este tipo de partición no existe.\n");
                    }

                }else if(mbrDisco.cantidadExtendida==1 && mbrDisco.cantidadPrimarias<=3) //ya no se puede crear una extendida
                {
                    //si existe una extendida verificar el nombre de las lógicas.

                    if(strcmp(tipo,"L")==0) //si es lógica
                    {

                    }
                    else if(strcmp(tipo,"P")==0 || strcmp(tipo," ")==0) // si es primaria
                    {

                    }else
                    {
                        printf("Este tipo de partición no existe.\n");
                    }
                }else if(mbrDisco.cantidadExtendida==0 && strcmp(tipo,"L")==0)
                {
                    printf("NO se puede crear una lógica si no está creada una extendida.\n");
                }
            }
        }
}
