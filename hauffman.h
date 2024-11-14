#ifndef hauffman_h
#define hauffman_h
typedef struct NodoHuffman {
  int valor;
  char caracter;
  struct NodoHuffman *izq, *der;
}NodoHuffman;
#endif