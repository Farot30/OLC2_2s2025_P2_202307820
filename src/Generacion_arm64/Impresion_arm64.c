#include "Impresion_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include <stdio.h>
#include <string.h>

// Función para generar impresión en ARM64
void generarImpresionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_PRINT) return;

    char buffer[512];
    char codigo[1024] = "";
    NodoAST* valorNodo = nodo->hijos[0];  // Nodo con el valor a imprimir

    if (!valorNodo) {
        // Imprimir una línea vacía
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =.Lnewline\n"
            "    mov x2, #1\n"
            "    mov x0, #1\n"
            "    mov x8, #64\n"
            "    svc #0\n");
        strcat(codigo, buffer);
    } else {
        // Detectar el tipo del valor a imprimir
        TipoDato tipo = deducirTipoExpr(valorNodo);

        if (tipo == TIPO_STRING) {
            // Imprimir cadena directamente
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "str_const_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz %s\n", nombreConst, valorNodo->valor);
            insertarEnSeccionData(gen, buffer);

            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    mov x2, #%zu\n"
                "    mov x0, #1\n"
                "    mov x8, #64\n"
                "    svc #0\n",
                nombreConst, strlen(valorNodo->valor) - 2);  // Restar 2 por las comillas
            strcat(codigo, buffer);
        } else if (tipo == TIPO_CHAR) {
            // Imprimir carácter directamente
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    strb w1, [sp, #-1]!\n"
                "    mov x1, sp\n"
                "    mov x2, #1\n"
                "    mov x0, #1\n"
                "    mov x8, #64\n"
                "    svc #0\n",
                valorNodo->valor[0]);
            strcat(codigo, buffer);
        } else {
            // Convertir otros tipos a cadena y luego imprimir
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "str_const_%d", gen->admin_registros->contador_constantes++);
            if (tipo == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w0, #%d\n"
                    "    bl int_to_ascii\n"
                    "    ldr x1, =%s\n"
                    "    mov x2, #<longitud>\n"
                    "    mov x0, #1\n"
                    "    mov x8, #64\n"
                    "    svc #0\n",
                    atoi(valorNodo->valor), nombreConst);
            } else if (tipo == TIPO_DOUBLE || tipo == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    // Conversión de double/float a cadena no implementada\n");
            }
            strcat(codigo, buffer);
        }
    }

    // Agregar el código generado a la sección .text
    insertarEnSeccionText(gen, codigo);
}