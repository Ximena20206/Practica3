#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PESOBIT(bpos) (1 << (bpos))
#define CONSULTARBIT(var, bpos) (((*(unsigned*)&var) & PESOBIT(bpos)) ? 1 : 0)
#define PONE_1(var, bpos) (*(unsigned*)&var |= PESOBIT(bpos))
#define PONE_0(var, bpos) (*(unsigned*)&var &= ~(PESOBIT(bpos)))

// Definición del árbol y las estructuras
#define MAX_TREE_HT 100

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Min-heap y operaciones relacionadas (mismas que en la codificación)
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Funciones relacionadas con la min-heap (no se repiten aquí por brevedad)

// Recurre el árbol de Huffman para encontrar el carácter correspondiente a los bits leídos
void decode(FILE *file, struct MinHeapNode *root) {
    struct MinHeapNode *current = root;
    unsigned char byte;
    int bitPos = 0;

    while (fread(&byte, sizeof(byte), 1, file)) {
        // Procesamos cada bit del byte leído
        for (bitPos = 0; bitPos < 8; bitPos++) {
            if (CONSULTARBIT(byte, bitPos)) {
                current = current->right;  // Si es 1, vamos al hijo derecho
            } else {
                current = current->left;   // Si es 0, vamos al hijo izquierdo
            }

            // Si llegamos a una hoja, decodificamos el carácter
            if (!(current->left) && !(current->right)) {
                printf("%c", current->data);
                current = root;  // Reiniciamos al nodo raíz para seguir leyendo
            }
        }
    }
    printf("\n");
}

// Función principal para la decodificación
int main() {
    // Datos de ejemplo del árbol de Huffman (deberías usar los datos reales de la codificación)
    char data[] = {'e', 'j', 'm', 'p', 'l', 'o'};
    int freq[] = {5, 7, 8, 15, 3, 4};  // Frecuencias de ejemplo
    int size = sizeof(data) / sizeof(data[0]);

    // Crear el árbol de Huffman
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Abrir el archivo de codificación
    FILE *file = fopen("codificacion.dat", "rb");
    if (!file) {
        perror("Error al abrir archivo para decodificación");
        return -1;
    }

    // Decodificar y mostrar el resultado
    printf("Texto decodificado:\n");
    decode(file, root);

    fclose(file);
    return 0;
}
