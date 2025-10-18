#include "Impresion_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Operadores_arm64.h"
#include "EscapeSecuencia_arm64.h" 
#include <stdio.h>
#include <string.h>

bool linea_vacia = false;
extern NodoAST* astGlobal;

static bool esVariableAscizImpresion(GeneradorDeclaraciones* gen, const char* nombreVar) {
    if (!gen->buffer || !nombreVar) return false;
    
    // Buscar la declaración exacta de la variable
    char patron[128];
    snprintf(patron, sizeof(patron), "%s:", nombreVar);
    
    char* pos = strstr(gen->buffer, patron);
    if (pos) {
        // Verificar que sea el inicio de una línea (o después de \n)
        if (pos != gen->buffer && *(pos - 1) != '\n') {
            // Buscar la siguiente ocurrencia
            pos = strstr(pos + 1, patron);
        }
        
        if (pos) {
            // Buscar el tipo de declaración en la misma línea
            char* finLinea = strchr(pos, '\n');
            if (finLinea) {
                // Crear una subcadena de solo esa línea
                size_t longLinea = finLinea - pos;
                char linea[256];
                strncpy(linea, pos, longLinea);
                linea[longLinea] = '\0';
                
                // Verificar si contiene .asciz
                if (strstr(linea, ".asciz")) {
                    return true;
                }
            }
        }
    }
    
    return false;
}
int getVectorSize(NodoAST* raiz, const char* nombre) {
    if (!raiz) return 0;
    if (raiz->tipo == AST_DECLARACION_VECTOR && raiz->num_hijos >= 2) {
        NodoAST* idNodo = raiz->hijos[1];
        if (idNodo && strcmp(idNodo->valor, nombre) == 0) {
            // Caso: new int[5]
            if (raiz->num_hijos >= 4 && raiz->hijos[3]->tipo == AST_1DIMENSION) {
                NodoAST* dimNodo = raiz->hijos[3]->hijos[0];
                return atoi(dimNodo->valor);
            }
            // Caso: { ... }
            if (raiz->num_hijos >= 3 && raiz->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
                NodoAST* lista = raiz->hijos[2];
                if (lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_INTERNO)
                    lista = lista->hijos[0];
                return lista->num_hijos;
            }
        }
    }
    // Buscar recursivamente
    for (int i = 0; i < raiz->num_hijos; i++) {
        int tam = getVectorSize(raiz->hijos[i], nombre);
        if (tam > 0) return tam;
    }
    return 0;
}
// Función para generar impresión en ARM64 utilizando printf
void generarImpresionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_PRINT) return;

    printf("[DEBUG] Generando impresión ARM64 para nodo tipo: %d, valor: '%s'\n",
           nodo->tipo, nodo->valor ? nodo->valor : "(null)");

    char buffer[512];
    char codigo[1024] = "";
    NodoAST* valorNodo = (nodo->num_hijos > 0) ? nodo->hijos[0] : NULL;  // Nodo con el valor a imprimir

    if (!valorNodo) {
        // Imprimir una línea vacía
        if (!linea_vacia) {
            insertarEnSeccionData(gen, ".Lnewline: .asciz \"\\n\"\n");
            linea_vacia = true;
        }
        snprintf(buffer, sizeof(buffer),
            "    ldr x0, =.Lnewline\n"
            "    bl printf\n");
        strcat(codigo, buffer);
    } else {
        // Detectar el tipo del valor a imprimir
        TipoDato tipo = deducirTipoExprARM64(valorNodo, astGlobal);

        if (tipo == TIPO_STRING) {
            char formatoConst[64];
            snprintf(formatoConst, sizeof(formatoConst), "format_string_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoConst);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                // **DETECTAR SI ES .asciz O .skip**
                bool esAsciz = esVariableAscizImpresion(gen, valorNodo->valor);
                
                if (esAsciz) {
                    // Es string literal (.asciz) - cargar dirección directamente
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"       // x1 = dirección del string
                        "    ldr x0, =%s\n"       // x0 = formato
                        "    bl printf\n",
                        valorNodo->valor, formatoConst);
                } else {
                    // Es puntero (.skip 8) - cargar el puntero que contiene
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"       // x1 = dirección de la variable
                        "    ldr x1, [x1]\n"      // x1 = puntero a la cadena
                        "    ldr x0, =%s\n"       // x0 = formato
                        "    bl printf\n",
                        valorNodo->valor, formatoConst);
                }
                strcat(codigo, buffer);
            } else {
                // Imprimir string literal
                char* cadenaEscapada = limpiarCadenaParaASM(valorNodo->valor);
                if (cadenaEscapada) {
                    char nombreConst[64];
                    snprintf(nombreConst, sizeof(nombreConst), "str_const_%d", gen->admin_registros->contador_constantes++);
                    snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreConst, cadenaEscapada);
                    insertarEnSeccionData(gen, buffer);

                    snprintf(buffer, sizeof(buffer),
                        "    ldr x0, =%s\n"
                        "    ldr x1, =%s\n"
                        "    bl printf\n",
                        formatoConst, nombreConst);
                    strcat(codigo, buffer);

                    free(cadenaEscapada);
                }
            }
        } else if (tipo == TIPO_CHAR) {
            // Definir el formato en la sección .data
            char formatoChar[64];
            snprintf(formatoChar, sizeof(formatoChar), "format_char_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\\n\"\n", formatoChar);
            insertarEnSeccionData(gen, buffer);

            // Imprimir carácter directamente
            snprintf(buffer, sizeof(buffer),
                "    ldr x0, =%s\n"  // Formato para carácter
                "    mov w1, #%d\n"  // Valor del carácter
                "    bl printf\n",
                formatoChar, valorNodo->valor[0]);
            strcat(codigo, buffer);
        } else if (tipo == TIPO_INT) {
            // Definir el formato en la sección .data
            char formatoInt[64];
            snprintf(formatoInt, sizeof(formatoInt), "format_int_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoInt);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                // Imprimir el valor de una variable
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr w1, [x1]\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    valorNodo->valor, formatoInt);
            } else {
                // Imprimir un literal
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    mov w1, #%d\n"
                    "    bl printf\n",
                    formatoInt, atoi(valorNodo->valor));
            }
            strcat(codigo, buffer);
        } else if (tipo == TIPO_DOUBLE || tipo == TIPO_FLOAT) {
            // Definir el formato en la sección .data
            char formatoFloat[64];
            snprintf(formatoFloat, sizeof(formatoFloat), "format_float_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoFloat);
            insertarEnSeccionData(gen, buffer);

            // Imprimir número de punto flotante
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "float_const_%d", gen->admin_registros->contador_constantes++);
            insertarConstanteDoubleRoData(gen, nombreConst, atof(valorNodo->valor));

            snprintf(buffer, sizeof(buffer),
                "    ldr x0, =%s\n"      // Formato para flotante
                "    ldr d0, =%s\n"      // Dirección del valor flotante
                "    bl printf\n",
                formatoFloat, nombreConst);
            strcat(codigo, buffer);
        } else if (tipo == TIPO_VECTOR && valorNodo->tipo == AST_IDENTIFICADOR) {
            TipoDato tipoBase = obtenerTipoBaseVector(astGlobal, valorNodo->valor);
            int tam = getVectorSize(astGlobal, valorNodo->valor);
            if (tam <= 0) tam = 1;
            const char* vecname = valorNodo->valor;

            // Formatos y tamaño en .data
            snprintf(buffer, sizeof(buffer), "format_vec_open: .asciz \"[\"\n");
            insertarEnSeccionData(gen, buffer);
            snprintf(buffer, sizeof(buffer), "format_vec_close: .asciz \"]\\n\"\n");
            insertarEnSeccionData(gen, buffer);
            snprintf(buffer, sizeof(buffer), "format_vec_sep: .asciz \", \"\n");
            insertarEnSeccionData(gen, buffer);
            snprintf(buffer, sizeof(buffer), "vector_size_%s: .word %d\n", vecname, tam);
            insertarEnSeccionData(gen, buffer);

            if (tipoBase == TIPO_INT){
                snprintf(buffer, sizeof(buffer), "format_vec_int: .asciz \"%%d\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector (usando registros diferentes)
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                // 2. Carga base y tamaño
                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 3. Inicio del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 4. Acceso e impresión del elemento
                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #2\n"
                    "    add x24, x19, w23, sxtw\n"
                    "    ldr w1, [x24]\n"
                    "    ldr x0, =format_vec_int\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                // 5. Incremento y separador
                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 6. Cierre del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n\n",
                    vecname);
                strcat(codigo, buffer);

            } else if(tipoBase == TIPO_CHAR){
                snprintf(buffer, sizeof(buffer), "format_vec_char: .asciz \"%%c\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector de caracteres
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");

                strcat(codigo, buffer);

                // 2. Carga base y tamaño
                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 3. Inicio del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 4. Acceso e impresión del elemento
                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #0\n"
                    "    add x24, x19, w23, sxtw\n"
                    "    ldrb w1, [x24]\n"
                    "    ldr x0, =format_vec_char\n"
                    "    bl printf\n");
                strcat(codigo, buffer);
                // 5. Incremento y separador
                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);
                // 6. Cierre del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n\n",
                    vecname);
                strcat(codigo, buffer);
            } else if (tipoBase == TIPO_STRING){
                snprintf(buffer, sizeof(buffer), "formar_vec_str: .asciz \"%%s\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector de cadenas
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #3\n"
                    "    add x24, x19, w23, sxtw\n"
                    "    ldr x1, [x24]\n"
                    "    ldr x0, =formar_vec_str\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                // 5. Incremento y separador
                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 6. Cierre del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n",
                    vecname);
                strcat(codigo, buffer);
            } else if (tipoBase == TIPO_BOOLEAN) {
                snprintf(buffer, sizeof(buffer), "format_vec_bool: .asciz \"%%d\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector de booleanos como 1 y 0
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #0\n" // 1 byte por booleano
                    "    add x24, x19, w23, sxtw\n"
                    "    ldrb w1, [x24]\n"
                    "    ldr x0, =format_vec_bool\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                // 5. Incremento y separador
                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                // 6. Cierre del ciclo
                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n",
                    vecname);
                strcat(codigo, buffer);
            } else if (tipoBase == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer), "format_vec_double: .asciz \"%%f\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector de doubles
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #3\n" // 8 bytes por double
                    "    add x24, x19, w23, sxtw\n"
                    "    ldr d1, [x24]\n"
                    "    ldr x0, =format_vec_double\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n",
                    vecname);
                strcat(codigo, buffer);
            } else if (tipoBase == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer), "format_vec_float: .asciz \"%%f\"\n");
                insertarEnSeccionData(gen, buffer);

                // Código ARM64 para imprimir el vector de floats
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =format_vec_open\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    ldr x19, =%s\n"
                    "    ldr x20, =vector_size_%s\n"
                    "    ldr w21, [x20]\n"
                    "    mov w22, #0\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_loop_%s:\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    lsl w23, w22, #2\n" // 4 bytes por float
                    "    add x24, x19, w23, sxtw\n"
                    "    ldr s1, [x24]\n"
                    "    ldr x0, =format_vec_float\n"
                    "    bl printf\n");
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "    add w22, w22, #1\n"
                    "    cmp w22, w21\n"
                    "    bge print_vec_end_%s\n"
                    "    ldr x0, =format_vec_sep\n"
                    "    bl printf\n"
                    "    b print_vec_loop_%s\n",
                    vecname, vecname);
                strcat(codigo, buffer);

                snprintf(buffer, sizeof(buffer),
                    "print_vec_end_%s:\n"
                    "    ldr x0, =format_vec_close\n"
                    "    bl printf\n",
                    vecname);
                strcat(codigo, buffer);
            }
        } else {
            // Tipo no soportado
            snprintf(buffer, sizeof(buffer),
                "    // Tipo no soportado para impresión\n");
            strcat(codigo, buffer);
        }
    }

    // Agregar el código generado a la sección .text
    insertarEnSeccionText(gen, codigo);
}