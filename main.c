#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

// Función para convertir a minúsculas (búsqueda insensible a mayúsculas)
void aMinusculas(char *str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Función para limpiar el buffer de entrada
void limpiarBuffer() {
    while(getchar() != '\n');
}

// Función para mostrar el menú principal
void mostrarMenu() {
    printf("\n=== MENÚ PRINCIPAL ===\n");
    printf("1. Ingresar productos\n");
    printf("2. Calcular producción\n");
    printf("3. Editar producto\n");
    printf("4. Eliminar producto\n");
    printf("5. Mostrar todos los productos\n");
    printf("6. Salir\n");
}

// Función para ingresar los límites de la fábrica
void ingresarLimites(int *tiempo, int *recursos) {
    printf("\n=== LÍMITES DE PRODUCCIÓN ===\n");
    
    do {
        printf("Tiempo disponible (horas): ");
        scanf("%d", tiempo);
        if(*tiempo <= 0) printf("Error: Debe ser positivo\n");
    } while(*tiempo <= 0);
    
    do {
        printf("Recursos disponibles: ");
        scanf("%d", recursos);
        if(*recursos <= 0) printf("Error: Debe ser positivo\n");
    } while(*recursos <= 0);
    
    limpiarBuffer();
}

// Función para ingresar datos de productos
void ingresarProductos(char nombres[][MAX_NOMBRE], int cantidades[], int tiempos[], int recursos[]) {
    printf("\n=== INGRESO DE PRODUCTOS ===\n");
    
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        printf("\nProducto %d:\n", i+1);
        
        // Nombre
        printf("Nombre: ");
        fgets(nombres[i], MAX_NOMBRE, stdin);
        nombres[i][strcspn(nombres[i], "\n")] = '\0';
        
        // Validación de datos numéricos
        do {
            printf("Cantidad demandada: ");
            scanf("%d", &cantidades[i]);
            if(cantidades[i] < 0) printf("Error: No puede ser negativo\n");
        } while(cantidades[i] < 0);
        
        do {
            printf("Tiempo por unidad (horas): ");
            scanf("%d", &tiempos[i]);
            if(tiempos[i] <= 0) printf("Error: Debe ser positivo\n");
        } while(tiempos[i] <= 0);
        
        do {
            printf("Recursos por unidad: ");
            scanf("%d", &recursos[i]);
            if(recursos[i] <= 0) printf("Error: Debe ser positivo\n");
        } while(recursos[i] <= 0);
        
        limpiarBuffer();
    }
}

// Función para buscar producto (retorna índice o -1 si no encuentra)
int buscarProducto(char nombres[][MAX_NOMBRE], const char *nombreBuscado) {
    char nombreTemp[MAX_NOMBRE];
    
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        strcpy(nombreTemp, nombres[i]);
        aMinusculas(nombreTemp);
        
        char busquedaTemp[MAX_NOMBRE];
        strcpy(busquedaTemp, nombreBuscado);
        aMinusculas(busquedaTemp);
        
        if(strstr(nombreTemp, busquedaTemp) != NULL) {
            return i; // Retorna el índice si encuentra coincidencia
        }
    }
    return -1; // No encontrado
}

// Función para calcular y mostrar los resultados
void calcularProduccion(char nombres[][MAX_NOMBRE], int cantidades[], int tiempos[], int recursos[], int tiempoDisp, int recursosDisp) {
    int total_tiempo = 0;
    int total_recursos = 0;
    int productos_registrados = 0;
    
    printf("\n=== RESUMEN DE PRODUCCIÓN ===\n");
    
    // Calcular totales y contar productos válidos
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        if(strlen(nombres[i]) > 0) { // Si el nombre no está vacío
            total_tiempo += cantidades[i] * tiempos[i];
            total_recursos += cantidades[i] * recursos[i];
            productos_registrados++;
        }
    }
    
    // Mostrar resultados
    printf("Productos registrados: %d\n", productos_registrados);
    printf("Tiempo requerido: %d/%d horas\n", total_tiempo, tiempoDisp);
    printf("Recursos necesarios: %d/%d\n", total_recursos, recursosDisp);
    
    // Determinar viabilidad
    if(productos_registrados == 0) {
        printf("\nNo hay productos registrados!\n");
    } else if(total_tiempo <= tiempoDisp && total_recursos <= recursosDisp) {
        printf("\n✅ PRODUCCIÓN VIABLE\n");
        printf("Recursos sobrantes: %d\n", recursosDisp - total_recursos);
        printf("Tiempo disponible: %d horas\n", tiempoDisp - total_tiempo);
    } else {
        printf("\n❌ NO SE PUEDE CUMPLIR LA DEMANDA\n");
        if(total_tiempo > tiempoDisp) {
            printf("- Faltan %d horas de producción\n", total_tiempo - tiempoDisp);
        }
        if(total_recursos > recursosDisp) {
            printf("- Faltan %d unidades de recursos\n", total_recursos - recursosDisp);
        }
    }
}

