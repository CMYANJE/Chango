#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "booleanas.h"

void mostrarCaratula() {
    printf("=====================================\n");
    printf("  MATEMATICAS DISCRETAS 1\n");
    printf("  NOMBRES: YANDRIX CHANGO\n");
    printf("  Generador de Expresiones Booleanas\n");
    printf("  Año Lectivo: 2024\n");
    printf("=====================================\n\n");
}

int obtenerNumVariables() {
    int n;
    do {
        printf("Digite el número de variables booleanas (2 o 3): ");
        scanf("%d", &n);
        if (n != 2 && n != 3)
            printf(RED "Error: Solo puede ingresar 2 o 3 variables.\n\n" RESET);
    } while (n != 2 && n != 3);
    return n;
}

void ingresarTabla(int n, int salida[]) {
    int filas = (1 << n);
    printf("\nIngrese los valores de salida (0 o 1) para cada fila:\n");
    printf("=====================================\n");
    printf(" A  B");
    if (n == 3) printf("  C");
    printf("  |  Salida\n");
    printf("=====================================\n");

    for (int i = 0; i < filas; i++) {
        int A = (i >> (n-1)) & 1;
        int B = (i >> (n-2)) & 1;
        int C = (n == 3) ? (i & 1) : 0;

        do {
            printf(" %d  %d", A, B);
            if (n == 3) printf("  %d", C);
            printf("  |  ");
            scanf("%d", &salida[i]);
            if (salida[i] != 0 && salida[i] != 1)
                printf(RED "Error: Solo puede ingresar 0 o 1.\n" RESET);
        } while (salida[i] != 0 && salida[i] != 1);
    }
}

void generarExpresion(int n, int salida[]) {
    int filas = (1 << n);
    int primera = 1;
    
    printf("\n" GREEN "=== EXPRESIÓN BOOLEANA GENERADA ===" RESET "\n");
    printf("Forma SOP (Suma de Productos): ");

    for (int i = 0; i < filas; i++) {
        if (salida[i] == 1) {
            if (!primera) printf(" + ");
            primera = 0;

            int A = (i >> (n-1)) & 1;
            int B = (i >> (n-2)) & 1;
            int C = (n == 3) ? (i & 1) : 0;

            printf("(");
            if (A == 0) printf("A'");
            else printf("A");
            
            if (B == 0) printf("B'");
            else printf("B");
            
            if (n == 3) {
                if (C == 0) printf("C'");
                else printf("C");
            }
            printf(")");
        }
    }
    
    if (primera) {
        printf("0 (Función siempre falsa)");
    }
    printf("\n");
}

void imprimirTabla(int n, int salida[]) {
    int filas = (1 << n);

    printf("\n" GREEN "=== TABLA DE VERDAD COMPLETA ===" RESET "\n");
    printf("=====================================\n");
    printf(" A  B");
    if (n == 3) printf("  C");
    printf("  |  Salida  |  Mintérmino\n");
    printf("=====================================\n");

    for (int i = 0; i < filas; i++) {
        int A = (i >> (n-1)) & 1;
        int B = (i >> (n-2)) & 1;
        int C = (n == 3) ? (i & 1) : 0;

        printf(" %d  %d", A, B);
        if (n == 3) printf("  %d", C);
        printf("  |    %d    |     ", salida[i]);
        
        if (salida[i] == 1) {
            printf(GREEN "m%d" RESET, i);
        } else {
            printf("-");
        }
        printf("\n");
    }
    printf("=====================================\n");
}

void generarCircuito(int n, int salida[]) {
    printf("\n" GREEN "=== IMPLEMENTACIÓN EN CIRCUITO LÓGICO ===" RESET "\n");
    printf("Para implementar esta expresión necesitará:\n");
    
    int filas = (1 << n);
    int terminos = 0;
    
    for (int i = 0; i < filas; i++) {
        if (salida[i] == 1) terminos++;
    }
    
    if (terminos == 0) {
        printf("- No se requieren compuertas (salida siempre 0)\n");
        return;
    }
    
    printf("- %d compuertas AND de %d entradas\n", terminos, n);
    printf("- %d compuertas NOT (según necesidad)\n", n);
    if (terminos > 1) {
        printf("- 1 compuerta OR de %d entradas\n", terminos);
    }
    
    printf("\nEstructura del circuito:\n");
    printf("1. Entradas: ");
    for (int i = 0; i < n; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n2. Compuertas NOT para generar complementos\n");
    printf("3. Compuertas AND para cada mintérmino\n");
    printf("4. Compuerta OR final para sumar productos\n");
}

int main() {
    system("clear"); // Para limpiar pantalla
    mostrarCaratula();

    int numVars = obtenerNumVariables();
    int filas = (1 << numVars);
    int salida[filas];

    ingresarTabla(numVars, salida);
    generarExpresion(numVars, salida);
    imprimirTabla(numVars, salida);
    generarCircuito(numVars, salida);

    printf("\n" GREEN "Proyecto completado exitosamente." RESET "\n");
    printf("Presione Enter para salir...");
    getchar();
    getchar();

    return 0;
}
