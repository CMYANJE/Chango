#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ZONAS 5
#define DIAS_HISTORICO 30
#define NUM_CONTAMINANTES 4
#define MAX_NOMBRE_ZONA 50
#define MAX_FILENAME 100

// Índices para mejor legibilidad
#define TEMPERATURA_IDX 0
#define HUMEDAD_IDX 1
#define VIENTO_IDX 2

// Contaminantes específicos para Ecuador
enum Contaminantes {PM2_5, PM10, NO2, SO2};

// Nombres de contaminantes para reportes
const char* nombres_contaminantes[] = {"PM2.5", "PM10", "NO2", "SO2"};
const char* unidades[] = {"μg/m³", "μg/m³", "μg/m³", "μg/m³"};

typedef struct {
    char nombre[MAX_NOMBRE_ZONA];
    float contaminantes[NUM_CONTAMINANTES]; 
    float clima[3]; // temperatura, humedad, viento
    float historico[DIAS_HISTORICO][NUM_CONTAMINANTES];
    int alerta; // 0=normal, 1=preventiva, 2=emergencia
} Zona;

// Límites según normativa ecuatoriana (μg/m³)
const float limites_ecuador[NUM_CONTAMINANTES] = {37.0, 75.0, 150.0, 125.0};

// Rangos válidos para validación
const float rangos_min[NUM_CONTAMINANTES] = {0.0, 0.0, 0.0, 0.0};
const float rangos_max[NUM_CONTAMINANTES] = {500.0, 600.0, 400.0, 300.0};

// Función para limpiar buffer de entrada
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Función para leer valores con validación mejorada
float leer_valor_valido(const char* mensaje, float min, float max) {
    float valor;
    int intentos = 0;
    int resultado;
    
    do {
        printf("%s", mensaje);
        resultado = scanf("%f", &valor);
        
        if(resultado != 1) {
            printf("❌ Error: Ingrese un número válido\n");
            limpiar_buffer();
            valor = min - 1; // Forzar repetición
            intentos++;
            continue;
        }
        
        if(valor < min || valor > max) {
            printf("⚠️  Error: Valor debe estar entre %.2f y %.2f\n", min, max);
            intentos++;
            if(intentos >= 3) {
                printf("⚠️  Demasiados intentos. Usando valor por defecto: %.2f\n", min);
                return min;
            }
        }
    } while(valor < min || valor > max);
    
    return valor;
}

// Función para leer nombre de zona de forma segura
void leer_nombre_zona(char* nombre) {
    printf("Nombre de la zona (ej: Quito-Centro): ");
    if(scanf("%49s", nombre) != 1) {
        strcpy(nombre, "Zona-Desconocida");
    }
    limpiar_buffer();
}

// Función para calcular factor de quemas agrícolas mejorada
float factor_quemas(int mes) {
    // Época seca en Ecuador: agosto-septiembre (más quemas)
    if (mes == 8 || mes == 9) return 1.5;
    // Época de transición: julio, octubre
    if (mes == 7 || mes == 10) return 1.2;
    // Época de mayor humedad (menos quemas)
    if (mes >= 3 && mes <= 5) return 0.8;
    return 1.0;
}

// Función para generar datos históricos más realistas
void generar_historico_realista(Zona *zona) {
    for(int d = 0; d < DIAS_HISTORICO; d++) {
        for(int c = 0; c < NUM_CONTAMINANTES; c++) {
            // Variación más natural: ±30% del valor base
            float variacion = 0.7 + 0.6 * ((float)rand() / RAND_MAX);
            zona->historico[d][c] = zona->contaminantes[c] * variacion;
            
            // Asegurar que no sea negativo
            if(zona->historico[d][c] < 0) {
                zona->historico[d][c] = 0.1;
            }
            
            // Añadir tendencia temporal (días más recientes similar a actual)
            if(d >= DIAS_HISTORICO - 7) {
                float factor_reciente = 0.9 + 0.2 * ((float)rand() / RAND_MAX);
                zona->historico[d][c] = zona->contaminantes[c] * factor_reciente;
            }
        }
    }
}

