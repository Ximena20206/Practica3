/*
INTEGRANTES DEL EQUIPO:
- Barajas Pacheco Harol Fabian
- Morán Diaz Barriga Jorge
- Rocha Arellano Ximena Yulian
- Rosales Barraza Erick Efren

V 1.0 Noviembre 2024

Programa que realiza la codificación de un archivo


Compilación
Windows: gcc huffmanCoder.c huffman.c -o h
Linux:  gcc huffmanCoder.c huffman.c -o h

Ejecución:
windows: ./h archivo
linux: ./h archivo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
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
    return 0;
}