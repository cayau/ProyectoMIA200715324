#include "ext2.h"

void formateoParticion(char *comando)
{
    char path[50];
    char type[50];
    char name[50];
    char add[50];
    char unit[50];

    strcpy(path," ");
    strcpy(type," ");
    strcpy(name," ");
    strcpy(add," ");
    strcpy(unit," ");


    int z, pathObligatorio=0, nombreObligatorio=0, tipoOpcional=0, agregarOpcional=0, unidadOpcional=0;
    for(z=0;comando[z];z++) //todas las letras se convierten en minúsculas
    {
        comando[z]=tolower(comando[z]);
    }
    char *token=strtok(comando," ");

    while(token!=NULL)
    {
        if (strcmp(token,"-path")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL, " ");
            strcpy(path,token);
            pathObligatorio=1;

        }else if(strcmp(token,"-type")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL," ");
            strcpy(type,token);
            tipoOpcional=1;

        }else if(strcmp(token,"-name")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL," ");
            strcpy(name,token);
            nombreObligatorio=1;

        }else if(strcmp(token, "-add")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(add, token);
            agregarOpcional=1;

        }else if(strcmp(token, "-unit")==0 && token!=NULL) //opcional
        {
            token=strtok(NULL, " ");
            strcpy(unit, token);
            unidadOpcional=1;
        }

        token=strtok(NULL,"=");
    }


    if(pathObligatorio==1 && nombreObligatorio==1)// path es obligatorio tiene que venir.
    {
        //verificar sí es agregar y que venga unidad ó puede que no venga.
        if(agregarOpcional==1 && (unidadOpcional==1 || unidadOpcional==0))
        {
            agregarQuitarEspacioParticion(path, name, add, unit);
        }
        else if(strcmp(type," ")==0) //si no viene nada es porque es formateo FULL completo
        {
            formateoCompleto(path,name);

        }else if(strcmp(type,"full")==0)
        {
            formateoCompleto(path,name);
        }else if(strcmp(type,"fast")==0)
        {
            formateoRapido(path,name);
        }
    }else
    {
        printf("La dirección es obligatoria y el nombre de la partición.\n");
        printf("Es opcional el tipo, agregar ó reducir, unidad.\n");
        printf("INTENTELO DE NUEVO.\n");
    }
}

void agregarQuitarEspacioParticion(char *path, char *name, char *add, char *unit)
{
    struct MBR mbrDisco;
    FILE *archivoMBR1=fopen(path,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);

    int i;
    int tamano,existeParticion=0;
    tamano=atoi(add);

    for(i=0;i<4;i++)
    {
        if(strcpy(mbrDisco.particion[i].nombreParticion,name)==0)
        {
            existeParticion=1;
            break;
        }
    }
    if(existeParticion==1)
    {
        //verificar sí viene Unidad.
        if(strcmp(unit," ")==0) //si no viene nada se utilizarán kilobytes
        {
            tamano=tamano/1024;

        }else if(strcmp(unit,"B")==0)
        {
            tamano=tamano/1024;

        }else if(strcmp(unit,"K")==0)
        {
            tamano=tamano*1024;

        }else if(strcmp(unit,"M")==0)
        {
            tamano=tamano*1024*1024;
        }

        //verificar sí el tamaño es menor ó mayor
        if(tamano<=0)
        {//reducir

        }else if(tamano>=0)
        {//aumentar el disco y verificar el tamaño final del disco como también si existe una partición a la par.

        }

    }else{
        printf("El nombre de esta partición no existe.\n");
    }

}

void formateoRapido(char *direccion, char* name)
{
    struct MBR mbrDisco;
    int existeParticion;
    FILE *archivoMBR1=fopen(direccion,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);

    int i;
    for(i=0; i<4 ;i++)
    {
        if(strcmp(mbrDisco.particion[i].nombreParticion,name)==0) //nombre de la partición es igual
        {
            //mbrDisco.particion[i].estado=0;//cambiar sólo el estado de esta partición.
            existeParticion=1;
            break;
        }
    }

    if(existeParticion==1)
    {
        FILE *archivoMBR=fopen(direccion,"rb+");
        fseek(archivoMBR,0L, SEEK_SET);
        fwrite(&mbrDisco,sizeof(struct MBR),1,archivoMBR);
        fclose(archivoMBR);

        printf("Se ha realizado un formateo rápido.\n");
        sistemaEXT2(direccion, name);//crear la raíz de EXT2

    }else
    {
        printf("Está partición no existe ó se ha realizado un formateo rápido anteriormente.\n");
    }

}

