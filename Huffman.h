#ifndef HUFFMAN_H
#define HUFFMAN_H

// Estructura para un nodo en el Árbol de Huffman
typedef struct NodoHuffman 
{
    char caracter;
    unsigned frecuencia;
    struct NodoHuffman *izquierda, *derecha;
} NodoHuffman;

// Funciones para el algoritmo de Huffman
/*
void contarFrecuencias(const char *nombreArchivo, int frecuencias[]);
Descripción: Lee un archivo de texto y cuenta la frecuencia de aparición de cada carácter ASCII.
Recibe: const char *nombreArchivo (nombre o ruta del archivo a leer), int frecuencias[] (arreglo de frecuencias a actualizar).
Devuelve: void.
Observaciones: Actualiza el arreglo de frecuencias basado en los caracteres encontrados en el archivo.
*/
void contarFrecuencias(const char *nombreArchivo, int frecuencias[]);

/*
NodoHuffman* crearNodoHuffman(char caracter, unsigned frecuencia);
Descripción: Crea un nuevo nodo de Huffman con un carácter y su frecuencia.
Recibe: char caracter (el carácter para el nodo), unsigned frecuencia (la frecuencia de aparición del carácter).
Devuelve: NodoHuffman* (puntero a un nuevo nodo de Huffman).
Observaciones: El nodo creado tiene sus hijos izquierdo y derecho inicializados en NULL.
*/
NodoHuffman* crearNodoHuffman(char caracter, unsigned frecuencia);

/*
void insertarEnColaDePrioridad(NodoHuffman** cola, int* tamaño, NodoHuffman* nodo);
Descripción: Inserta un nodo en la cola de prioridad, manteniendo el orden por frecuencia.
Recibe: NodoHuffman** cola (referencia a la cola de prioridad), int* tamaño (tamaño actual de la cola), NodoHuffman* nodo (nodo a insertar).
Devuelve: void.
Observaciones: La cola de prioridad está implementada como un arreglo simple.
*/
void insertarEnColaDePrioridad(NodoHuffman** cola, int* tamano, NodoHuffman* nodo);
/*
NodoHuffman* construirArbolDeHuffman(int frecuencias[]);
Descripción: Construye el árbol de Huffman a partir de un arreglo de frecuencias.
Recibe: int frecuencias[] (arreglo de frecuencias de caracteres).
Devuelve: NodoHuffman* (raíz del árbol de Huffman construido).
Observaciones: Utiliza una cola de prioridad para construir el árbol.
*/
NodoHuffman* construirArbolDeHuffman(int frecuencias[]);

/*
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias);
Descripción: Reconstruye el árbol de Huffman a partir del archivo que contiene las frecuencias de los caracteres.
Recibe: const char *archivoFrecuencias (nombre o ruta del archivo que contiene las frecuencias de los caracteres).
Devuelve: NodoHuffman* (la raíz del árbol de Huffman reconstruido).
Observaciones: Esta función es esencial para la decodificación, ya que reconstruye la estructura del árbol necesaria para interpretar la secuencia codificada.
*/
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias);

#endif