#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TADListaSL.h" 
#include "hauffman.h"

//compilacion gcc haufman.c hauffman.h-o hauffman 
//ejecucion /.hauffman nombreArchivo
int main (int argc, char *argv[]){
	char* nombreArchivo= argv[1];
	FILE* p=leerArchivo(nombreArchivo);
	if (p==NULL){
	fprintf (stderr,"No se pudo abrir el archivo");
		return 0;
	}
	fclose(p);
	contarFrecuencias(p);

    posicion po=First(&ocurrencias)
    for(int i=0;i<Size(&ocurrencias);i++)
	{
		elemento a=Position(&ocurrencias,po);
		printf("\n%d, %c",a.n, a.c);
		po=Following(&ocurrencias,p);
	}


	Destroy(&ocurrencias);
	return 0;
}