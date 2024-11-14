#include <stdio.h>
#include <string.h>
#include "TADListaSL.h"
#include "hauffman.h"

#define caracteres 256
lista ocurrencias;

void LlenarLista(int *registro) {
    elemento aux;
    Initialize(&ocurrencias);

    for (int i = 0; i < caracteres; i++) {
        if (registro[i] == 0)
            continue;

        aux.n = registro[i];
        aux.c = (char)i;

        if (Empty(&ocurrencias)) {
            Add(&ocurrencias, aux);
        } else if (Position(&ocurrencias, First(&ocurrencias)).n > aux.n) {
            Insert(&ocurrencias, First(&ocurrencias), aux, FALSE);
        } else {
            posicion auxp = First(&ocurrencias);
            elemento auxcomp = Position(&ocurrencias, auxp);

            while (auxp != NULL && aux.n < auxcomp.n) {
                auxp = Following(&ocurrencias, auxp);
                if (auxp != NULL)
                    auxcomp = Position(&ocurrencias, auxp);
            }
            if (auxp == NULL)
                Add(&ocurrencias, aux);
            else
                Insert(&ocurrencias, auxp, aux, FALSE);
        }
    }
}

void contarFrecuencias(FILE* p) {
    int frecuencias[caracteres] = {0};
    int c;

    // Recorre el archivo para contar frecuencias
    while ((c = fgetc(p)) != EOF) {
        frecuencias[(unsigned char)c]++;
    }

    // Muestra las frecuencias de los caracteres
    for (int i = 0; i < caracteres; i++) {
        if (frecuencias[i] > 0)
            printf("Caracter '%c' (ASCII %d): %d veces\n", i, i, frecuencias[i]);
    }

    // Llama a LlenarLista para almacenar en la lista
    LlenarLista(frecuencias);
}

// Compilación: gcc haufman.c hauffman.h -o hauffman
// Ejecución: ./hauffman nombreArchivo
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Falta el nombre del archivo.\n");
        return 1;
    }

    char* nombreArchivo = argv[1];
    FILE* p = fopen(nombreArchivo, "r");
    if (p == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo\n");
        return 1;
    }

    contarFrecuencias(p);
    fclose(p);

    // Recorre la lista para imprimir los elementos
    posicion po = First(&ocurrencias);
    for (int i = 0; i < Size(&ocurrencias); i++) {
        elemento a = Position(&ocurrencias, po);
        printf("\nFrecuencia: %d, Caracter: %c", a.n, a.c);
        po = Following(&ocurrencias, po);
    }

    Destroy(&ocurrencias);
    return 0;
}

