#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"


/* 
     La función contarFrecuencias calcula la frecuencia de cada carácter presente en un archivo.

     Parámetros:
         - p: Puntero a un archivo abierto en modo lectura (FILE*).
         - frecuencias: Arreglo de enteros que almacena las frecuencias de cada carácter (ASCII).

     Descripción:
         - Lee el archivo carácter por carácter y aumenta en 1 la frecuencia del carácter leído en el arreglo.
         - Cada posición del arreglo frecuencias representa la frecuencia de un carácter ASCII específico.
    
     Algoritmo:
         - Leer cada carácter del archivo.
         - Aumentar en 1 el contador de frecuencias en la posición correspondiente al valor ASCII del carácter.

     Devuelve:
         - No devuelve ningún valor, pero modifica el arreglo frecuencias.
 */
void contarFrecuencias(FILE* p, int* frecuencias) { 
	int caracter; 
	while ((caracter = fgetc(p)) != EOF){ 
		frecuencias[(unsigned char)caracter]++; 
	} 
}

/* 
     La función creaNodo genera un nodo para el Árbol de Huffman.

     Parámetros:
         - ch: Caracter entero que representa el valor ASCII del carácter.
         - valor: Frecuencia o peso asociado al nodo.

     Descripción:
         - Crea un nodo asignando el carácter y la frecuencia correspondiente.
         - Inicializa los punteros a los nodos hijos izquierdo y derecho en NULL, al igual que el puntero al siguiente nodo en la lista.

     Devuelve:
         - Un puntero al nuevo nodo creado (NodoHuffman*).
 */
NodoHuffman* creaNodo(int ch, int valor){
			NodoHuffman* nuevoNodo = (NodoHuffman*)malloc(sizeof(NodoHuffman)); 
			nuevoNodo->valor = valor; 
			nuevoNodo->caracter=(char)ch;
			nuevoNodo->izq = NULL; 
			nuevoNodo->der = NULL; 
			nuevoNodo->sig = NULL; 
			return nuevoNodo;
}


/* 
     La función arbolHuffman construye un Árbol de Huffman a partir de una lista de nodos.

     Parámetros:
         - raiz: Puntero al primer nodo de una lista ordenada de nodos de Huffman.

     Descripción:
         - Combina los dos nodos con menor frecuencia en un nuevo nodo, sumando sus frecuencias y estableciendo
           el nuevo nodo como raíz, y los nodos combinados como hijos.
         - Inserta el nuevo nodo en la lista de forma ordenada y repite el proceso hasta construir el árbol completo.

     Devuelve:
         - El nodo raíz del árbol de Huffman (NodoHuffman*).
 */
NodoHuffman* arbolHuffman (NodoHuffman* raiz){
	if (raiz==NULL || raiz->sig==NULL){
	return raiz;	
	}
	NodoHuffman* siguiente=raiz->sig;
	NodoHuffman* combinado = creaNodo(raiz->valor + siguiente->valor, 0); 
	combinado->izq = raiz; combinado->der = siguiente; 
	// Desplazar la lista para excluir los dos primeros nodos fusionados 
	NodoHuffman* nuevaRaiz = siguiente->sig; 
	combinado->sig = nuevaRaiz; 
	// Insertar el nodo combinado en la nueva posición correcta 
	if (!nuevaRaiz || combinado->valor <= nuevaRaiz->valor) { 
		combinado->sig = nuevaRaiz; 
		return arbolHuffman(combinado); 
		}else{ 
			NodoHuffman* actual = nuevaRaiz; 
			while (actual->sig && actual->sig->valor < combinado->valor) { 
				actual = actual->sig; 
			} 
	combinado->sig = actual->sig; 
	actual->sig = combinado; 
	return arbolHuffman (nuevaRaiz);
}
}

/* 
     La función insertarOrdenado inserta un nodo en una lista de nodos de Huffman de forma ordenada.

     Parámetros:
         - lista: Puntero al primer nodo de la lista de nodos.
         - nuevo: Puntero al nodo que se insertará en la lista.

     Descripción:
         - Inserta el nodo en la lista de forma que se mantenga el orden ascendente según la frecuencia.

     Devuelve:
         - La lista de nodos con el nuevo nodo insertado en la posición adecuada.
 */
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

/* 
     La función asignaArbol genera códigos de Huffman para cada carácter en el árbol.

     Parámetros:
         - raiz: Puntero al nodo raíz del árbol de Huffman.

     Descripción:
         - Recorrido del árbol para asignar un código binario a cada carácter, 
           utilizando ‘0’ para un camino hacia la izquierda y ‘1’ para la derecha.
         - Los códigos generados se guardan en un arreglo de cadenas (codigos).

     Devuelve:
         - No devuelve un valor explícito, pero guarda cada código en la posición correspondiente de codigos.
 */
void asignaArbol(NodoHuffman* raiz){
	if (raiz == NULL){
        return;
    }
    if (raiz->izq == NULL && raiz->der == NULL) {
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
    }

    if (raiz->izq != NULL){
        codigo[longitud] = '0';
        asignaArbol(raiz->izq, codigo, longitud + 1, codigos);
    }

    if (raiz->der != NULL){
        codigo[longitud] = '1';
        asignaArbol(raiz->der, codigo, longitud + 1, codigos);
    }
	
	 if (raiz->izq == NULL && raiz->der == NULL){
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
        printf("Codigo para '%c': %s\n", raiz->caracter, codigo);
    }

	
}


/* 
    La función leerArchivo abre un archivo en modo lectura.
 
    Parámetros:
      - nombreArchivo: Cadena de caracteres que contiene el nombre o ruta del archivo a abrir.
 
    Descripción:
      - Abre el archivo en modo lectura. Si no se puede abrir, devuelve NULL.
 
    Devuelve:
      - Puntero al archivo abierto, o NULL si no pudo abrirse.
 */
FILE* leerArchivo(char* nombreArchivo){
	FILE* p=fopen (nombreArchivo, "r");
	return p;
}

int main (int argc, char *argv[]){
	int i;
	char* nombreArchivo= argv[1];
	FILE* p=leerArchivo(nombreArchivo);
	if (p==NULL){
	fprintf (stderr,"No se pudo abrir el archivo");
		return 0;
	}
	int frecuencias[256] = {0}; 
	contarFrecuencias(p, frecuencias);
	NodoHuffman* lista=NULL;
	for (i=0; i<256; i++){
		if (frecuencias [i]!=0){ 
			lista= insertarOrdenado(lista, creaNodo(i, frecuencias[i]));
		}
	}
	NodoHuffman* arbol= arbolHuffman(lista);
	imprimirArbol (arbol, 0);
	asignaArbol (arbol, nombreArchivo);
}

