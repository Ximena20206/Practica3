/*
INTEGRANTES DEL EQUIPO:
- Barajas Pacheco Harol Fabian
- Morán Diaz Barriga Jorge
- Rocha Arellano Ximena Yulian
- Rosales Barraza Erick Efren

V 1.0 Noviembre 2024

Programa que realiza la decodificación de un archivo


Compilación
Windows: gcc huffmanDeoder.c huffman.c -o hd
Linux:  gcc huffmanDeoder.c huffman.c -o hd

Ejecución:
windows: ./hd archivo
linux: ./hd archivo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"

/*
    En la siguiente función "crearNodo", se crea un nuevo nodo para un árbol de Huffman.

    Parámetros:
        - caracter: El carácter representado por el nodo. Para nodos internos, puede ser un valor especial como -1.
        - frecuencia: La frecuencia asociada al carácter en los datos.

    Observaciones:
        - Los nodos creados se inicializan con sus hijos izquierdo y derecho como NULL.
        - Este nodo puede representar tanto hojas como nodos internos del árbol.

    Algoritmo:
        - Se reserva memoria para un nuevo nodo de tipo NodoHuffman.
        - Se asignan los valores del carácter y la frecuencia al nodo.
        - Los punteros izquierdo y derecho del nodo se inicializan a NULL.

    Devuelve:
        - Un puntero al nodo creado.
*/
NodoHuffman* crearNodo(char caracter, unsigned frecuencia) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    nodo->caracter = caracter;
    nodo->frecuencia = frecuencia;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

/*
    En la siguiente función "reconstruirArbol", se reconstruye un árbol de Huffman
    utilizando una tabla de códigos previamente generada.

    Parámetros:
        - codigos: Una tabla que contiene los códigos binarios para cada carácter (hasta 256).

    Observaciones:
        - Cada código binario se interpreta como un camino en el árbol, donde '0' indica moverse
          a la izquierda y '1' a la derecha.
        - Los caracteres asociados a cada código se almacenan en las hojas del árbol.

    Algoritmo:
        - Se crea un nodo raíz vacío.
        - Para cada carácter con un código válido en la tabla:
            - Se recorre el código binario, moviéndose por los nodos existentes o creando nuevos.
            - Al final del código, se almacena el carácter en el nodo correspondiente.
        - Se retorna el nodo raíz del árbol reconstruido.

    Devuelve:
        - Un puntero a la raíz del árbol de Huffman reconstruido.
*/
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
            actual->caracter = i;
        }
    }
    return raiz;
}

/*
    En la siguiente función "leerTablaDecodificacion", se lee una tabla de códigos de decodificación
    desde un archivo de texto.

    Parámetros:
        - codigos: Un arreglo de cadenas que almacenará los códigos binarios para cada carácter.
        - archivo: El nombre del archivo que contiene la tabla de códigos.

    Observaciones:
        - Cada línea del archivo debe estar en el formato "carácter: código_binario".
        - El archivo debe ser accesible y no estar vacío.

    Algoritmo:
        - Se abre el archivo para lectura.
        - Se lee línea por línea, analizando el carácter y su código binario.
        - Se almacena el código binario en la posición correspondiente del carácter en la tabla.
        - El archivo se cierra después de la lectura.

    Devuelve:
        - Nada. Actualiza directamente la tabla de códigos.
*/
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

/*
    La función "decodificarArchivo" decodifica un archivo codificado con Huffman
    y genera el archivo original.

    Parámetros:
        - raiz: Nodo raíz del árbol de Huffman utilizado para la decodificación.
        - entrada: Nombre del archivo codificado de entrada.
        - salida: Nombre del archivo donde se guardará el contenido decodificado.

    Observaciones:
        - El archivo de entrada contiene la codificación en formato binario.
        - La decodificación recorre el árbol de Huffman en función de los bits del archivo.

    Algoritmo:
        - Abre el archivo de entrada en modo lectura y el de salida en modo escritura.
        - Recorre el archivo de entrada bit por bit, navegando el árbol de Huffman.
        - Cuando se alcanza una hoja, escribe el carácter asociado en el archivo de salida.
        - Reinicia la navegación desde la raíz del árbol.

    Devuelve:
        - Nada. Genera el archivo decodificado.
*/
void decodificarArchivo(NodoHuffman* raiz, const char* entrada, const char* salida) {
    FILE *f = fopen(entrada, "r");
    if (!f) {
        perror("Error al abrir el archivo de codificación");
        return;
    }

    FILE *g = fopen(salida, "wb");
    if (!g) {
        perror("Error al abrir el archivo de salida");
        return;
    }

    int bit;
    NodoHuffman* actual = raiz;

    while((bit = fgetc(f)) != EOF) {
        if (actual->der == NULL && actual->izq == NULL) {
            fputc(actual->caracter, g);
            actual = raiz;
        }
        if (bit == '0') {
            actual = actual->izq;
        }else if (bit == '1') {
            actual = actual->der;
        }
    }
    fputc(actual->caracter, g);

    fclose(f);
    
    if (!g) {
        perror("Error al abrir el archivo de salida");
        return;
    }
    
}

/* Programa principal del decodificador*/
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
    free(arbol);
    return 0;
}

