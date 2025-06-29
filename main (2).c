#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Versión recursiva básica
int comb_recursiva(int n, int k) {
    if (k == 0 || k == n) return 1;
    if (k < 0 || k > n) return 0; // Validación adicional
    return comb_recursiva(n-1, k-1) + comb_recursiva(n-1, k);
}

// Versión iterativa más eficiente
long long comb_iterativa(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n-k) k = n-k; // Aprovechamos la propiedad de simetría
    
    long long res = 1;
    for (int i = 1; i <= k; i++) {
        res = res * (n - k + i) / i;
    }
    return res;
}

// Versión con memoización (tabla de resultados)
#define MAX_N 30
long long tabla[MAX_N][MAX_N];

void inicializar_tabla() {
    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++)
            tabla[i][j] = -1;
}

long long comb_memoizacion(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (n >= MAX_N || k >= MAX_N) return 0; // Verificar límites
    
    if (tabla[n][k] != -1) return tabla[n][k];
    tabla[n][k] = comb_memoizacion(n-1, k-1) + comb_memoizacion(n-1, k);
    return tabla[n][k];
}

// Función para imprimir el triángulo de Pascal
void imprimir_triangulo_pascal(int filas) {
    printf("\nTriángulo de Pascal con %d filas:\n", filas);
    printf("================================\n");
    
    for (int n = 0; n < filas; n++) {
        // Espacios para centrar
        for (int esp = 0; esp < filas - n - 1; esp++) {
            printf("   ");
        }
        
        for (int k = 0; k <= n; k++) {
            printf("%6lld", comb_iterativa(n, k));
        }
        printf("\n");
    }
}

// Función para calcular una fila completa del triángulo
void imprimir_fila_pascal(int n) {
    printf("\nFila %d del triángulo de Pascal:\n", n);
    printf("Posición: ");
    for (int k = 0; k <= n; k++) {
        printf("%6d", k);
    }
    printf("\nValor:    ");
    for (int k = 0; k <= n; k++) {
        printf("%6lld", comb_iterativa(n, k));
    }
    printf("\n");
}

// Función para demostrar propiedades del triángulo de Pascal
void demostrar_propiedades() {
    printf("\n=== PROPIEDADES DEL TRIÁNGULO DE PASCAL ===\n");
    
    int n = 6;
    printf("\n1. SIMETRÍA - Fila %d:\n", n);
    for (int k = 0; k <= n; k++) {
        long long val1 = comb_iterativa(n, k);
        long long val2 = comb_iterativa(n, n-k);
        printf("   C(%d,%d) = %lld = C(%d,%d) = %lld\n", n, k, val1, n, n-k, val2);
    }
    
    printf("\n2. SUMA DE FILA - Las sumas de cada fila son potencias de 2:\n");
    for (int i = 0; i <= 5; i++) {
        long long suma = 0;
        for (int j = 0; j <= i; j++) {
            suma += comb_iterativa(i, j);
        }
        printf("   Fila %d: suma = %lld = 2^%d\n", i, suma, i);
    }
    
    printf("\n3. IDENTIDAD DE PASCAL:\n");
    printf("   C(n,k) = C(n-1,k-1) + C(n-1,k)\n");
    for (int i = 2; i <= 5; i++) {
        for (int j = 1; j < i; j++) {
            long long izq = comb_iterativa(i, j);
            long long der1 = comb_iterativa(i-1, j-1);
            long long der2 = comb_iterativa(i-1, j);
            printf("   C(%d,%d) = %lld = C(%d,%d) + C(%d,%d) = %lld + %lld\n", 
                   i, j, izq, i-1, j-1, i-1, j, der1, der2);
        }
    }
}

// Función para comparar métodos con diferentes tamaños
void comparar_metodos() {
    printf("\n=== COMPARACIÓN DE MÉTODOS ===\n");
    
    int casos[][2] = {{10, 5}, {15, 7}, {20, 10}, {25, 12}};
    int num_casos = sizeof(casos) / sizeof(casos[0]);
    
    printf("%-10s %-12s %-12s %-12s %-12s\n", "Caso", "Iterativo", "Memoización", "Recursivo", "Tiempo(ms)");
    printf("---------------------------------------------------------------\n");
    
    for (int i = 0; i < num_casos; i++) {
        int n = casos[i][0];
        int k = casos[i][1];
        
        // Reinicializar tabla
        inicializar_tabla();
        
        // Método iterativo
        clock_t inicio = clock();
        long long res_iter = comb_iterativa(n, k);
        clock_t fin = clock();
        double tiempo_iter = ((double)(fin - inicio)) / CLOCKS_PER_SEC * 1000;
        
        // Método con memoización
        inicio = clock();
        long long res_memo = comb_memoizacion(n, k);
        fin = clock();
        double tiempo_memo = ((double)(fin - inicio)) / CLOCKS_PER_SEC * 1000;
        
        // Método recursivo (solo para casos pequeños)
        long long res_rec = 0;
        double tiempo_rec = 0;
        if (n <= 12) { // Límite para evitar tiempos excesivos
            inicio = clock();
            res_rec = comb_recursiva(n, k);
            fin = clock();
            tiempo_rec = ((double)(fin - inicio)) / CLOCKS_PER_SEC * 1000;
        }
        
        printf("C(%2d,%2d)  %-12lld %-12lld", n, k, res_iter, res_memo);
        if (n <= 12) {
            printf("%-12lld %.3f/%.3f\n", res_rec, tiempo_iter, tiempo_memo);
        } else {
            printf("%-12s %.3f/%.3f\n", "N/A", tiempo_iter, tiempo_memo);
        }
    }
}

