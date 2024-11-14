#include <stdio.h>
#include <string.h>
#include "huffman.h"

//void almecenarCola ()
//	
//}
void contarFrecuencias(FILE* p, int* frecuencias) { 
	int caracter; 
	while ((caracter = fgetc(p)) != EOF){ 
		frecuencias[(unsigned char)caracter]++; 
	} 
}
void arbolBinario (){
	
}
void lista(){
	
}
void asignaArbol(){
	
}
void grabaCompresion(){
	
}


FILE* leerArchivo(char* nombreArchivo){
	FILE* p=fopen (nombreArchivo, "r");
	return p;
}
//compilacion gcc haufman.c hauffman.h-o hauffman 
//ejecucion /.hauffman nombreArchivo
int main (int argc, char *argv[]){
	int i;
	char* nombreArchivo= argv[1];
	FILE* p=leerArchivo(nombreArchivo);
	if (p==NULL){
	fprintf (stderr,"No se pudo abrir el archivo");
		return 0;
	}
	int frecuencias[256] = {0}; 
	contarFrecuencias(p, frecuencias);
	for (i = 0; i < 256; i++) { 
		if (frecuencias[i] > 0) 
		printf("Caracter '%c' (ASCII %d): %d veces\n", i, i, frecuencias[i]); 
	}
	
}
