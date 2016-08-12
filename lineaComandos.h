#ifndef LINEACOMANDOS_H_INCLUDED
#define LINEACOMANDOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <time.h>
#include <ctype.h>

extern char pathTMP[100];

struct INFOparticion{
    char estado; //indica sí está activa ó no.
    char tipo; //tipo de la partición P=primaria ó E=Extendida
    char ajuste; //tipo de ajuste B=Best, F=First, W=Worst
    int byteInicio; //byte inicial de la partición
    int byteFinal; //byte final de la partición (tamaño total)
    char nombreParticion[16]; //nombre de la partición
};

struct MBR{
    int tamano; //total del disco en bytes
    char fechaCreacion[50]; //fecha y hora de creación del disco
    int numeroRandom; //número para identificar a cada disco por un número.
    int cantidadPrimarias;
    int cantidadExtendida;
    struct INFOparticion particion[4];
};


void menuPrincipal();
void crearDiscos(char* comando);
void crearDiscosDatos(char *size, char *unidad, char *direccion);
void eliminarDiscos(char *comando);

void crearParticiones(char *comando);
void verificarTamanoParticiones(char *size, char *unidad, char *direccion, char *tipo, char *ajuste,
                           char *delet, char *name, char *add);
void crearParticionesDISCO(int tamano, char *direccion, char *tipo, char *ajuste, char *delet, char *name, char *add);
void crearParticionesEliminar(char *delet, char *nombre, char *direccion);
void crearParticionesAgregar(char *add, char *nombre, char *direccion, char *unidad);

#endif // LINEACOMANDOS_H_INCLUDED
