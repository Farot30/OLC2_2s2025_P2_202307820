#include "Break_continue_arm64.h"
#include "../generador_arm64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Stack global de contextos de bucles
ContextoBucle* stack_contextos_bucle = NULL;

void pushContextoBucle(const char* etiq_break, const char* etiq_continue, const char* tipo) {
    ContextoBucle* nuevo = malloc(sizeof(ContextoBucle));
    strncpy(nuevo->etiqueta_break, etiq_break, sizeof(nuevo->etiqueta_break) - 1);
    strncpy(nuevo->etiqueta_continue, etiq_continue, sizeof(nuevo->etiqueta_continue) - 1);
    strncpy(nuevo->tipo_contexto, tipo, sizeof(nuevo->tipo_contexto) - 1);
    nuevo->siguiente = stack_contextos_bucle;
    stack_contextos_bucle = nuevo;
    
    printf("[DEBUG CONTEXTO] Push: break=%s, continue=%s, tipo=%s\n", 
           etiq_break, etiq_continue, tipo);
}

void popContextoBucle() {
    if (!stack_contextos_bucle) return;
    
    ContextoBucle* temp = stack_contextos_bucle;
    stack_contextos_bucle = stack_contextos_bucle->siguiente;
    
    printf("[DEBUG CONTEXTO] Pop: break=%s, continue=%s\n", 
           temp->etiqueta_break, temp->etiqueta_continue);
    
    free(temp);
}

ContextoBucle* getContextoActual() {
    return stack_contextos_bucle;
}

void generarCodigoBreak(GeneradorDeclaraciones* gen, NodoAST* nodoBreak) {
    (void)nodoBreak; // Evitar warning de parámetro no usado
    
    ContextoBucle* contexto = getContextoActual();
    
    if (!contexto) {
        printf("[ERROR] Break fuera de un ciclo o switch (línea %d, columna %d)\n",
               nodoBreak->linea, nodoBreak->columna);
        return;
    }
    
    char codigo[128];
    snprintf(codigo, sizeof(codigo),
        "    // Break: salir del %s\n"
        "    b %s\n",
        contexto->tipo_contexto, contexto->etiqueta_break);
    
    insertarEnSeccionText(gen, codigo);
    
    printf("[DEBUG BREAK] Generado salto a %s\n", contexto->etiqueta_break);
}

void generarCodigoContinue(GeneradorDeclaraciones* gen, NodoAST* nodoContinue) {
    (void)nodoContinue; // Evitar warning de parámetro no usado
    
    ContextoBucle* contexto = getContextoActual();
    
    if (!contexto) {
        printf("[ERROR] Continue fuera de un ciclo (línea %d, columna %d)\n",
               nodoContinue->linea, nodoContinue->columna);
        return;
    }
    
    // **VALIDAR QUE NO SE USE EN SWITCH**
    if (strcmp(contexto->tipo_contexto, "switch") == 0) {
        printf("[ERROR] Continue no se puede usar en switch (línea %d, columna %d)\n",
               nodoContinue->linea, nodoContinue->columna);
        return;
    }
    
    char codigo[128];
    snprintf(codigo, sizeof(codigo),
        "    // Continue: siguiente iteración del %s\n"
        "    b %s\n",
        contexto->tipo_contexto, contexto->etiqueta_continue);
    
    insertarEnSeccionText(gen, codigo);
    
    printf("[DEBUG CONTINUE] Generado salto a %s\n", contexto->etiqueta_continue);
}