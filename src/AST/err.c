#include "err.h"
#include <stdlib.h>
#include <string.h>

ErrorSem* lista = NULL;
int contador = 0;

void initErrores(){ lista = NULL; contador = 0; }

static char* dupstr(const char* s){ return s ? strdup(s) : strdup(""); }

void addError(const char* descripcion, const char* ambito, int linea, int columna){
    ErrorSem* e = (ErrorSem*)malloc(sizeof(ErrorSem));
    e->id = ++contador;
    e->descripcion = dupstr(descripcion);
    e->ambito = dupstr(ambito ? ambito : "global");
    e->linea = linea;
    e->columna = columna;
    e->sig = lista;
    lista = e;
}

int hayErrores(){ return contador > 0; }

void mostrarErrores(){
    if(!lista){
        printf("\n== Sin errores ==\n");
        return;
    }
    printf("\n== Reporte de Errores ==\n");
    printf("%-4s | %-50s | %-10s | %-5s | %-6s\n",
           "No.", "Descripción", "Ámbito", "Linea", "Col");
    printf("---------------------------------------------------------------\n");
    ErrorSem* cur = lista;
    while(cur){
        printf("%-4d | %-50s | %-10s | %-5d | %-6d\n",
               cur->id, cur->descripcion, cur->ambito, cur->linea, cur->columna);
        cur = cur->sig;
    }
    printf("---------------------------------------------------------------\n");
}

void liberarErrores(){
    ErrorSem* cur = lista;
    while(cur){
        ErrorSem* tmp = cur;
        cur = cur->sig;
        free(tmp->descripcion);
        free(tmp->ambito);
        free(tmp);
    }
    lista = NULL;
}