// Función para ingresar datos con validación mejorada
void ingresar_datos(Zona *zonas) {
    printf("\n🌍 === INGRESO DE DATOS PARA %d ZONAS ===\n", NUM_ZONAS);
    
    for(int i = 0; i < NUM_ZONAS; i++) {
        printf("\n📍 --- Zona %d ---\n", i+1);
        
        leer_nombre_zona(zonas[i].nombre);
        
        printf("\n🏭 Ingrese niveles actuales de contaminantes:\n");
        for(int c = 0; c < NUM_CONTAMINANTES; c++) {
            char mensaje[100];
            snprintf(mensaje, sizeof(mensaje), "  %s (%s, límite %.1f): ", 
                    nombres_contaminantes[c], unidades[c], limites_ecuador[c]);
            zonas[i].contaminantes[c] = leer_valor_valido(mensaje, 
                                                         rangos_min[c], 
                                                         rangos_max[c]);
        }
        
        printf("\n🌤️  Ingrese datos climáticos:\n");
        zonas[i].clima[TEMPERATURA_IDX] = leer_valor_valido("  Temperatura (°C, -10 a 40): ", -10.0, 40.0);
        zonas[i].clima[HUMEDAD_IDX] = leer_valor_valido("  Humedad (%, 0 a 100): ", 0.0, 100.0);
        zonas[i].clima[VIENTO_IDX] = leer_valor_valido("  Velocidad viento (km/h, 0 a 80): ", 0.0, 80.0);
        
        // Generar datos históricos
        generar_historico_realista(&zonas[i]);
        
        printf("✅ Datos de %s ingresados correctamente\n", zonas[i].nombre);
    }
}

// Función para calcular promedios mejorada
void calcular_promedios(Zona *zona) {
    for(int i = 0; i < NUM_CONTAMINANTES; i++) {
        float suma = 0;
        int dias_validos = 0;
        
        for(int d = 0; d < DIAS_HISTORICO; d++) {
            if(zona->historico[d][i] >= 0) { // Solo datos válidos
                suma += zona->historico[d][i];
                dias_validos++;
            }
        }
        
        if(dias_validos > 0) {
            zona->contaminantes[i] = suma / dias_validos;
        }
    }
}

// Función para predecir contaminación mejorada
void predecir_contaminacion(Zona *zona, int mes) {
    float factor = factor_quemas(mes);
    
    for(int i = 0; i < NUM_CONTAMINANTES; i++) {
        float prediccion = 0;
        float peso_total = 0;
        
        // Usar los últimos 7 días con pesos decrecientes
        for(int d = 0; d < 7 && d < DIAS_HISTORICO; d++) {
            float peso = 7 - d; // Más peso a días más recientes
            prediccion += zona->historico[DIAS_HISTORICO-1-d][i] * peso;
            peso_total += peso;
        }
        
        if(peso_total > 0) {
            zona->contaminantes[i] = (prediccion / peso_total) * factor;
        }
        
        // Ajuste por condiciones climáticas
        if(zona->clima[VIENTO_IDX] > 15.0) {
            zona->contaminantes[i] *= 0.8; // Viento fuerte dispersa contaminantes
        }
        if(zona->clima[HUMEDAD_IDX] > 80.0) {
            zona->contaminantes[i] *= 1.1; // Alta humedad retiene contaminantes
        }
    }
}

// Función para evaluar alertas mejorada
void evaluar_alertas(Zona *zona) {
    zona->alerta = 0; // Por defecto sin alerta
    int contaminantes_elevados = 0;
    
    for(int i = 0; i < NUM_CONTAMINANTES; i++) {
        float porcentaje = (zona->contaminantes[i] / limites_ecuador[i]) * 100;
        
        if(porcentaje > 100) {
            zona->alerta = 2; // Emergencia
            contaminantes_elevados++;
        } else if(porcentaje > 75 && zona->alerta < 1) {
            zona->alerta = 1; // Alerta preventiva
            contaminantes_elevados++;
        }
    }
    
    // Ajuste especial para PM2.5 (más peligroso)
    if((zona->contaminantes[PM2_5] / limites_ecuador[PM2_5]) > 0.8 && zona->alerta < 1) {
        zona->alerta = 1;
    }
    
    // Si múltiples contaminantes están elevados, aumentar alerta
    if(contaminantes_elevados >= 2 && zona->alerta < 2) {
        zona->alerta = 2;
    }
}

// Función para mostrar resultados de una zona
void mostrar_resultados_zona(Zona zona) {
    printf("\n🏙️  === %s ===\n", zona.nombre);
    
    printf("📊 Niveles actuales:\n");
    for(int i = 0; i < NUM_CONTAMINANTES; i++) {
        float porcentaje = (zona.contaminantes[i] / limites_ecuador[i]) * 100;
        char estado[20];
        
        if(porcentaje <= 50) strcpy(estado, "🟢 BUENO");
        else if(porcentaje <= 75) strcpy(estado, "🟡 REGULAR");
        else if(porcentaje <= 100) strcpy(estado, "🟠 MALO");
        else strcpy(estado, "🔴 PELIGROSO");
        
        printf("  %s: %.2f %s (%.1f%% del límite) %s\n", 
               nombres_contaminantes[i], zona.contaminantes[i], 
               unidades[i], porcentaje, estado);
    }
    
    printf("\n🌡️  Condiciones climáticas:\n");
    printf("  Temperatura: %.1f°C | Humedad: %.1f%% | Viento: %.1f km/h\n",
           zona.clima[TEMPERATURA_IDX], zona.clima[HUMEDAD_IDX], zona.clima[VIENTO_IDX]);
    
    printf("\n🚨 Nivel de alerta: ");
    switch(zona.alerta) {
        case 0: printf("🟢 NORMAL\n"); break;
        case 1: printf("🟡 PREVENTIVA\n"); break;
        case 2: printf("🔴 EMERGENCIA\n"); break;
    }
}

