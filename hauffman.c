#include <stdio.h>
typedef struct NodoHuffman 
{
    char caracter;
    unsigned frecuencia;
    struct NodoHuffman *izquierda, *derecha;
} NodoHuffman;

void contarFrecuencias(const char *nombreArchivo, int frecuencias[]) 
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) 
	{
        perror("Error al abrir el archivo");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(archivo)) != EOF) 
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
/*Compilacion:
gcc hauffman.c -o hauffman
./hauffman archivo.ext archivovomp.ext */
int main (int *argc, char *argv[]){
	int frecuencias [256]={0};
	NodoHuffman *raiz;
	char codigos [256][256], codigoActual [256];
	char* nombreArchivo= argv[2];
}