void formateoCompleto(char *direccion, char *name)
{

    struct MBR mbrDisco;
    int existeParticion;
    FILE *archivoMBR1=fopen(direccion,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);

    int i,j;
    FILE *archivoMBR=fopen(direccion,"rb+");

    for(i=0; i<4 ;i++)
    {
        if(strcpy(mbrDisco.particion[i].nombreParticion,name)==0 && mbrDisco.particion[i].estado==1) //nombre de la partición es igual
        {
            mbrDisco.particion[i].estado=0;//cambiar sólo el estado de esta partición.
            existeParticion=1;
            for(j=mbrDisco.particion[i].byteInicio;j<mbrDisco.particion[i].byteFinal;j++)
            {
                fseek(archivoMBR,j, SEEK_SET); //cada vez cambia de posición.
                fputc("\0",archivoMBR); //se rellena todo el espacio de la partición en \0
            }
            break;
        }
    }
    fclose(archivoMBR);

    if(existeParticion==1)
    {

        printf("Se ha realizado un formateo rápido.\n");
        //crear la raíz de EXT2

    }else
    {
        printf("Está partición no existe ó se ha realizado un formateo rápido ó completo anteriormente.\n");
    }


}

void sistemaEXT2(char *path, char *name)
{
    //hacer las particiones del ext2
    //crear las particiones del ext2

    //1.abrir el mbr
    //2.recorrer las particiones que contiene el disco
    //3.verificar el nombre de la particion entrante con la que esta almacenada
   struct MBR mbrDisco;

    int tamanoInicial=0, tamanoFinal=0, tamanoInodo=0, tamanoBloque=0;

    FILE *archivoMBR1=fopen(path,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);

    int i, verificarParticion=0,j,k;
    for(i=0;i<4;i++)
    {
        if(strcmp(mbrDisco.particion[i].nombreParticion,name)==0)
        {
           verificarParticion=1;
           tamanoInicial=mbrDisco.particion[i].byteInicio; //tamaño inicial de la partición.
           tamanoFinal=mbrDisco.particion[i].byteFinal; //tamaño final de la partición.
           break;
        }
    }
    //calcular cúantos INODOS
    tamanoInodo=(tamanoFinal-sizeof(struct SUPERBLOQUE)/4+sizeof(struct INODOS)+(3*sizeof(struct BLOQUES)));
    tamanoBloque=3*tamanoInodo;
    //calcular cúantos BLOQUES

    if(verificarParticion==1)
    {
        FILE *archivoSB=fopen(path,"rb+");
        fseek(archivoSB, tamanoInicial,SEEK_SET);
        struct tm f=*localtime(&(time_t){time(NULL)});

        struct INODOS inodos[tamanoInodo];
        struct bitmapINODOS bitmapInodo[tamanoInodo];
        struct bitmapBLOQUES bitmapBloque[tamanoBloque];
        struct BLOQUES bloques[tamanoBloque];
        struct SUPERBLOQUE superBloque;


        /**************SUPERBLOQUE*********************/
        //strcpy(superBloque.s_fechaDesmontaje,asctime(&f));
        //strcpy(superBloque.s_fechaMontaje,asctime(&f));
        superBloque.s_bitmapBloques=superBloque.s_bitmapBloques+1;
        superBloque.s_bitmapInodos=superBloque.s_bitmapInodos+1;
        superBloque.s_bloquesLibres=tamanoBloque-1;
        superBloque.s_contadorMontaje=superBloque.s_contadorMontaje+1;
        superBloque.s_inicioBloques=superBloque.s_inicioBloques+1;
        superBloque.s_inicioInodos=superBloque.s_inicioInodos+1;
        superBloque.s_inodosLibres=tamanoInodo-1;
        superBloque.s_magico=superBloque.s_magico+1;
        superBloque.s_primerBloqueLibre=1;
        superBloque.s_primerInodoLibre=1;
        superBloque.s_tamanoBloque=sizeof(struct BLOQUES);
        superBloque.s_tamanoInodo=sizeof(struct INODOS);
        superBloque.s_totalBloques=tamanoBloque;
        superBloque.s_totalInodos=tamanoInodo;


        fwrite(&superBloque,sizeof(superBloque),1,archivoSB);
        fclose(archivoSB);

        /***************BITMAP DE INODOS*******************/
        for(j=0;j<tamanoInodo;j++)
        {
            if(bitmapInodo[0].val=='0') //está es la raíz.
            {
                bitmapInodo[j].val='1';
            }else
            {
                bitmapInodo[j].val='0';
            }
        }

        FILE *archivoBI=fopen(path,"rb+");
        fseek(archivoBI, tamanoInicial+sizeof(struct SUPERBLOQUE),SEEK_SET);
        fwrite(&bitmapInodo, sizeof(struct bitmapINODOS),1,archivoBI);
        fclose(archivoBI);

        /******************BITMAP DE BLOQUES****************************/
        for(k=0;k<tamanoBloque;k++)
        {
            if(bitmapBloque[0].val=='0') //está es la raíz.
            {
                bitmapBloque[k].val='1';
            }else
            {
                bitmapBloque[k].val='0';
            }
        }
        FILE *archivoBB=fopen(path,"rb+");
        fseek(archivoBB, tamanoInicial+sizeof(struct SUPERBLOQUE)+sizeof(bitmapInodo),SEEK_SET);
        fwrite(&bitmapBloque, sizeof(struct bitmapBLOQUES),1,archivoBB);
        fclose(archivoBB);

        /****************************INODOS********************************/
        j=0;
        for(j=0;j<tamanoInodo;j++)
        {
            inodos[j].i_block[0]=0; //primer apuntador directo apunta a un bloque
            strcpy(inodos[j].i_inicioFecha,asctime(&f));
            inodos[j].i_tamano=0;
            inodos[j].i_tipo=0; //0=Carpeta, 1=Archivo
            inodos[j].i_uid=201020946;
            strcpy(inodos[j].i_ultimaFecha,asctime(&f));
            strcpy(inodos[j].i_ultimaFechaModificacion,asctime(&f));
        }
        FILE *archivoI=fopen(path,"rb+");
        fseek(archivoI, tamanoInicial+sizeof(struct SUPERBLOQUE)+sizeof(bitmapInodo)+sizeof(bitmapBloque),SEEK_SET);
        fwrite(&inodos, sizeof(struct INODOS),1,archivoI);
        fclose(archivoI);

        /*****************************BLOQUES***********************************/
        k=0;
        for(k=0;k<tamanoBloque;k++)
        {
            if(k==0)
            {
                bloques[k].tipo2.contenido[0].b_inodo=0; //apuntar el mismo.
                bloques[k].tipo2.contenido[1].b_inodo=0; //apuntar al padre.
                strcpy(bloques[k].tipo2.contenido[0].b_nombre,"raiz");

            }else{
                bloques[k].tipo1;
                bloques[k].tipo2;
                bloques[k].tipo3;
            }
        }
        FILE *archivoB=fopen(path,"rb+");
        fseek(archivoB, tamanoInicial+sizeof(struct SUPERBLOQUE)+sizeof(bitmapInodo)+sizeof(bitmapBloque)+sizeof(inodos),SEEK_SET);
        fwrite(&bloques, sizeof(struct BLOQUES),1,archivoB);
        fclose(archivoB);


    }


}

