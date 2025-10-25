#include "Print.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include <stdio.h>
#include <ctype.h>
#include "../../AST/err.h"  
#include "../../simbolos/Evaluador.h"
#include "../../simbolos/salida.h"

extern void capture_print_output(const char* text);


void printUnicode(int codepoint) {
    if (codepoint <= 0x7F)
        printf("%c", codepoint);
    else if (codepoint <= 0x7FF)
        printf("%c%c",
            0xC0 | ((codepoint >> 6) & 0x1F),
            0x80 | (codepoint & 0x3F));
    else if (codepoint <= 0xFFFF)
        printf("%c%c%c",
            0xE0 | ((codepoint >> 12) & 0x0F),
            0x80 | ((codepoint >> 6) & 0x3F),
            0x80 | (codepoint & 0x3F));
    else
        printf("\\u%04X", codepoint); // fuera de BMP
}



void imprimirEvaluado(NodoAST* expr) {
    printf("[DEBUG] imprimirEvaluado() expr tipo: %d, valor: '%s', num_hijos: %d\n",
    expr ? expr->tipo : 0,
    expr && expr->valor ? expr->valor : "(null)",
    expr ? expr->num_hijos : -1);
    if (expr && expr->tipo == AST_CADENA) {
        printf("%s\n", expr->valor ? expr->valor : "");
        capture_print_output(expr->valor ? expr->valor : "");
        capture_print_output("\n");
        return;
    }
    
    if (!expr) {
        printf("\n");
        capture_print_output("\n");
        return;
    }

    if (expr->tipo == AST_CADENA && (!expr->valor || strlen(expr->valor) == 0)) {
        printf("\n");
        capture_print_output("\n");
        return;
    }
    
    ValorSimbolo v = evaluarExpr(expr);
    //printf("[DEBUG] Resultado evaluación - inicializado: %d, tipo: %d\n", 
    //       v.inicializado, v.tipo);
    
    if (!v.inicializado) {
        //printf("[PRINT] (sin valor)\n");
        capture_print_output("[PRINT] (sin valor)\n");
        return;
    }
    //printf("[DEBUG] Tipo de valor a imprimir: %d\n", v.tipo);

    char buffer[256];
    switch (v.tipo) {
        case TIPO_INT:
        //printf("[DEBUG] Entrando a TIPO_INT\n");
            snprintf(buffer, sizeof(buffer), "%d", v.v.i);
            break;
        case TIPO_FLOAT:
            //printf("[DEBUG] Entrando a TIPO_FLOAT\n");
            snprintf(buffer, sizeof(buffer), "%f", v.v.f);
            break;
        case TIPO_DOUBLE:
            //printf("[DEBUG] Entrando a TIPO_DOUBLE\n");
            snprintf(buffer, sizeof(buffer), "%lf", v.v.d);
            break;
        case TIPO_CHAR: {
            unsigned int codepoint = (unsigned char)v.v.c;
            if (codepoint <= 0x7F) {
                snprintf(buffer, sizeof(buffer), "%c", v.v.c);
                //printf("%s\n", buffer);

            } else {
                printf("'");
                printUnicode(codepoint);
                printf("'\n");
                // Opcional: también puedes guardar en buffer para capture_print_output
                snprintf(buffer, sizeof(buffer), "\\u%04X", codepoint);
                capture_print_output("\n");
            }
            break;
        }
        case TIPO_BOOLEAN:
            //printf("[DEBUG] Entrando a TIPO_BOOLEAN\n");
            snprintf(buffer, sizeof(buffer), "%s", v.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
           // printf("[DEBUG] Entrando a TIPO_STRING\n");
            printf("[DEBUG PRINT] v.v.s = '%s'\n", v.v.s ? v.v.s : "null");
            snprintf(buffer, sizeof(buffer), "%s", v.v.s ? v.v.s : "null");
            break;
        default:
            snprintf(buffer, sizeof(buffer), "(tipo desconocido)");
            break;
    }
    
    printf("%s\n", buffer);
    capture_print_output(buffer);
    capture_print_output("\n");
}
