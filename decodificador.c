#include <stdio.h>
#include <string.h> // Para la manipulación del documento
#include <stdlib.h>

// Definicion de la estructra de cada nodo del arbol de Huffman
typedef struct Nodo {
    char letra;
    int frecuencia;
    struct Nodo *izquierda;
    struct Nodo *derecha;
} *Nodo; // Nodos definidos

// Variables globales para la codificación
int n_nodos = 0; // Contador de nodos
int qend = 0; // Final de la queue
struct Nodo quantity[256] = {{0}}; // Arreglo de nodos para la codificación
Nodo prim[255], *q = prim - 1; // Cola de prioridad
char let[1024]; // Arreglo de letras para la encriptación de cada letra

//Función para crear un nuevo nodo
Nodo nuevo_nodo(int freq, char let, Nodo izq, Nodo der) {
    Nodo n = quantity + n_nodos++;
    if(freq != 0) {
        n -> letra = let;
        n -> frecuencia = freq;
    }
    else {
        n -> izquierda = izq;
        n -> derecha = der;
        n -> frecuencia = izq -> frecuencia + der -> frecuencia;
    }
    return n;
}

//Función paraa insertar un nodo en la cola de prioridad
void insertar_nodo(Nodo n) {
    int j, i = qend ++;
    while ((j = i/2)){
        if (q[j] -> frecuencia <= n -> frecuencia) break;
        q[i] = q[j];
        i = j;
    }
    q[i] = n;
}

//Función para sacar un nodo de la cola de prioridad
Nodo sacar_nodo() {
    int i, l;
    Nodo n = q[i = 1];

    if (qend < 2) return 0;
    qend--;

    while((l = i * 2) < qend) {
        if (l + 1 < qend && q[l + 1] -> frecuencia < q[l] -> frecuencia) l++;
        q[i] = q[l];
        i = l;
    }
    q[i] = q[qend];

    return n;
}

//Función para importar tabla
void import_tabla(FILE *f, unsigned int *freq) {
    char c;
    int i = 0;

    while ((c = fgetc(f)) != EOF) { //Toma los valores de la tabla hasta que esta se acabe
        freq[i++] = (unsigned char) c;
    }

    for (i = 0; i < 128; i++) {
        if (freq[i]) insertar_nodo(nuevo_nodo(freq[i], i, 0, 0)); // Insertar nodos en el árbol de frecuencias
    }
    while (qend > 2) {
        insertar_nodo(nuevo_nodo(0, 0, sacar_nodo(), sacar_nodo())); // Crea el árbol por primera vez
    }
}

void decodificar(FILE *f, FILE *out) {
    int i = 0, lim = 0, j = 0;
    char c;
    Nodo n = q[1];

    fscanf(f, "%d", &lim); // Tamaño del archivo en la primera linea
    fseek(f, 1, SEEK_CUR); // Saltar el \n

    printf("Decodificación:\n");

    for (i = 0; i < lim; i++) {
        if(j == 0){
            c = fgetc(f);
        }
        if (c&128) n = n -> derecha;
        else n = n -> izquierda;
        if (n -> letra) {
            putchar(n -> letra);
            fputc(n -> letra, out);
            n = q[1];
        }
        c = c << 1;
        if (++j > 7){
            j = 0;
        }

        putchar('\n');
        if (q[1] != n) printf ("Error\n");
    }
}

int main(int argc, char *argv[]) {
    FILE *ftab, *fhuf, *fout; // Punteros a los archivos
    unsigned int freq[128] = {0}; // Arreglo de frecuencias

    if (argc != 1) {
        printf("Error\n");
        return 0;
    }

    ftab = fopen("frecuencias.txt", "r"); // Abrir el archivo de tabla
    fhuf = fopen("codificacion.dat", "r"); // Abrir el archivo de codificación

    import_tabla(ftab, freq); // Importar la tabla de frecuencias

    fout = fopen(argv[1], "w"); // Abrir el archivo de salida
    decodificar(fhuf, fout); // Decodificar el archivo

    fclose(ftab); // Cerrar los archivos
    fclose(fhuf);
    fclose(fout);

    return 0;
}