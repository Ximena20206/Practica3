#include <stdio.h>
#include "huffman.h"
//void almecenarCola (){
//	
//}
void contarFrecuencias(FILE* p){
    char ch;
    while ((ch = fgetc(archivo)) != eof) 
	{
        frecuencias[(unsigned char)ch]++;
    }

    fclose(archivo);

    // Imprimir las frecuencias después de leer todo el archivo
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            printf("Caracter '%c', Frecuencia: %d\n", i, frecuencias[i]);
        }
    }
}
FILE* leerArchivo(char* nombreArchivo){
	FILE* p=fopen (nombreArchivo, "r");
	return p;
}
//compilacion gcc haufman.c hauffman.h-o hauffman 
//ejecucion /.hauffman nombreArchivo
int main (int *argc, char *argv[]){
	char* nombreArchivo= argv[1];
	FILE* p=leerArchivo(nombreArchivo);
	if (p==NULL){
	fprintf (stderr,"No se pudo abrir el archivo");
		return 0;
	}
	void contarFrecuencias(p);
	
}
