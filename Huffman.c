#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>


/*
void contarFrecuencias(const char *nombreArchivo, int frecuencias[]);
Descripción: Lee un archivo de texto y cuenta la frecuencia de aparición de cada carácter ASCII.
Recibe: const char *nombreArchivo (nombre o ruta del archivo a leer), int frecuencias[] (arreglo de frecuencias a actualizar).
Devuelve: void.
Observaciones: Actualiza el arreglo de frecuencias basado en los caracteres encontrados en el archivo.
*/
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

/*
NodoHuffman* crearNodoHuffman(char caracter, unsigned frecuencia);
Descripción: Crea un nuevo nodo de Huffman con un carácter y su frecuencia.
Recibe: char caracter (el carácter para el nodo), unsigned frecuencia (la frecuencia de aparición del carácter).
Devuelve: NodoHuffman* (puntero a un nuevo nodo de Huffman).
Observaciones: El nodo creado tiene sus hijos izquierdo y derecho inicializados en NULL.
*/
NodoHuffman* crearNodoHuffman(char caracter, unsigned frecuencia) 
{
    NodoHuffman* nuevoNodo = (NodoHuffman*) malloc(sizeof(NodoHuffman));
    if (nuevoNodo == NULL) 
	{
        perror("Error al crear un nuevo nodo de Huffman");
        exit(1);
    }

    nuevoNodo->caracter = caracter;
    nuevoNodo->frecuencia = frecuencia;
    nuevoNodo->izquierda = nuevoNodo->derecha = NULL;
	
    return nuevoNodo;
}

// Inserta un nodo en la cola de prioridad manteniendo el orden por frecuencia
/*
void insertarEnColaDePrioridad(NodoHuffman** cola, int* tamaño, NodoHuffman* nodo);
Descripción: Inserta un nodo en la cola de prioridad, manteniendo el orden por frecuencia.
Recibe: NodoHuffman** cola (referencia a la cola de prioridad), int* tamaño (tamaño actual de la cola), NodoHuffman* nodo (nodo a insertar).
Devuelve: void.
Observaciones: La cola de prioridad está implementada como un arreglo simple.
*/
void insertarEnColaDePrioridad(NodoHuffman** cola, int* tamano, NodoHuffman* nodo) 
{
    int i = *tamano - 1;
	
    // Buscar la posición correcta para el nuevo nodo
    while (i >= 0 && cola[i]->frecuencia > nodo->frecuencia) 
	{
        cola[i + 1] = cola[i];
        i--;
    }

    // Insertar el nuevo nodo
    cola[i + 1] = nodo;
    (*tamano)++;
	
}


// Construye el árbol de Huffman a partir de las frecuencias de los caracteres
/*
NodoHuffman* construirArbolDeHuffman(int frecuencias[]);
Descripción: Construye el árbol de Huffman a partir de un arreglo de frecuencias.
Recibe: int frecuencias[] (arreglo de frecuencias de caracteres).
Devuelve: NodoHuffman* (raíz del árbol de Huffman construido).
Observaciones: Utiliza una cola de prioridad para construir el árbol.
*/
// Construye el árbol de Huffman a partir de las frecuencias de los caracteres
NodoHuffman* construirArbolDeHuffman(int frecuencias[]) 
{
    NodoHuffman* cola[256]; // Cola de prioridad para almacenar los nodos
    int tamano = 0; // Tamaño actual de la cola

    // Crear un nodo de Huffman para cada carácter y agregarlo a la cola
    for (int i = 0; i < 256; i++) 
	{
        if (frecuencias[i] > 0) 
		{
            NodoHuffman* nodo = crearNodoHuffman(i, frecuencias[i]);
            insertarEnColaDePrioridad(cola, &tamano, nodo);
        }
    }

    // Construir el árbol de Huffman
    while (tamano > 1) 
	{
        // Extraer los dos nodos con menor frecuencia
        NodoHuffman *izquierda = cola[0];
        NodoHuffman *derecha = cola[1];

        // Crear un nuevo nodo padre
        NodoHuffman* padre = crearNodoHuffman('\0', izquierda->frecuencia + derecha->frecuencia);
        padre->izquierda = izquierda;
        padre->derecha = derecha;

        // Quitar los dos nodos de la cola
        for (int j = 2; j < tamano; j++) 
		{
            cola[j - 2] = cola[j];
        }
        tamano -= 2;

        // Insertar el padre en la cola
        insertarEnColaDePrioridad(cola, &tamano, padre);
    }

    // El último nodo en la cola es la raíz del árbol de Huffman
    return cola[0];
}


