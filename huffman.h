#ifndef HUFFMAN_H
#define HUFFMAN_H

// Estructura para un nodo en el Árbol de Huffman
typedef struct NodoHuffman 
{
    char caracter;
    unsigned frecuencia;
    unsigned int valor;
    struct NodoHuffman *izq, *der, *sig;
} NodoHuffman;



#endif