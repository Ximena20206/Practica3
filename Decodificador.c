#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//DECLARACION DE FUNCIONES 

/*
bool esNodoHoja(NodoHuffman *nodo);
Descripción: Determina si un nodo dado en el árbol de Huffman es un nodo hoja.
Recibe: NodoHuffman *nodo (el nodo a evaluar).
Devuelve: bool (verdadero si el nodo es hoja, falso en caso contrario).
Observaciones: Los nodos hoja en el árbol de Huffman contienen los caracteres del texto original. Esta función es útil durante el proceso de decodificación.
*/
bool esNodoHoja(NodoHuffman *nodo);

/*
void decodificarHuffman(char *archivoCodificado, char *archivoFrecuencias, char *archivoSalida);
Descripción: Decodifica una secuencia de bits codificada mediante el algoritmo de Huffman, reconstruyendo el texto original.
Recibe: char *archivoCodificado (nombre del archivo que contiene la secuencia de bits codificada), char *archivoFrecuencias (nombre del archivo que contiene las frecuencias de los caracteres), char *archivoSalida (nombre del archivo donde se escribirá el texto decodificado).
Devuelve: void.
Observaciones: Esta función utiliza el árbol de Huffman reconstruido y la secuencia de bits para decodificar el mensaje original y escribirlo en el archivo de salida.
*/
void decodificarHuffman(char *archivoCodificado, char *archivoFrecuencias, char *archivoSalida);

/*
void liberarArbolHuffman(NodoHuffman *raiz);
Descripción: Libera la memoria asignada para el árbol de Huffman.
Recibe: NodoHuffman *raiz (la raíz del árbol de Huffman).
Devuelve: void.
Observaciones: Es importante liberar la memoria utilizada por el árbol una vez que se ha completado la decodificación para evitar fugas de memoria.
*/
void liberarArbolHuffman(NodoHuffman *raiz);

/*
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias);
Descripción: Reconstruye el árbol de Huffman a partir del archivo que contiene las frecuencias de los caracteres.
Recibe: const char *archivoFrecuencias (nombre o ruta del archivo que contiene las frecuencias de los caracteres).
Devuelve: NodoHuffman* (la raíz del árbol de Huffman reconstruido).
Observaciones: Esta función es esencial para la decodificación, ya que reconstruye la estructura del árbol necesaria para interpretar la secuencia codificada.
*/
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias);


int main() 
{
    char *archivoCodificado = "codificacion.txt";
    char *archivoFrecuencias = "frecuencias.txt";
    char *archivoSalida = "original.txt";

    decodificarHuffman(archivoCodificado, archivoFrecuencias, archivoSalida);
    return 0;
}




/*
bool esNodoHoja(NodoHuffman *nodo);
Descripción: Determina si un nodo dado en el árbol de Huffman es un nodo hoja.
Recibe: NodoHuffman *nodo (el nodo a evaluar).
Devuelve: bool (verdadero si el nodo es hoja, falso en caso contrario).
Observaciones: Los nodos hoja en el árbol de Huffman contienen los caracteres del texto original. Esta función es útil durante el proceso de decodificación.
*/
bool esNodoHoja(NodoHuffman *nodo) 
{
    return nodo->izquierda == NULL && nodo->derecha == NULL;
}

// Función para liberar la memoria del árbol de Huffman
/*
void liberarArbolHuffman(NodoHuffman *raiz);
Descripción: Libera la memoria asignada para el árbol de Huffman.
Recibe: NodoHuffman *raiz (la raíz del árbol de Huffman).
Devuelve: void.
Observaciones: Es importante liberar la memoria utilizada por el árbol una vez que se ha completado la decodificación para evitar fugas de memoria.
*/
void liberarArbolHuffman(NodoHuffman *raiz) 
{
    if (raiz == NULL) return;
    liberarArbolHuffman(raiz->izquierda);
    liberarArbolHuffman(raiz->derecha);
    free(raiz);
}

/*
void decodificarHuffman(char *archivoCodificado, char *archivoFrecuencias, char *archivoSalida);
Descripción: Decodifica una secuencia de bits codificada mediante el algoritmo de Huffman, reconstruyendo el texto original.
Recibe: char *archivoCodificado (nombre del archivo que contiene la secuencia de bits codificada), char *archivoFrecuencias 
(nombre del archivo que contiene las frecuencias de los caracteres), char *archivoSalida (nombre del archivo donde se escribirá el texto decodificado).
Devuelve: void.
Observaciones: Esta función utiliza el árbol de Huffman reconstruido y la secuencia de bits para decodificar el mensaje original y escribirlo en el archivo de salida.
*/
void decodificarHuffman(char *archivoCodificado, char *archivoFrecuencias, char *archivoSalida)
 {
    // Leer y reconstruir el árbol de Huffman usando 'archivoFrecuencias'
    NodoHuffman *raiz = reconstruirArbolHuffman(archivoFrecuencias);

    // Abrir el archivo codificado para lectura
    FILE *archivo = fopen(archivoCodificado, "r");
    if (archivo == NULL) 
	{
        perror("Error al abrir el archivo de codificación");
        exit(1);
    }

    // Abrir el archivo de salida para escritura
    FILE *salida = fopen(archivoSalida, "w");
    if (salida == NULL) 
	{
        perror("Error al abrir el archivo de salida");
        exit(1);
    }

    char bit;
    NodoHuffman *actual = raiz;
    while ((bit = fgetc(archivo)) != EOF) 
	{
        // Recorrer el árbol de Huffman
        if (bit == '0') 
		{
            actual = actual->izquierda;
        } else if (bit == '1') 
		{
            actual = actual->derecha;
        }

        // Si llegamos a un nodo hoja
        if (esNodoHoja(actual)) 
		{
            fprintf(salida, "%c", actual->caracter);
            actual = raiz;  // Volver al inicio del árbol para el siguiente carácter
        }
    }

    fclose(archivo);
    fclose(salida);
    liberarArbolHuffman(raiz); // Función para liberar la memoria del árbol
}

// Función para reconstruir el árbol de Huffman a partir del archivo de frecuencias
/*
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias);
Descripción: Reconstruye el árbol de Huffman a partir del archivo que contiene las frecuencias de los caracteres.
Recibe: const char *archivoFrecuencias (nombre o ruta del archivo que contiene las frecuencias de los caracteres).
Devuelve: NodoHuffman* (la raíz del árbol de Huffman reconstruido).
Observaciones: Esta función es esencial para la decodificación, ya que reconstruye la estructura del árbol necesaria para interpretar la secuencia codificada.
*/
NodoHuffman* reconstruirArbolHuffman(const char *archivoFrecuencias) 
{
    FILE *archivo = fopen(archivoFrecuencias, "r");
    if (archivo == NULL) 
	{
        perror("Error al abrir el archivo de frecuencias");
        exit(1);
    }

    NodoHuffman* cola[256]; // Cola de prioridad para almacenar los nodos
    int tamano = 0;

    char caracter;
    int frecuencia;
    while (fscanf(archivo, "%c %d\n", &caracter, &frecuencia) != EOF) 
	{
        NodoHuffman* nodo = crearNodoHuffman(caracter, frecuencia);
        insertarEnColaDePrioridad(cola, &tamano, nodo);
    }

    // Construir el árbol de Huffman
    while (tamano > 1) 
	{
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

    fclose(archivo);
    return cola[0]; // Retorna la raíz del árbol reconstruido
}

