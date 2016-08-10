

void menuPrincipal()
{
    char comando[200];
    char comandoTMP[200];
    int z, entroMontar=0, entroDesmontar=0, soloDisco=0;

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
        printf("Crear Disco\n");
        soloDisco=1;

    }else if(strcmp(token,"rmdisk")==0) //eliminar discos
    {
        soloDisco=1;
        printf("Eliminar Disco\n");

    }else if(strcmp(token,"fdisk")==0) //crear particiones
    {
        soloDisco=1;
        printf("Crear Particiones Disco\n");

    }else if(strcmp(token,"mkfs")==0) //formateo completo de la partición
    {
        printf("Formatear Disco\n");

    }else if(strcmp(token,"mount")==0) //montar
    {
        printf("Montar Disco\n");
        entroMontar=1;

    }else if(strcmp(token,"unmount")==0) //desmontar
    {
        entroDesmontar=1;
        printf("Desmontar Disco\n");
    }
    menuPrincipal();
}
