#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//DECLARACION DE FUNCIONES

/*
void generarCodigosHuffman(NodoHuffman *raiz, char *codigo, int longitud, char codigos[256][256]);
Descripción: Genera los códigos Huffman para cada carácter del texto original a partir del árbol de Huffman construido.
Recibe: NodoHuffman *raiz (raíz del árbol de Huffman), char *codigo (cadena temporal para almacenar el código actual), 
int longitud (longitud actual del código), char codigos[256][256] (matriz para almacenar los códigos de cada carácter).
Devuelve: void.
Observaciones: Esta función recorre el árbol de Huffman y almacena los códigos de cada carácter en la matriz 'codigos'. 
Es esencial para la etapa de codificación del texto original.
*/
void generarCodigosHuffman(NodoHuffman *raiz, char *codigo, int longitud, char codigos[256][256]);

/*
void liberarArbolHuffman(NodoHuffman *raiz);
Descripción: Libera la memoria asignada a todos los nodos del árbol de Huffman.
Recibe: NodoHuffman *raiz (puntero a la raíz del árbol de Huffman).
Devuelve: void.
Observaciones: Esta función es importante para evitar fugas de memoria, especialmente después de completar la codificación o decodificación.
*/
void liberarArbolHuffman(NodoHuffman *raiz);

/*
void generarArchivoCodificacion(const char *cadenaCodificada);
Descripción: Crea un archivo de texto que contiene la cadena de bits resultante de la codificación Huffman del texto original.
Recibe: const char *cadenaCodificada (cadena de caracteres que representa la secuencia de bits de la codificación Huffman).
Devuelve: void.
Observaciones: El archivo generado, "codificacion.txt", se utiliza junto con el archivo de frecuencias para decodificar el mensaje.
*/
void generarArchivoCodificacion(const char *cadenaCodificada);

/*
void generarArchivoFrecuencias(int frecuencias[256]);
Descripción: Genera un archivo de texto con las frecuencias de cada carácter encontrado en el texto original.
Recibe: int frecuencias[256] (arreglo que contiene las frecuencias de los caracteres ASCII).
Devuelve: void.
Observaciones: El archivo generado, denominado "frecuencias.txt", es crucial para el proceso de decodificación, ya que proporciona la información 
necesaria para reconstruir el árbol de Huffman.
*/
void generarArchivoFrecuencias(int frecuencias[256]);

// Función principal
int main() 
{
    int frecuencias[256] = {0};
    NodoHuffman *raiz;
    char codigos[256][256];
    char codigoActual[256];
	char nombreArchivo[50];
	
	printf("Ingrese el nombre del archivo que desea evaluar:\n");
	scanf("%50s", &nombreArchivo);

    // Llama a contarFrecuencias con un archivo de entrada
    contarFrecuencias(nombreArchivo, frecuencias);

    // Construye el árbol de Huffman
    raiz = construirArbolDeHuffman(frecuencias);

    // Inicializa y genera los códigos de Huffman
    memset(codigos, 0, sizeof(codigos));
    generarCodigosHuffman(raiz, codigoActual, 0, codigos);

    // Imprime los códigos de Huffman
    for (int i = 0; i < 256; ++i) 
	{
        if (codigos[i][0] != '\0') 
		{
            printf("Caracter '%c': Codigo %s\n", i, codigos[i]);
        }
    }
	FILE *archivoEntrada = fopen(nombreArchivo, "r");
    if (archivoEntrada == NULL) {
        perror("Error al abrir el archivo de entrada");
        exit(1);
    }

    char ch;
    char cadenaCodificada[10000] = ""; 
    while ((ch = fgetc(archivoEntrada)) != EOF) 
	{
        strcat(cadenaCodificada, codigos[(unsigned char)ch]);
    }
    fclose(archivoEntrada);

    // Guardar la cadena codificada y las frecuencias en archivos
    generarArchivoCodificacion(cadenaCodificada);
    generarArchivoFrecuencias(frecuencias);

    // Libera la memoria del árbol de Huffman
    liberarArbolHuffman(raiz);

    return 0;
}
// Función para generar los códigos de Huffman
/*
void generarCodigosHuffman(NodoHuffman *raiz, char *codigo, int longitud, char codigos[256][256]);
Descripción: Genera los códigos Huffman para cada carácter del texto original a partir del árbol de Huffman construido.
Recibe: NodoHuffman *raiz (raíz del árbol de Huffman), char *codigo (cadena temporal para almacenar el código actual), int longitud (longitud actual del código), char codigos[256][256] (matriz para almacenar los códigos de cada carácter).
Devuelve: void.
Observaciones: Esta función recorre el árbol de Huffman y almacena los códigos de cada carácter en la matriz 'codigos'. Es esencial para la etapa de codificación del texto original.
*/
void generarCodigosHuffman(NodoHuffman *raiz, char *codigo, int longitud, char codigos[256][256]) 
{
    if (raiz == NULL) 
	{
        return;
    }

    // Si es una hoja, almacena el código
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

}

/*
void liberarArbolHuffman(NodoHuffman *raiz);
Descripción: Libera la memoria asignada a todos los nodos del árbol de Huffman.
Recibe: NodoHuffman *raiz (puntero a la raíz del árbol de Huffman).
Devuelve: void.
Observaciones: Esta función es importante para evitar fugas de memoria, especialmente después de completar la codificación o decodificación.
*/
void liberarArbolHuffman(NodoHuffman *raiz) 
{
    if (raiz == NULL) return;
    liberarArbolHuffman(raiz->izquierda);
    liberarArbolHuffman(raiz->derecha);
    free(raiz);
}

/*
void generarArchivoCodificacion(const char *cadenaCodificada);
Descripción: Crea un archivo de texto que contiene la cadena de bits resultante de la codificación Huffman del texto original.
Recibe: const char *cadenaCodificada (cadena de caracteres que representa la secuencia de bits de la codificación Huffman).
Devuelve: void.
Observaciones: El archivo generado, "codificacion.txt", se utiliza junto con el archivo de frecuencias para decodificar el mensaje.
*/
void generarArchivoCodificacion(const char *cadenaCodificada) 
{
    FILE *archivo = fopen("codificacion.txt", "w");
    if (archivo == NULL) 
	{
        perror("Error al crear el archivo de codificación");
        exit(1);
    }

    fprintf(archivo, "%s", cadenaCodificada);
    fclose(archivo);
}

/*
void generarArchivoFrecuencias(int frecuencias[256]);
Descripción: Genera un archivo de texto con las frecuencias de cada carácter encontrado en el texto original.
Recibe: int frecuencias[256] (arreglo que contiene las frecuencias de los caracteres ASCII).
Devuelve: void.
Observaciones: El archivo generado, denominado "frecuencias.txt", es crucial para el proceso de decodificación, ya que proporciona la información necesaria para reconstruir el árbol de Huffman.
*/
void generarArchivoFrecuencias(int frecuencias[256]) 
{
    FILE *archivo = fopen("frecuencias.txt", "w");
    if (archivo == NULL) {
        perror("Error al crear el archivo de frecuencias");
        exit(1);
    }

    for (int i = 0; i < 256; i++) 
	{
        if (frecuencias[i] != 0) {
            fprintf(archivo, "%c %d\n", (char)i, frecuencias[i]);
        }
    }

    fclose(archivo);
}