void sistemaEXT2crearCarpetas(char *comando)
{
    char id[50];
    char path[50];
    char p[50];
    int z, pathObligatorio=0, idObligatorio=0;

    strcpy(id, " ");
    strcpy(path, " ");
    strcpy(p, " ");

    for(z=0;comando[z];z++) //todas las letras se convierten en minúsculas
    {
        comando[z]=tolower(comando[z]);
    }
    char *token=strtok(comando," ");

    while(token!=NULL)
    {
        if (strcmp(token,"-path")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL, " ");
            strcpy(path,token);
            pathObligatorio=1;

        }else if(strcmp(token,"-id")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL, " ");
            strcpy(id, token);
            idObligatorio=1;

        }else if(strcmp(token, "-p")==0 && token!=NULL)
        {
            token=strtok(NULL, " ");
            strcpy(id, token);
        }
    }

    if(pathObligatorio==1 && idObligatorio==1)
    {
       //verificar en el archivo temporal si existe este -id de la partición y si coincide con el path
    }

}
void crearCARPETAS(char *path, char *id, char *p)
{
    //abrir el mbr
    struct MBR mbrDisco;
    FILE *archivoMBR1=fopen(path,"rb+");
    fseek(archivoMBR1,0L, SEEK_SET);
    fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
    fclose(archivoMBR1);

    //verificar que particion está en ese ID y compararla con las particiones con el mbr
    int z;
    for(z=0;z<4;z++)
    {

    }
    //si existe entra a crear la carpeta.

    //abrir superBloque, bitmap Inodos, bitmap Bloques, Inodos, bloques
    //modificarlos (ver tipo de ajuste para Inodos, bloques)
    //abrir Inodos y verificar los apuntadores a bloque a carpeta de la raíz y agregar la nueva carpeta.
    //si están llenos los cuatro ítems regresar al Inodos y recorrer el siguiente apuntador libre.
}