// Función interactiva para que el usuario ingrese valores
void modo_interactivo() {
    int opcion;
    int n, k, filas;
    
    do {
        printf("\n=== MODO INTERACTIVO ===\n");
        printf("1. Calcular C(n,k) específico\n");
        printf("2. Mostrar fila completa del triángulo\n");
        printf("3. Mostrar triángulo de Pascal\n");
        printf("4. Volver al menú principal\n");
        printf("Selecciona una opción: ");
        
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
                printf("Ingresa n: ");
                scanf("%d", &n);
                printf("Ingresa k: ");
                scanf("%d", &k);
                
                if (n < 0 || k < 0 || k > n) {
                    printf("Error: Los valores deben cumplir 0 ≤ k ≤ n\n");
                    break;
                }
                
                printf("\nResultados para C(%d,%d):\n", n, k);
                printf("Método iterativo: %lld\n", comb_iterativa(n, k));
                
                inicializar_tabla();
                printf("Método memoización: %lld\n", comb_memoizacion(n, k));
                
                if (n <= 15) {
                    printf("Método recursivo: %d\n", comb_recursiva(n, k));
                } else {
                    printf("Método recursivo: N/A (demasiado lento para n>15)\n");
                }
                break;
                
            case 2:
                printf("Ingresa el número de fila (n): ");
                scanf("%d", &n);
                if (n >= 0 && n < 20) {
                    imprimir_fila_pascal(n);
                } else {
                    printf("Error: n debe estar entre 0 y 19\n");
                }
                break;
                
            case 3:
                printf("Ingresa el número de filas: ");
                scanf("%d", &filas);
                if (filas > 0 && filas <= 15) {
                    imprimir_triangulo_pascal(filas);
                } else {
                    printf("Error: número de filas debe estar entre 1 y 15\n");
                }
                break;
                
            case 4:
                printf("Volviendo al menú principal...\n");
                break;
                
            default:
                printf("Opción no válida\n");
        }
    } while (opcion != 4);
}

int main() {
    int opcion_principal;
    
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║     IMPLEMENTACIÓN DE LA REGLA DE PASCAL     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    do {
        printf("\n=== MENÚ PRINCIPAL ===\n");
        printf("1. Ejemplos básicos\n");
        printf("2. Mostrar triángulo de Pascal\n");
        printf("3. Demostrar propiedades\n");
        printf("4. Comparar métodos de cálculo\n");
        printf("5. Modo interactivo\n");
        printf("6. Salir\n");
        printf("Selecciona una opción: ");
        
        scanf("%d", &opcion_principal);
        
        switch(opcion_principal) {
            case 1:
                printf("\n=== EJEMPLOS BÁSICOS ===\n");
                
                // Ejemplo con versión recursiva
                printf("\nVersión recursiva (casos pequeños):\n");
                printf("C(4, 2) = %d\n", comb_recursiva(4, 2));
                printf("C(5, 3) = %d\n", comb_recursiva(5, 3));
                printf("C(6, 2) = %d\n", comb_recursiva(6, 2));
                
                // Ejemplo con versión iterativa
                printf("\nVersión iterativa (más eficiente):\n");
                printf("C(10, 5) = %lld\n", comb_iterativa(10, 5));
                printf("C(15, 7) = %lld\n", comb_iterativa(15, 7));
                printf("C(20, 10) = %lld\n", comb_iterativa(20, 10));
                
                // Ejemplo con memoización
                inicializar_tabla();
                printf("\nVersión con memoización:\n");
                printf("C(12, 6) = %lld\n", comb_memoizacion(12, 6));
                printf("C(18, 9) = %lld\n", comb_memoizacion(18, 9));
                printf("C(25, 12) = %lld\n", comb_memoizacion(25, 12));
                break;
                
            case 2:
                imprimir_triangulo_pascal(10);
                break;
                
            case 3:
                demostrar_propiedades();
                break;
                
            case 4:
                comparar_metodos();
                break;
                
            case 5:
                modo_interactivo();
                break;
                
            case 6:
                printf("\n¡Gracias por usar el programa!\n");
                printf("Desarrollado para aprender sobre la Regla de Pascal\n");
                break;
                
            default:
                printf("Opción no válida. Por favor selecciona del 1 al 6.\n");
        }
        
    } while (opcion_principal != 6);
    
    return 0;
}