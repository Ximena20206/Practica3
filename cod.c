#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PESOBIT(bpos) (1 << (bpos))
#define CONSULTARBIT(var, bpos) (((*(unsigned*)&var) & PESOBIT(bpos)) ? 1 : 0)
#define PONE_1(var, bpos) (*(unsigned*)&var |= PESOBIT(bpos))
#define PONE_0(var, bpos) (*(unsigned*)&var &= ~(PESOBIT(bpos)))

#define MAX_TREE_HT 100

// Estructura de un nodo en el árbol de Huffman
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Estructura para una min-heap (mínima cola de prioridad)
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Función para crear un nuevo nodo de min-heap
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->data = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

// Función para crear una min-heap de cierta capacidad
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Intercambio de dos nodos de min-heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Reordenamiento en la min-heap
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extrae el nodo con frecuencia mínima
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Inserta un nuevo nodo en la min-heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Construye la min-heap y la inicializa con los datos de frecuencia
struct MinHeap* buildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    for (int i = (minHeap->size - 2) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
    return minHeap;
}

// Construcción del árbol de Huffman
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = buildMinHeap(data, freq, size);
    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Almacena los códigos de Huffman en un arreglo
void storeCodes(struct MinHeapNode* root, int arr[], int top, int codes[256][MAX_TREE_HT], int codeLengths[256]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes, codeLengths);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes, codeLengths);
    }
    if (!(root->left) && !(root->right)) {
        codeLengths[(unsigned char)root->data] = top;
        for (int i = 0; i < top; i++)
            codes[(unsigned char)root->data][i] = arr[i];
    }
}

// Genera los códigos de Huffman y almacena las longitudes
void generateCodes(char data[], int freq[], int size, int codes[256][MAX_TREE_HT], int codeLengths[256]) {
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    storeCodes(root, arr, top, codes, codeLengths);
}

// Función para codificar la cadena de entrada utilizando los códigos generados
void encode(char str[], int codes[256][MAX_TREE_HT], int codeLengths[256]) {
    FILE *file = fopen("codificacion.dat", "wb");
    if (!file) {
        perror("Error al abrir archivo para codificación");
        return;
    }
    unsigned char byte = 0;
    int bitPos = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char ch = str[i];
        for (int j = 0; j < codeLengths[ch]; j++) {
            if (codes[ch][j] == 1) {
                PONE_1(byte, bitPos);
            } else {
                PONE_0(byte, bitPos);
            }
            bitPos++;
            if (bitPos == 8) {
                fwrite(&byte, sizeof(byte), 1, file);
                byte = 0;
                bitPos = 0;
            }
        }
    }
    if (bitPos > 0) {
        fwrite(&byte, sizeof(byte), 1, file);
    }
    fclose(file);
}

// Función principal
int main() {
    char str[] = "ejemplo de huffman";
    int freq[] = {5, 7, 8, 15, 3, 4}; // Frecuencias de los caracteres (simulación)
    char data[] = {'e', 'j', 'm', 'p', 'l', 'o'}; // Caracteres correspondientes a las frecuencias
    int size = sizeof(data) / sizeof(data[0]);

    int codes[256][MAX_TREE_HT] = {0};
    int codeLengths[256] = {0};

    generateCodes(data, freq, size, codes, codeLengths);
    encode(str, codes, codeLengths);

    printf("Codificación completada en 'codificacion.dat'\n");
    return 0;
}
