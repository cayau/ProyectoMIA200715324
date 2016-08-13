#ifndef EXT2_H
#define EXT2_H

#include "lineaComandos.h"

struct SUPERBLOQUE{

    int s_totalInodos; //guarda el número total de inodos.
    int s_totalBloques; //guarda el número total de bloques.
    int s_bloquesLibres; //contiene el número de bloques libres.
    int s_inodosLibres; //contiene el número de inodos libres.
    char s_fechaMontaje[50]; //fecha última de montaje.
    char s_fechaDesmontaje[50]; //fecha última de desmontaje.
    int s_contadorMontaje; //indica cúantas veces se ha montado el sistema.
    int s_magico; //nada relevante sólo contiene el valor de 0xEF53
    int s_tamanoInodo; //tamaño inodo
    int s_tamanoBloque; //tamaño del bloque
    int s_primerInodoLibre; //primer inodo libre.
    int s_primerBloqueLibre; //primer bloque libre.
    int s_bitmapInodos; //guarda el inicio del bitmap de inodos.
    int s_bitmapBloques; //guarda el inicio del bitmap de bloques.
    int s_inicioInodos; //guarda el inicio de la tabla de inodos.
    int s_inicioBloques; //guarda el inicio de la tabla de bloques.
};

struct bitmapINODOS
{
    char val;
};

struct bitmapBLOQUES
{
    char val;
};

struct INODOS
{
    int i_uid; //UID del usuario y tendrá el valor de 1.
    int i_tamano; //tamano del archivo en bytes.
    char i_ultimaFecha[50]; //última fecha en que se leyó el inodo sin modificarlo.
    char i_inicioFecha[50]; //fecha en que se creó el inodo.
    char i_ultimaFechaModificacion[50]; //última fecha en que se modificó el inodo.
    int i_block[15]; // 12 registros son directos y los últimos 3 son indirectos con tres capas, tendrán el valor de -1.
    char i_tipo; //indica si 1=Archivo ó 0=carpeta
};

struct contenidoBloqueCARPETAS
{
    char b_nombre; //nombre de la carpeta ó archivo.
    int b_inodo; //apuntador hacia un inodo asociado al archivo o carpeta.
};

struct bloqueCARPETAS
{
    struct contenidoBloqueCARPETAS contenido[4]; //los primeros dos se utilizan para . apunta a sí mismo y el .. al padre
};


struct bloqueARCHIVOS
{
    char contenido[64]; //contenido del archivo.
};

struct bloquesAPUNTADORES{
    int apuntadores[16];
};

struct BLOQUES{
    struct bloqueARCHIVOS tipo1;
    struct bloqueCARPETAS tipo2;
    struct bloquesAPUNTADORES tipo3;
};

void formateoParticion(char *comando);
void formateoCompleto(char *direccion, char *name);
void formateoRapido(char *direccion,char* name);
void agregarQuitarEspacioParticion(char *path, char *name, char *add, char *unit);
void sistemaEXT2(char *path, char *name);
void sistemaEXT2crearArchivos(char *comando);
void sistemaEXT2crearCarpetas(char *comando);

void agregarIndicesTemporales(char *comando);

int cont;



#endif // EXT2_H
