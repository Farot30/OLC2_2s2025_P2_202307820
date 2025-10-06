#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H
#include <stdio.h>
#include "../comun/strutil.h"


typedef struct ErrorSem {
    int id;
    char* descripcion;
    char* ambito;
    int linea;
    int columna;
    struct ErrorSem* sig;
} ErrorSem;

extern ErrorSem* lista;
extern int contador;

void initErrores();
void addError(const char* descripcion, const char* ambito, int linea, int columna);
int hayErrores();
void mostrarErrores();
void liberarErrores();

#endif