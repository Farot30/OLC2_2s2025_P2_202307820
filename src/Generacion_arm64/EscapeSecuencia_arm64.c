#include "EscapeSecuencia_arm64.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* escaparParaARM64(const char* cadena) {
    if (!cadena) return NULL;
    
    size_t len = strlen(cadena);
    char* resultado = malloc(len * 2 + 1); // Espacio extra para escapes
    if (!resultado) return NULL;
    
    int j = 0;
    for (int i = 0; i < len; i++) {
        switch (cadena[i]) {
            case '\n':  // Carácter newline literal → \\n
                resultado[j++] = '\\';
                resultado[j++] = 'n';
                break;
            case '\r':  // Carácter retorno literal → \\r
                resultado[j++] = '\\';
                resultado[j++] = 'r';
                break;
            case '\t':  // Carácter tab literal → \\t
                resultado[j++] = '\\';
                resultado[j++] = 't';
                break;
            case '\\':  /* Barra invertida literal → \\\\*/
                resultado[j++] = '\\';
                resultado[j++] = '\\';
                break;
            case '"':   // Comilla doble literal → \\"
                resultado[j++] = '\\';
                resultado[j++] = '"';
                break;
            default:    // Carácter normal
                resultado[j++] = cadena[i];
                break;
        }
    }
    
    resultado[j] = '\0';
    return resultado;
}

char* limpiarCadenaParaASM(const char* cadena) {
    if (!cadena) return NULL;
    
    printf("[DEBUG ESCAPE] Cadena original: '%s'\n", cadena);
    
    // **NO REMOVER COMILLAS SI YA ESTÁN PROCESADAS POR EL LEXER**
    // El lexer ya removió las comillas externas, solo procesar escapes
    char* resultado = escaparParaARM64(cadena);
    
    printf("[DEBUG ESCAPE] Cadena escapada: '%s'\n", resultado ? resultado : "NULL");
    
    return resultado;
}