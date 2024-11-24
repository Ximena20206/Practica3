#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

/* Función que calcula el tamaño de un archivo en bytes */
long obtenerTamanioArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return -1;
    }
    fseek(archivo, 0, SEEK_END);
    long tamanio = ftell(archivo);
    fclose(archivo);
    return tamanio;
}

/* Función que calcula y muestra el porcentaje de compresión */
void calcularPorcentajeCompresion(const char* archivoOriginal, const char* archivoComprimido) {
    long tamanioOriginal = obtenerTamanioArchivo(archivoOriginal);
    long tamanioComprimido = obtenerTamanioArchivo(archivoComprimido);

    if (tamanioOriginal == -1 || tamanioComprimido == -1) {
        fprintf(stderr, "Error al obtener el tamaño de los archivos\n");
        return;
    }

    double porcentajeCompresion = (1 - (double)tamanioComprimido / tamanioOriginal) * 100;

    printf("Tamanio del archivo original: %ld bytes\n", tamanioOriginal);
    printf("Tamanio del archivo comprimido: %ld bytes\n", tamanioComprimido);
    printf("Porcentaje de compresion: %.2f%%\n", porcentajeCompresion);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_entrada> \n", argv[0]);
        return 1;
    }

    int i;
    char* nombreArchivo = argv[1];
    FILE* p = leerArchivo(nombreArchivo);
    if (p == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo\n");
        return 0;
    }

    int frecuencias[256] = {0}; 
    contarFrecuencias(p, frecuencias);

    NodoHuffman* lista = NULL;
    for (i = 0; i < 256; i++) {
        if (frecuencias[i] != 0) { 
            lista = insertarOrdenado(lista, creaNodo(i, frecuencias[i]));
        }
    }

    NodoHuffman* arbol = arbolHuffman(lista);

    char codigos[256][256] = {0};
    char codigo[256] = {0};
    asignaArbol(arbol, codigo, 0, codigos);

    rewind(p);
    grabaTablaDecodificacion(codigos, "frecuencias.txt");
    grabaCompresion(p, "codificacion.dat", codigos);

    fclose(p);

    // Calcular porcentaje de compresión
    calcularPorcentajeCompresion(nombreArchivo, "codificacion.dat");

    return 0;
}