#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

void contarFrecuencias(FILE* p, int* frecuencias) { 
    int byte; 
    while ((byte = fgetc(p)) != EOF) { 
        frecuencias[byte]++; 
    } 
}

NodoHuffman* creaNodo(int ch, int valor) {
    NodoHuffman* nuevoNodo = (NodoHuffman*)malloc(sizeof(NodoHuffman)); 
    nuevoNodo->valor = valor; 
    nuevoNodo->caracter = (char)ch;
    nuevoNodo->izq = NULL; 
    nuevoNodo->der = NULL; 
    nuevoNodo->sig = NULL; 
    return nuevoNodo;
}

NodoHuffman* insertarOrdenado(NodoHuffman* lista, NodoHuffman* nuevo) { 
    if (!lista || nuevo->valor <= lista->valor) { 
        nuevo->sig = lista; 
        return nuevo; 
    } 
    NodoHuffman* actual = lista; 
    while (actual->sig && actual->sig->valor < nuevo->valor) { 
        actual = actual->sig; 
    } 
    nuevo->sig = actual->sig; 
    actual->sig = nuevo; 
    return lista; 
}

NodoHuffman* arbolHuffman(NodoHuffman* raiz) {
    while (raiz && raiz->sig) {
        NodoHuffman* siguiente = raiz->sig;
        NodoHuffman* combinado = creaNodo(-1, raiz->valor + siguiente->valor); 
        combinado->izq = raiz; 
        combinado->der = siguiente; 
        raiz = siguiente->sig; 
        raiz = insertarOrdenado(raiz, combinado);
    }
    return raiz;
}

void asignaArbol(NodoHuffman* raiz, char* codigo, int longitud, char codigos[256][256]) {
    if (raiz == NULL) {
        return;
    }
    if (raiz->izq == NULL && raiz->der == NULL) {
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
        printf("Codigo para '%c': %s\n", raiz->caracter, codigo);
    }

    if (raiz->izq != NULL) {
        codigo[longitud] = '0';
        asignaArbol(raiz->izq, codigo, longitud + 1, codigos);
    }

    if (raiz->der != NULL) {
        codigo[longitud] = '1';
        asignaArbol(raiz->der, codigo, longitud + 1, codigos);
    }
}

void grabaTablaDecodificacion(char codigos[256][256], const char* archivo) {
    FILE* f = fopen(archivo, "w");
    if (!f) {
        perror("Error al abrir el archivo para la tabla de decodificación");
        return;
    }
    for (int i = 0; i < 256; i++) {
        if (codigos[i][0] != '\0') {
            fprintf(f, "%c: %s\n", i, codigos[i]);
        }
    }
    fclose(f);
}

void grabaCompresion(FILE* entrada, const char* salida, char codigos[256][256]) {
    FILE* f = fopen(salida, "wb");
    if (!f) {
        perror("Error al abrir el archivo de salida");
        return;
    }
    int byte;
    while ((byte = fgetc(entrada)) != EOF) {
        fputs(codigos[byte], f);
    }
    fclose(f);
}

FILE* leerArchivo(char* nombreArchivo) {
    FILE* p = fopen(nombreArchivo, "rb");
    return p;
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
    return 0;
}