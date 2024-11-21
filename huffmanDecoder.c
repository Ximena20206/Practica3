#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

// Crear un nuevo nodo de Huffman
NodoHuffman* crearNodo(char caracter, unsigned frecuencia) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    nodo->caracter = caracter;
    nodo->frecuencia = frecuencia;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

// Reconstruye el árbol de Huffman a partir de la tabla de códigos
NodoHuffman* reconstruirArbol(char codigos[256][256]) {
    NodoHuffman* raiz = crearNodo(-1, 0);
    for (int i = 0; i < 256; i++) {
        if (codigos[i][0] != '\0') {
            NodoHuffman* actual = raiz;
            char* codigo = codigos[i];
            for (int j = 0; codigo[j] != '\0'; j++) {
                if (codigo[j] == '0') {
                    if (actual->izq == NULL) {
                        actual->izq = crearNodo(-1, 0);
                    }
                    actual = actual->izq;
                } else if (codigo[j] == '1') {
                    if (actual->der == NULL) {
                        actual->der = crearNodo(-1, 0);
                    }
                    actual = actual->der;
                }
            }
            actual->caracter = (char)i;
        }
    }
    return raiz;
}

// Lee la tabla de decodificación desde el archivo
void leerTablaDecodificacion(char codigos[256][256], const char* archivo) {
    FILE* f = fopen(archivo, "r");
    if (!f) {
        perror("Error al abrir el archivo de la tabla de decodificación");
        return;
    }
    char linea[512];
    while (fgets(linea, sizeof(linea), f)) {
        char caracter;
        char codigo[256];
        sscanf(linea, "%c: %s", &caracter, codigo);
        strcpy(codigos[(unsigned char)caracter], codigo);
    }
    fclose(f);
}

// Función para leer un bit del archivo comprimido
int leerBit(FILE* archivo, int* buffer, int* posicion) {
    if (*posicion == 8) { // Si ya hemos leído todos los bits del byte actual
        *buffer = fgetc(archivo); // Leer el siguiente byte
        if (*buffer == EOF) {
            return -1; // Fin del archivo
        }
        *posicion = 0; // Reiniciar la posición
    }

    int bit = (*buffer >> (7 - *posicion)) & 1; // Extraer el bit actual
    (*posicion)++; // Avanzar a la siguiente posición
    return bit;
}

// Decodifica el archivo comprimido utilizando el árbol de Huffman
void decodificarArchivo(NodoHuffman* raiz, const char* entrada, const char* salida) {
    
}

// Programa principal del decodificador
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_salida>\n", argv[0]);
        return 1;
    }

    char codigos[256][256] = {0};
    leerTablaDecodificacion(codigos, "frecuencias.txt");

    NodoHuffman* arbol = reconstruirArbol(codigos);
    decodificarArchivo(arbol, "codificacion.dat", argv[1]);

    printf("Archivo decodificado correctamente en '%s'.\n", argv[1]);
    return 0;
}
