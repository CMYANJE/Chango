#ifndef BOOLEANAS_H
#define BOOLEANAS_H

// Definición de colores ANSI
#define RESET   "\033[0m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define BLUE    "\033[0;34m"
#define YELLOW  "\033[0;33m"

// Declaraciones de funciones
void mostrarCaratula();
int obtenerNumVariables();
void ingresarTabla(int n, int salida[]);
void generarExpresion(int n, int salida[]);
void imprimirTabla(int n, int salida[]);
void generarCircuito(int n, int salida[]);

#endif // BOOLEANAS_H
