#include <stdio.h>
#include <stdlib.h>


int main() 
{
    FILE *archivo;
    char ch;
    int contadorBits = 0;
    archivo = fopen("codificacion.txt", "r");
    if (archivo == NULL) 
    {
        printf("Error al abrir el archivo");
        return 1;
    }
    while ((ch = fgetc(archivo)) != EOF) 
    {
        if (ch == '0' || ch == '1') 
        {
            contadorBits++;
        }
    }

    fclose(archivo);

    // Calcula el tamaño en bytes
    int tamanoBytes = contadorBits / 8;
    // Calcula el tamaño en kilobytes
    float tamanoKB = (float)tamanoBytes / 1024;
    
    printf("El tamano de la codificacion en bytes es: %d\n", tamanoBytes);
    printf("El tamano de la codificacion en kilobytes es: %.2f KB\n", tamanoKB);

    return 0;
}
