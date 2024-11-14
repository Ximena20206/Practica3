#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"

void contarFrecuencias(FILE* p, int* frecuencias) { 
	int caracter; 
	while ((caracter = fgetc(p)) != EOF){ 
		frecuencias[(unsigned char)caracter]++; 
	} 
}
NodoHuffman* creaNodo(int ch, int valor){
			NodoHuffman* nuevoNodo = (NodoHuffman*)malloc(sizeof(NodoHuffman)); 
			nuevoNodo->valor = valor; 
			nuevoNodo->caracter=(char)ch;
			nuevoNodo->izq = NULL; 
			nuevoNodo->der = NULL; 
			nuevoNodo->sig = NULL; 
			return nuevoNodo;
}
			
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
void asignaArbol(NodoHuffman* raiz){
	if (raiz == NULL){
        return;
    }
    if (raiz->izquierda == NULL && raiz->derecha == NULL) {
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
    }

    if (raiz->izquierda != NULL){
        codigo[longitud] = '0';
        asignaArbol(raiz->izquierda, codigo, longitud + 1, codigos);
    }

    if (raiz->derecha != NULL){
        codigo[longitud] = '1';
        asignaArbol(raiz->derecha, codigo, longitud + 1, codigos);
    }
	
	 if (raiz->izquierda == NULL && raiz->derecha == NULL){
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
        printf("Codigo para '%c': %s\n", raiz->caracter, codigo);
    }

	
}
void grabaCompresion(){
	
}


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
