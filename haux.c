
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
/*
    En la siguiente función "contarFrecuencias", se cuentan las frecuencias de cada carácter
    en un archivo dado.

    Parámetros:
        - frecuencias: Un arreglo de enteros donde se almacenará la frecuencia de cada carácter.
        - archivo: El nombre del archivo del cual se contarán las frecuencias.

    Observaciones:
        - El archivo debe ser accesible y se leerá completamente.
        - Cada posición en el arreglo frecuencias representa un carácter ASCII (0-255).

    Algoritmo:
        - Inicializa el arreglo frecuencias en 0.
        - Abre el archivo en modo lectura.
        - Lee el archivo carácter por carácter, incrementando la frecuencia correspondiente.
        - Cierra el archivo después de la lectura.

    Devuelve:
        - Nada. Actualiza directamente el arreglo de frecuencias.
*/
void contarFrecuencias(FILE* p, int* frecuencias);

/*
    La función "creaNodo" crea un nuevo nodo para el árbol de Huffman.

    Parámetros:
        - ch: Carácter asociado al nodo. -1 si es un nodo interno.
        - valor: Frecuencia del carácter o la suma de las frecuencias de los nodos hijos.

    Observaciones:
        - Los nodos creados pueden representar hojas (caracteres) o nodos internos.

    Algoritmo:
        - Reserva memoria para un nuevo nodo.
        - Inicializa los valores del nodo con los parámetros proporcionados.
        - Los punteros izquierdo, derecho y siguiente se inicializan en NULL.

    Devuelve:
        - Un puntero al nodo recién creado.
*/
NodoHuffman* creaNodo(int ch, int valor);

/*
    En la siguiente función "insertarOrdenado", se inserta un nodo en una lista enlazada
    manteniendo el orden según la frecuencia de los nodos.

    Parámetros:
        - lista: Un puntero doble al nodo cabeza de la lista.
        - nuevoNodo: El nodo a insertar.

    Observaciones:
        - La lista enlazada debe estar previamente ordenada por frecuencia.
        - Si la lista está vacía, el nuevo nodo se convierte en la cabeza.

    Algoritmo:
        - Si la lista está vacía o el nuevo nodo tiene menor frecuencia que la cabeza actual,
          el nuevo nodo se convierte en la nueva cabeza.
        - De lo contrario, se recorre la lista hasta encontrar la posición adecuada para insertar.
        - Se ajustan los punteros para insertar el nuevo nodo.

    Devuelve:
        - Nada. Actualiza directamente la lista enlazada.
*/
NodoHuffman* insertarOrdenado(NodoHuffman* lista, NodoHuffman* nuevo);

/*
    En la siguiente función "arbolHuffman", se construye un árbol de Huffman a partir
    de las frecuencias de los caracteres.

    Parámetros:
        - frecuencias: Un arreglo de enteros con las frecuencias de cada carácter.

    Observaciones:
        - El árbol de Huffman es una estructura utilizada para la compresión de datos.
        - Los nodos hoja representan caracteres, mientras que los nodos internos combinan
          frecuencias de sus hijos.

    Algoritmo:
        - Crea una lista enlazada ordenada con nodos hoja para cada carácter con frecuencia > 0.
        - Mientras haya más de un nodo en la lista:
            - Toma los dos nodos con menor frecuencia.
            - Crea un nuevo nodo que combine las frecuencias de ambos.
            - Inserta el nuevo nodo en la lista manteniendo el orden.
        - El único nodo restante es la raíz del árbol de Huffman.

    Devuelve:
        - Un puntero al nodo raíz del árbol de Huffman.
*/
NodoHuffman* arbolHuffman(NodoHuffman* raiz);

/*
    La función "asignaArbol" genera los códigos binarios de cada carácter
    recorriendo un árbol de Huffman.

    Parámetros:
        - raiz: Nodo raíz del árbol de Huffman.
        - codigo: Arreglo temporal para almacenar el código actual durante el recorrido.
        - longitud: Longitud del código actual.
        - codigos: Arreglo de cadenas para guardar los códigos de cada carácter.

    Observaciones:
        - Los códigos binarios se generan al recorrer el árbol: '0' para la izquierda y '1' para la derecha.
        - Se almacena el código en el índice correspondiente al carácter en el arreglo codigos.

    Algoritmo:
        - Si el nodo es una hoja, guarda el código actual en el índice del carácter.
        - Llama recursivamente a la función para los nodos izquierdo y derecho.

    Devuelve:
        - Nada. Actualiza el arreglo codigos con los valores generados.
*/
void asignaArbol(NodoHuffman* raiz, char* codigo, int longitud, char codigos[256][256]);

/*
    La función "grabaTablaDecodificacion" escribe los códigos de Huffman en un archivo.

    Parámetros:
        - codigos: Arreglo de cadenas con los códigos de cada carácter.
        - archivo: Nombre del archivo donde se guardará la tabla.

    Observaciones:
        - Sólo se escriben los caracteres con códigos definidos.

    Algoritmo:
        - Abre el archivo en modo escritura.
        - Recorre el arreglo de códigos, escribiendo cada carácter y su código.
        - Cierra el archivo al terminar.

    Devuelve:
        - Nada. Genera un archivo con la tabla de decodificación.
*/
void grabaTablaDecodificacion(char codigos[256][256], const char* archivo);

/*
    En la siguiente función "grabaCompresion", se escribe la codificación de un archivo
    utilizando los códigos de Huffman generados previamente.

    Parámetros:
        - entrada: Puntero al archivo original que será comprimido.
        - salida: Nombre del archivo donde se guardará la codificación comprimida.
        - codigos: Arreglo de cadenas que contiene los códigos de Huffman para cada carácter.

    Observaciones:
        - La función asume que los códigos de Huffman para todos los caracteres ya han sido generados
          y están almacenados en el arreglo `codigos`.
        - Cada carácter del archivo de entrada es reemplazado por su código correspondiente.

    Algoritmo:
        - Abre el archivo de salida en modo escritura binaria.
        - Recorre el archivo de entrada carácter por carácter.
        - Busca el código correspondiente en el arreglo `codigos` y lo escribe en el archivo de salida.
        - Cierra el archivo de salida al finalizar.

    Devuelve:
        - Nada. Genera el archivo comprimido con la codificación de Huffman.
*/
void grabaCompresion(FILE* entrada, const char* salida, char codigos[256][256]);

/*
    En la siguiente función "leerArchivo", se abre un archivo en modo lectura binaria
    y devuelve un puntero al archivo.

    Parámetros:
        - nombreArchivo: Nombre del archivo que se desea abrir.

    Observaciones:
        - El archivo debe ser accesible y estar disponible en el sistema de archivos.
        - Si el archivo no puede ser abierto, la función retorna un puntero nulo (NULL).
        - Es responsabilidad del usuario cerrar el archivo después de usarlo.

    Algoritmo:
        - Llama a `fopen` con el nombre del archivo y el modo "rb" (lectura binaria).
        - Si el archivo se abre correctamente, retorna el puntero al archivo.
        - Si ocurre un error al abrir el archivo, retorna NULL.

    Devuelve:
        - Un puntero al archivo abierto, o NULL si no se puede abrir.
*/
FILE* leerArchivo(char* nombreArchivo);