// Función para generar recomendaciones específicas
void generar_recomendaciones(Zona zona) {
    printf("\n💡 Recomendaciones para %s:\n", zona.nombre);
    
    if(zona.alerta >= 1) {
        printf("⚠️  MEDIDAS PREVENTIVAS:\n");
        
        if(zona.contaminantes[PM2_5] > limites_ecuador[PM2_5] * 0.75) {
            printf("  • Evitar actividades físicas intensas al aire libre\n");
            printf("  • Usar mascarilla de protección (N95 o KN95)\n");
            printf("  • Mantener ventanas cerradas durante el día\n");
            
            // Recomendaciones específicas por ciudad
            if(strstr(zona.nombre, "Quito") != NULL || strstr(zona.nombre, "quito") != NULL) {
                printf("  • Evitar ejercitarse en el Parque La Carolina y El Ejido\n");
                printf("  • Considerar la altitud (2800m) - mayor impacto respiratorio\n");
            }
            if(strstr(zona.nombre, "Guayaquil") != NULL || strstr(zona.nombre, "guayaquil") != NULL) {
                printf("  • Evitar el Malecón 2000 en horas de mayor tráfico\n");
                printf("  • Cuidado extra por el calor y humedad\n");
            }
        }
        
        if(zona.contaminantes[NO2] > limites_ecuador[NO2] * 0.75) {
            printf("  • Preferir transporte público o bicicleta\n");
            printf("  • Evitar zonas de alto tráfico vehicular\n");
            printf("  • Planificar rutas por calles menos transitadas\n");
        }
        
        if(zona.contaminantes[SO2] > limites_ecuador[SO2] * 0.75) {
            printf("  • Alejarse de zonas industriales\n");
            printf("  • Personas con asma: llevar inhalador\n");
        }
        
        if(zona.alerta == 2) {
            printf("\n🚨 MEDIDAS DE EMERGENCIA:\n");
            printf("  • Permanecer en interiores tanto como sea posible\n");
            printf("  • Restricción vehicular recomendada\n");
            printf("  • Grupos vulnerables: niños, ancianos, embarazadas - extremar cuidados\n");
            printf("  • Considerar cerrar escuelas y actividades al aire libre\n");
        }
    } else {
        printf("✅ Niveles dentro de rangos aceptables\n");
        printf("  • Mantener prácticas ecológicas\n");
        printf("  • Usar transporte público cuando sea posible\n");
        printf("  • Aprovechar para actividades al aire libre\n");
    }
    
    // Recomendaciones según clima
    if(zona.clima[VIENTO_IDX] < 5.0) {
        printf("  • Viento bajo: contaminantes se acumulan más\n");
    }
    if(zona.clima[HUMEDAD_IDX] > 80.0) {
        printf("  • Alta humedad: mayor retención de contaminantes\n");
    }
}

// Función para imprimir línea separadora
void imprimir_separador() {
    for(int i = 0; i < 50; i++) {
        printf("=");
    }
    printf("\n");
}

