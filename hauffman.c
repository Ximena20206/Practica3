#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "TADListaSL.h"
#define caracteres 256
lista ocurrencias;
//void almecenarCola ()
//	
//}
void contarFrecuencias(FILE* p, int* frecuencias) { 
	int caracter; 
	while ((caracter = fgetc(p)) != EOF){ 
		frecuencias[(unsigned char)caracter]++; 
	} 
}
/*NodoHuffman* arbolHuffman (NodoHuffman* raiz){
	if (raiz!=NULL || raiz->sig==NULL){
	return raiz;	
	}
	NodoHuffman* siguiente=raiz->sig;
	NodoHuffman* combinado = crearNodo(raiz->valor + siguiente->valor); 
	combinado->izq = raiz; combinado->der = siguiente; 
	// Desplazar la lista para excluir los dos primeros nodos fusionados 
	NodoHuffman* nuevaRaiz = siguiente->sig; 
	combinado->sig = nuevaRaiz; 
	// Insertar el nodo combinado en la nueva posición correcta 
	if (!nuevaRaiz || combinado->valor <= nuevaRaiz->valor) { 
		combinado->sig = nuevaRaiz; return arbolHuffman(combinado); 
		}else{ 
			NodoHuffman* actual = nuevaRaiz; 
			while (actual->sig && actual->sig->valor < combinado->valor) { 
				actual = actual->sig; 
			} 
	combinado->sig = actual->sig; 
	actual->sig = combinado; 
	return arbolHuffman (nuevaRaiz);
}
}*/
void Llenarlista(int *registro){
    lista ocurrencias;
    elemento aux;

    Initialize(&ocurrencias);
    for(int i=0; i<caracteres ; i++){
        if(registro[i]==0)
            continue;
		aux.n=registro[i];
        aux.c=(char)i;
		
        if(Empty(&ocurrencias)){//anadimos el primer elemento a la lista
		    Add(&ocurrencias, aux);
        }else if(Position(&ocurrencias, First(&ocurrencias)).n > aux.n){
			//si el actual es mayor al de enfrente
			Insert(&ocurrencias , First(&ocurrencias) , aux ,FALSE);
		}else{
            posicion auxp= First(&ocurrencias);
            elemento auxcomp= Position(&ocurrencias, auxp);

            while(auxp!=NULL && aux.n > auxcomp.n){
                auxp=Following(&ocurrencias, auxp);
                if (auxp != NULL) 
                    auxcomp = Position(&ocurrencias, auxp);
                
            }
			if(auxp==NULL)
				Add(&ocurrencias, aux);
			else
             	Insert(&ocurrencias , auxp , aux ,FALSE);
            
        }
		
	}
//return ocurrencias;
}

/*void asignaArbol(){
	if (raiz == NULL){
        return;
    }
    // Si es una hoja, almacena el cÃ³digo
    if (raiz->izquierda == NULL && raiz->derecha == NULL) 
	{
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
    }

    // Recorrer a la izquierda (agrega '0')
    if (raiz->izquierda != NULL) 
	{
        codigo[longitud] = '0';
        generarCodigosHuffman(raiz->izquierda, codigo, longitud + 1, codigos);
    }

    // Recorrer a la derecha (agrega '1')
    if (raiz->derecha != NULL) 
	{
        codigo[longitud] = '1';
        generarCodigosHuffman(raiz->derecha, codigo, longitud + 1, codigos);
    }
	
	 if (raiz->izquierda == NULL && raiz->derecha == NULL) 
	{
        codigo[longitud] = '\0';
        strcpy(codigos[(unsigned char)raiz->caracter], codigo);
        printf("Codigo para '%c': %s\n", raiz->caracter, codigo);
    }

	
}*/
void grabaCompresion(){
	
}


FILE* leerArchivo(char* nombreArchivo){
	FILE* p=fopen (nombreArchivo, "r");
	return p;
}
//compilacion gcc haufman.c hauffman.h-o hauffman 
//ejecucion /.hauffman nombreArchivo
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
	for (i = 0; i < 256; i++) { 
		if (frecuencias[i] > 0) 
		printf("Caracter '%c' (ASCII %d): %d veces\n", i, i, frecuencias[i]); 
	}
	Llenarlista (frecuencias);
	//printf ("%d", ocurrencias->tam);
	posicion po=First(&ocurrencias);
	printf ("LISTA:\n");
	printf ("tamaño lista %d ", Size (&ocurrencias));
	for(i=0;i<Size(&ocurrencias);i++){
		elemento a=Position(&ocurrencias,po);
		printf("\n%d, %c",a.n, a.c);
		po=Following(&ocurrencias,po);
	}
	//arbolHuffman(First (ocurrencias));
	//asignaArbol
}
