#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"


/* La siguiente función, "contarFrecuencias", recorre un archivo y cuenta la frecuencia de cada carácter.
   
   Parámetros:
       - *p: Puntero al archivo que se va a leer.
       - *frecuencias: Arreglo de enteros donde se almacenarán las frecuencias de cada carácter (indexado por el valor ASCII del carácter).

   Observaciones:
       - La función lee el archivo caracter por caracter hasta llegar al final.
       - Cada carácter leído incrementa el valor en la posición correspondiente en el arreglo `frecuencias`.
       - El índice en `frecuencias` corresponde al valor ASCII del carácter.
       - La función asume que el arreglo `frecuencias` ya está inicializado con ceros.

   Algoritmo:
       - Se abre el archivo y se lee un carácter en cada iteración del bucle.
       - Mientras no se alcance el final del archivo:
           - Se convierte el carácter leído a un índice en el arreglo `frecuencias` y se incrementa su valor.
       - El proceso continúa hasta que el archivo ha sido completamente leído.

   Devuelve:
       - No retorna un valor explícito, pero actualiza el arreglo `frecuencias` con la cantidad de apariciones de cada carácter.
*/
void contarFrecuencias(FILE* p, int* frecuencias) { 
	int caracter; 
	while ((caracter = fgetc(p)) != EOF){ 
		frecuencias[(unsigned char)caracter]++; 
	} 
}

/* La función "creaNodo" crea un nuevo nodo para el árbol de Huffman, con valores especificados para el carácter y su frecuencia.

   Parámetros:
       - ch: Entero que representa el carácter del nodo.
       - valor: Frecuencia o peso asociado con el carácter.

   Observaciones:
       - La función reserva memoria para un nuevo nodo `NodoHuffman`.
       - Inicializa los atributos del nodo con el carácter y su valor, y deja sus punteros `izq`, `der` y `sig` en NULL.
       - Es llamada cada vez que se necesita añadir un nuevo nodo a la lista o al árbol de Huffman.

   Algoritmo:
       - Se reserva memoria para un nuevo nodo.
       - Se asignan el valor y el carácter al nuevo nodo.
       - Los punteros `izq`, `der`, y `sig` del nodo se inicializan en NULL.
       - Devuelve el nodo recién creado para ser enlazado al árbol o lista.

   Devuelve:
       - Un puntero al nuevo nodo de tipo `NodoHuffman` con sus atributos inicializados.
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


/* La función "arbolHuffman" crea un árbol de Huffman a partir de una lista de nodos de caracteres y sus frecuencias.

   Parámetros:
       - *raiz: Puntero a la lista ordenada de nodos por frecuencia, que será transformada en el árbol de Huffman.

   Observaciones:
       - La función combina los dos nodos con menor frecuencia en un nuevo nodo combinado.
       - Este proceso se repite hasta que la lista se convierte en un solo nodo, que es la raíz del árbol.
       - El nodo combinado se inserta de forma ordenada en la lista para mantenerla en orden de frecuencia ascendente.

   Algoritmo:
       - Si la lista tiene uno o cero elementos, devuelve el nodo como raíz.
       - Fusiona los dos primeros nodos en uno nuevo, con frecuencia igual a la suma de ambos.
       - Ajusta la lista para excluir los nodos fusionados y reinserta el nodo combinado en la posición adecuada.
       - Llama a sí misma recursivamente hasta que la lista se convierte en un solo nodo.

   Devuelve:
       - La raíz del árbol de Huffman creado a partir de la lista de frecuencias.
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

/* La función "insertarOrdenado" inserta un nuevo nodo en una lista ordenada de nodos en función de la frecuencia.

   Parámetros:
       - *lista: Puntero al inicio de la lista ordenada de nodos.
       - *nuevo: Puntero al nuevo nodo que se va a insertar en la lista.

   Observaciones:
       - Inserta el nodo `nuevo` en su posición adecuada para mantener la lista ordenada por frecuencia.
       - Si la lista está vacía o el nodo `nuevo` tiene menor o igual frecuencia que el primer nodo, se inserta al inicio.
       - La función recorre la lista hasta encontrar la posición adecuada para el nuevo nodo.

   Algoritmo:
       - Si la lista está vacía o el nodo `nuevo` debe ir al inicio, se inserta al inicio y se devuelve.
       - Si no, se recorre la lista hasta encontrar la posición correcta.
       - Se enlaza el nodo `nuevo` en la lista y se devuelve la lista actualizada.

   Devuelve:
       - La lista con el nuevo nodo insertado en la posición ordenada.
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

/* La función "asignaArbol" asigna códigos binarios a cada carácter en el árbol de Huffman, almacenando los códigos en un arreglo.

   Parámetros:
       - *raiz: Puntero a la raíz del árbol de Huffman.
       - *codigos: Arreglo donde se almacenarán los códigos binarios generados.
       - *codigo: Arreglo temporal para construir los códigos.
       - longitud: Longitud actual del código en construcción.

   Observaciones:
       - La función recorre el árbol y asigna un '0' o '1' al código en función de si se mueve hacia la izquierda o derecha.
       - Cuando encuentra una hoja (carácter), almacena el código completo en `codigos`.
       - Es recursiva y continúa hasta que se asignan códigos a todas las hojas del árbol.

   Algoritmo:
       - Si el nodo es NULL, retorna.
       - Si el nodo es una hoja, termina el código actual y lo guarda en `codigos`.
       - Si el nodo tiene un hijo izquierdo, añade '0' al código y llama recursivamente.
       - Si tiene un hijo derecho, añade '1' al código y llama recursivamente.
       - Continúa hasta completar los códigos para todas las hojas.

   Devuelve:
       - No retorna un valor explícito, pero actualiza el arreglo `codigos` con los códigos binarios para cada carácter.
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