// Función para exportar reporte completo
int exportar_reporte_completo(Zona zonas[], int num_zonas, int mes, const char* nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if(archivo == NULL) {
        printf("❌ Error: No se pudo crear el archivo %s\n", nombre_archivo);
        return 0;
    }
    
    // Obtener tiempo actual de forma segura
    time_t tiempo_actual = time(NULL);
    char* tiempo_str = ctime(&tiempo_actual);
    
    // Encabezado del reporte
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "REPORTE DE CALIDAD DEL AIRE - ECUADOR\n");
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "Mes: %d\n", mes);
    fprintf(archivo, "Fecha de generación: %s", tiempo_str);
    fprintf(archivo, "Normativa: Texto Unificado de Legislación Ambiental (TULSMA)\n\n");
    
    // Datos por zona
    for(int i = 0; i < num_zonas; i++) {
        fprintf(archivo, "ZONA: %s\n", zonas[i].nombre);
        fprintf(archivo, "----------------------------------------\n");
        
        fprintf(archivo, "Niveles actuales:\n");
        for(int c = 0; c < NUM_CONTAMINANTES; c++) {
            float porcentaje = (zonas[i].contaminantes[c] / limites_ecuador[c]) * 100;
            fprintf(archivo, "  %s: %.2f %s (%.1f%% del límite - %.2f)\n", 
                   nombres_contaminantes[c], zonas[i].contaminantes[c], 
                   unidades[c], porcentaje, limites_ecuador[c]);
        }
        
        fprintf(archivo, "\nCondiciones climáticas:\n");
        fprintf(archivo, "  Temperatura: %.1f°C\n", zonas[i].clima[TEMPERATURA_IDX]);
        fprintf(archivo, "  Humedad: %.1f%%\n", zonas[i].clima[HUMEDAD_IDX]);
        fprintf(archivo, "  Viento: %.1f km/h\n", zonas[i].clima[VIENTO_IDX]);
        
        fprintf(archivo, "\nNivel de alerta: ");
        switch(zonas[i].alerta) {
            case 0: fprintf(archivo, "NORMAL\n"); break;
            case 1: fprintf(archivo, "PREVENTIVA\n"); break;
            case 2: fprintf(archivo, "EMERGENCIA\n"); break;
        }
        
        fprintf(archivo, "\n");
    }
    
    // Resumen general
    fprintf(archivo, "RESUMEN GENERAL\n");
    fprintf(archivo, "===============\n");
    int zonas_normales = 0, zonas_alerta = 0, zonas_emergencia = 0;
    
    for(int i = 0; i < num_zonas; i++) {
        switch(zonas[i].alerta) {
            case 0: zonas_normales++; break;
            case 1: zonas_alerta++; break;
            case 2: zonas_emergencia++; break;
        }
    }
    
    fprintf(archivo, "Zonas en estado normal: %d\n", zonas_normales);
    fprintf(archivo, "Zonas en alerta preventiva: %d\n", zonas_alerta);
    fprintf(archivo, "Zonas en emergencia: %d\n", zonas_emergencia);
    
    fprintf(archivo, "\n--- Fin del reporte ---\n");
    
    fclose(archivo);
    return 1;
}

// Función principal mejorada
int main() {
    // Inicializar generador de números aleatorios UNA SOLA VEZ
    srand(time(NULL));
    
    Zona zonas[NUM_ZONAS];
    int mes_actual;
    char nombre_archivo[MAX_FILENAME];
    
    printf("🌍 ======================================================\n");
    printf("   SISTEMA DE MONITOREO DE CALIDAD DEL AIRE - ECUADOR 🇪🇨\n");
    printf("======================================================\n");
    printf("📋 Basado en normativa TULSMA (Texto Unificado de Legislación Ambiental)\n\n");
    
    mes_actual = (int)leer_valor_valido("📅 Ingrese el mes actual (1-12): ", 1, 12);
    
    // 1. Ingreso de datos
    ingresar_datos(zonas);
    
    // 2. Procesamiento
    printf("\n⚙️  Procesando datos y generando predicciones...\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        calcular_promedios(&zonas[i]);
        predecir_contaminacion(&zonas[i], mes_actual);
        evaluar_alertas(&zonas[i]);
    }
    
    // 3. Salida de resultados
    printf("\n📊 ===============================\n");
    printf("   RESULTADOS DEL ANÁLISIS\n");
    printf("===============================\n");
    
    for(int i = 0; i < NUM_ZONAS; i++) {
        mostrar_resultados_zona(zonas[i]);
        generar_recomendaciones(zonas[i]);
        printf("\n");
        imprimir_separador();
    }
    
    // 4. Exportar a archivo
    printf("\n💾 Generando reporte...\n");
    printf("📄 Nombre para archivo de reporte (ej: reporte_aire.txt): ");
    if(scanf("%99s", nombre_archivo) != 1) {
        strcpy(nombre_archivo, "reporte_calidad_aire.txt");
    }
    
    if(exportar_reporte_completo(zonas, NUM_ZONAS, mes_actual, nombre_archivo)) {
        printf("✅ Reporte generado exitosamente en: %s\n", nombre_archivo);
    } else {
        printf("❌ Error al crear el archivo de reporte\n");
        return 1;
    }
    
    printf("\n🎉 ===============================\n");
    printf("   ANÁLISIS COMPLETADO EXITOSAMENTE!\n");
    printf("===============================\n");
    printf("📧 Para más información: Ministerio del Ambiente - Ecuador\n");
    
    return 0;
}