void sistemaEXT2crearArchivos(char *comando)
{

}

void agregarIndicesTemporales(char *comando)
{

    char name[50];
    char path[50];
    int z,pathObligatorio=0,nombreObligatorio=0, valor=97,pathIgual=0;
    char ASCII,ID;

    strcpy(name," ");
    strcpy(path," ");

    for(z=0;comando[z];z++) //todas las letras se convierten en minúsculas
    {
        comando[z]=tolower(comando[z]);
    }
    char *token=strtok(comando," ");

    while(token!=NULL)
    {
        if (strcmp(token,"-path")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL, " ");
            strcpy(path,token);
            pathObligatorio=1;

        }else if(strcmp(token,"-name")==0 && token!=NULL) //obligatorio
        {
            token=strtok(NULL," ");
            strcpy(name,token);
            nombreObligatorio=1;

        }
        token=strtok(NULL,"=");
    }
    //comprobar sí el path anterior es igual al que viene
    if(strcmp(path,pathTMP)==0) //si son iguales no cambiar de letra pero sí de numero
    {
        pathIgual=1;
    }
    else //si no son iguales cambiar de letra y empezar el número.
    {
        strcpy(pathTMP,path);

    }
    if(pathObligatorio==1 && nombreObligatorio==1 && pathIgual==1)
    {
        //abrir el mbr y verificar cúantas particiones tengo.
        //contador y agregarlo al ID.
        int z,existeParticion=0;
        struct MBR mbrDisco;
        FILE *archivoMBR1=fopen(path,"rb+");
        fseek(archivoMBR1,0L, SEEK_SET);
        fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
        fclose(archivoMBR1);

        for(z=0;z<4;z++)
        {
          if(strcmp(mbrDisco.particion[z].nombreParticion,name)==0) //verificar el nombre de la particion
          {
                existeParticion=1;
                cont++;
                break;
          }
        }
        if(existeParticion==1)
        {
            char linea[100];
            char TMPASCII[100];
            char TMPASCIICONT[100];
            FILE *archivoTMP;
          if((archivoTMP=fopen("/home/fersro/temporal.txt","rt")!=NULL))
          {
            while(fgets(linea,100,archivoTMP)!=NULL)
            {
                ASCII=valor;
                ID=ASCII;
                if(linea[2]==ID)
                {
                    valor++;
                }
            }
          }
            archivoTMP=fopen("/home/fersro/temporal.txt","a+");
            ASCII=valor;
            strcpy(TMPASCII,"vd");
            strcat(TMPASCII,ASCII); //vda
            sprintf(TMPASCIICONT, "%d",cont); //vda1
            strcat(TMPASCII,TMPASCIICONT);
            fprintf(archivoTMP,"%s,%s\n",cont,TMPASCII,name);
            fclose(archivoTMP);
        }else
        {
            printf("El nombre de la partición NO existe.\n");
        }
    }else if(pathObligatorio==1 && nombreObligatorio==1 && pathIgual==0)
    {
        int z,existeParticion=0;
        struct MBR mbrDisco;
        FILE *archivoMBR1=fopen(path,"rb+");
        fseek(archivoMBR1,0L, SEEK_SET);
        fread(&mbrDisco,sizeof(struct MBR),1,archivoMBR1);
        fclose(archivoMBR1);

        for(z=0;z<4;z++)
        {
          if(strcmp(mbrDisco.particion[z].nombreParticion,name)==0) //verificar el nombre de la particion
          {
                existeParticion=1;
                cont++;
                break;
          }
        }
        if(existeParticion==1)
        {
                char TMPASCII[100];
                char TMPASCIICONT[100];
                FILE *archivoTMP;

                archivoTMP=fopen("/home/fersro/temporal.txt","a+");
                ASCII=valor;
                strcpy(TMPASCII,"vd");
                TMPASCII[2]=ASCII; //vda
                sprintf(TMPASCIICONT, "%s%d",TMPASCII,cont); //vda1
                fprintf(archivoTMP,"%s,%s,%s\n",TMPASCIICONT,name,path);
                fclose(archivoTMP);
          }
        }
    else
    {
        printf("El comando NO existe o esta mal escrito.\n");
    }

}