// Función para editar un producto existente
void editarProducto(char nombres[][MAX_NOMBRE], int cantidades[], int tiempos[], int recursos[]) {
    char nombreBusqueda[MAX_NOMBRE];
    int indice;
    
    printf("\n=== EDITAR PRODUCTO ===\n");
    
    if(strlen(nombres[0]) == 0) {
        printf("No hay productos registrados!\n");
        return;
    }
    
    printf("Ingrese nombre del producto a editar: ");
    fgets(nombreBusqueda, MAX_NOMBRE, stdin);
    nombreBusqueda[strcspn(nombreBusqueda, "\n")] = '\0';
    
    indice = buscarProducto(nombres, nombreBusqueda);
    
    if(indice == -1) {
        printf("Producto no encontrado!\n");
    } else {
        printf("\nEditando producto: %s\n", nombres[indice]);
        
        // Editar nombre
        printf("Nuevo nombre [%s]: ", nombres[indice]);
        char nuevoNombre[MAX_NOMBRE];
        fgets(nuevoNombre, MAX_NOMBRE, stdin);
        if(strlen(nuevoNombre) > 1) { // Si el usuario ingresó algo
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            strcpy(nombres[indice], nuevoNombre);
        }
        
        // Editar cantidad
        printf("Nueva cantidad [%d]: ", cantidades[indice]);
        char input[20];
        fgets(input, 20, stdin);
        if(strlen(input) > 1) {
            int nuevaCantidad = atoi(input);
            if(nuevaCantidad >= 0) cantidades[indice] = nuevaCantidad;
        }
        
        // Editar tiempo
        printf("Nuevo tiempo por unidad [%d]: ", tiempos[indice]);
        fgets(input, 20, stdin);
        if(strlen(input) > 1) {
            int nuevoTiempo = atoi(input);
            if(nuevoTiempo > 0) tiempos[indice] = nuevoTiempo;
        }
        
        // Editar recursos
        printf("Nuevos recursos por unidad [%d]: ", recursos[indice]);
        fgets(input, 20, stdin);
        if(strlen(input) > 1) {
            int nuevosRecursos = atoi(input);
            if(nuevosRecursos > 0) recursos[indice] = nuevosRecursos;
        }
        
        printf("Producto actualizado con éxito!\n");
    }
}

// Función para eliminar un producto
void eliminarProducto(char nombres[][MAX_NOMBRE], int cantidades[], int tiempos[], int recursos[]) {
    char nombreBusqueda[MAX_NOMBRE];
    int indice;
    
    printf("\n=== ELIMINAR PRODUCTO ===\n");
    
    if(strlen(nombres[0]) == 0) {
        printf("No hay productos registrados!\n");
        return;
    }
    
    printf("Ingrese nombre del producto a eliminar: ");
    fgets(nombreBusqueda, MAX_NOMBRE, stdin);
    nombreBusqueda[strcspn(nombreBusqueda, "\n")] = '\0';
    
    indice = buscarProducto(nombres, nombreBusqueda);
    
    if(indice == -1) {
        printf("Producto no encontrado!\n");
    } else {
        printf("\nEliminando producto: %s\n", nombres[indice]);
        strcpy(nombres[indice], ""); // Cadena vacía para nombre
        cantidades[indice] = 0;
        tiempos[indice] = 0;
        recursos[indice] = 0;
        printf("Producto eliminado con éxito!\n");
    }
}

// Función para mostrar todos los productos
void mostrarProductos(char nombres[][MAX_NOMBRE], int cantidades[], int tiempos[], int recursos[]) {
    printf("\n=== LISTA DE PRODUCTOS ===\n");
    
    int productosMostrados = 0;
    
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        if(strlen(nombres[i]) > 0) {
            printf("\nProducto %d:\n", i+1);
            printf("Nombre: %s\n", nombres[i]);
            printf("Cantidad: %d\n", cantidades[i]);
            printf("Tiempo por unidad: %d horas\n", tiempos[i]);
            printf("Recursos por unidad: %d\n", recursos[i]);
            productosMostrados++;
        }
    }
    
    if(productosMostrados == 0) {
        printf("No hay productos registrados!\n");
    } else {
        printf("\nTotal productos registrados: %d\n", productosMostrados);
    }
}

int main() {
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    int cantidades[MAX_PRODUCTOS] = {0};
    int tiempos[MAX_PRODUCTOS] = {0};
    int recursos[MAX_PRODUCTOS] = {0};
    int tiempo_disponible = 0;
    int recursos_disponibles = 0;
    int opcion;
    
    printf("=== SISTEMA DE OPTIMIZACIÓN DE PRODUCCIÓN ===\n");
    printf("=== FÁBRICA DE COMPONENTES ELECTRÓNICOS ===\n\n");
    
    // Inicializar nombres como cadenas vacías
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        strcpy(nombres[i], "");
    }
    
    // Ingresar límites de la fábrica
    ingresarLimites(&tiempo_disponible, &recursos_disponibles);
    
    do {
        mostrarMenu();
        printf("\nSeleccione una opción: ");
        scanf("%d", &opcion);
        limpiarBuffer();
        
        switch(opcion) {
            case 1:
                ingresarProductos(nombres, cantidades, tiempos, recursos);
                break;
            case 2:
                calcularProduccion(nombres, cantidades, tiempos, recursos, 
                                 tiempo_disponible, recursos_disponibles);
                break;
            case 3:
                editarProducto(nombres, cantidades, tiempos, recursos);
                break;
            case 4:
                eliminarProducto(nombres, cantidades, tiempos, recursos);
                break;
            case 5:
                mostrarProductos(nombres, cantidades, tiempos, recursos);
                break;
            case 6:
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpción no válida. Intente nuevamente.\n");
        }
        
        if(opcion != 6) {
            printf("\nPresione Enter para continuar...");
            limpiarBuffer();
        }
    } while(opcion != 6);
    
    return 0;
}