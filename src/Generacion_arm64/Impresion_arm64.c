#include "Impresion_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Operadores_arm64.h"
#include "EscapeSecuencia_arm64.h" 
#include <stdio.h>
#include <string.h>

bool linea_vacia = false;
extern NodoAST* astGlobal;

// Función para insertar las constantes str_true y str_false en la sección .data


// **ESTRUCTURAS PARA ANÁLISIS DE CONCATENACIÓN EN PRINTS**
#define MAX_PARTES_CONCAT 16
typedef struct {
    NodoAST* nodo;
    TipoDato tipo;
} ParteConcatenacion;

// **FUNCIÓN RECURSIVA PARA RECOLECTAR PARTES DE CONCATENACIÓN**
static void recolectarPartesConcatenacion(NodoAST* nodo, ParteConcatenacion* partes, int* numPartes, int maxPartes) {
    if (!nodo || *numPartes >= maxPartes) return;
    printf("[DEBUG CONCAT] Parte: tipo=%d, valor=%s, deducido=%d\n", nodo->tipo, nodo->valor ? nodo->valor : "(null)", partes[*numPartes].tipo);
    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "+") == 0 && nodo->num_hijos == 2) {
        // Concatenación: procesar recursivamente izquierda y derecha
        recolectarPartesConcatenacion(nodo->hijos[0], partes, numPartes, maxPartes);
        recolectarPartesConcatenacion(nodo->hijos[1], partes, numPartes, maxPartes);
    } else {
        // Nodo hoja: agregar a las partes
        partes[*numPartes].nodo = nodo;
        partes[*numPartes].tipo = deducirTipoExprARM64(nodo, astGlobal);
        
        (*numPartes)++;
    }
}



bool esVariableAscizImpresion(GeneradorDeclaraciones* gen, const char* nombreVar) {
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

static void insertarConstantesBooleanas(GeneradorDeclaraciones* gen) {
    if (!esVariableAscizImpresion(gen, "str_true")) {
        insertarEnSeccionData(gen, "str_true: .asciz \"true\"\n");
    }
    if (!esVariableAscizImpresion(gen, "str_false")) {
        insertarEnSeccionData(gen, "str_false: .asciz \"false\"\n");
    }
    if(!esVariableAscizImpresion(gen, "__temp_print_str")){
        insertarEnSeccionBss(gen, "__temp_print_str: .skip 8\n");
    }
}
static bool contieneStringEnOperacion(NodoAST* nodo) {
    if (!nodo) return false;
    TipoDato tipo = deducirTipoExprARM64(nodo, astGlobal);
    
    if (tipo == TIPO_STRING) return true;
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (contieneStringEnOperacion(nodo->hijos[i])) return true;
    }
    return false;
}

// **FUNCIÓN PARA GENERAR PRINTF OPTIMIZADO DESDE CONCATENACIÓN**
static bool generarPrintfDesdeConcatenacion(GeneradorDeclaraciones* gen, NodoAST* valorNodo, char* codigo) {
    ParteConcatenacion partes[MAX_PARTES_CONCAT];
    int numPartes = 0;
    
    // Recolectar todas las partes de la concatenación
    recolectarPartesConcatenacion(valorNodo, partes, &numPartes, MAX_PARTES_CONCAT);
    
    if (numPartes == 0) return false;
    
    // Construir el formato dinámicamente
    char formato[512] = "";
    int numArgs = 0;
    
    for (int i = 0; i < numPartes; i++) {
        NodoAST* parte = partes[i].nodo;
        TipoDato tipo = partes[i].tipo;
        
        if (tipo == TIPO_STRING && parte->tipo == AST_CADENA) {
            // String literal: agregar directamente al formato
            strcat(formato, parte->valor);
        } else if (tipo == TIPO_INT) {
            strcat(formato, "%d");
            numArgs++;
        } else if (tipo == TIPO_FLOAT) {
            strcat(formato, "%f");
            numArgs++;
        } else if(tipo == TIPO_DOUBLE){
            strcat(formato, "%f");
            numArgs++;
        } else if (tipo == TIPO_CHAR) {
            strcat(formato, "%c");
            numArgs++;
        } else if (tipo == TIPO_BOOLEAN) {
            strcat(formato, "%s");  // Usaremos "true"/"false"
            numArgs++;
        } else if (tipo == TIPO_STRING && parte->tipo == AST_IDENTIFICADOR) {
            strcat(formato, "%s");
            numArgs++;
        } else {
            // Tipo no soportado: usar StringBuilder
            printf("[DEBUG PRINTF] Tipo no soportado en concatenación: tipo=%d, valor=%s\n", tipo, parte->valor ? parte->valor : "(null)");
            return false;
        }
    }
    
    // Agregar salto de línea al final
    strcat(formato, "\\n");
    
    // Crear constante para el formato
    char nombreFormato[64];
    char buffer[512];
    snprintf(nombreFormato, sizeof(nombreFormato), "format_concat_%d", gen->admin_registros->contador_constantes++);
    snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreFormato, formato);
    insertarEnSeccionData(gen, buffer);
    
    // Generar código para cargar argumentos
    snprintf(buffer, sizeof(buffer), "    // Printf optimizado para concatenación\n");
    strcat(codigo, buffer);
    
    int regIndex = 1; // x1, x2, x3, ...
    for (int i = 0; i < numPartes; i++) {
        NodoAST* parte = partes[i].nodo;
        TipoDato tipo = partes[i].tipo;
        
        // Saltar strings literales (ya están en el formato)
        if (tipo == TIPO_STRING && parte->tipo == AST_CADENA) continue;
        
        if (tipo == TIPO_INT) {
            if (parte->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n",
                    regIndex, parte->valor, regIndex, regIndex);
            } else if (parte->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%s\n",
                    regIndex, parte->valor);
            }
            strcat(codigo, buffer);
            regIndex++;
        } else if (tipo == TIPO_FLOAT || tipo == TIPO_DOUBLE) {
            if (parte->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n",
                    regIndex, parte->valor, regIndex - 1, regIndex);
            }
            strcat(codigo, buffer);
            regIndex++;
        } else if (tipo == TIPO_CHAR) {
            if (parte->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldrb w%d, [x%d]\n",
                    regIndex, parte->valor, regIndex, regIndex);
            } else if (parte->tipo == AST_CARACTER) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n",
                    regIndex, (int)parte->valor[0]);
            }
            strcat(codigo, buffer);
            regIndex++;
        } else if (tipo == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            // Cargar "true" o "false" según el valor
            if (parte->tipo == AST_IDENTIFICADOR) {
                // Necesitamos convertir 0/1 a "false"/"true"
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldrb w%d, [x%d]\n"
                    "    cmp w%d, #0\n"
                    "    ldr x%d, =str_false\n"
                    "    ldr x19, =str_true\n"
                    "    csel x%d, x19, x%d, ne\n",
                    regIndex, parte->valor, regIndex, regIndex, regIndex, regIndex, regIndex, regIndex);
            } else if (parte->tipo == AST_BOOLEAN) {
                const char* strBool = (strcmp(parte->valor, "true") == 0) ? "str_true" : "str_false";
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n",
                    regIndex, strBool);
            }
            strcat(codigo, buffer);
            regIndex++;
        } else if (tipo == TIPO_STRING && parte->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr x%d, [x%d]\n",
                regIndex, parte->valor, regIndex, regIndex);
            strcat(codigo, buffer);
            regIndex++;
        }
    }
    
    // Cargar formato y llamar a printf
    snprintf(buffer, sizeof(buffer),
        "    ldr x0, =%s\n"
        "    bl printf\n",
        nombreFormato);
    strcat(codigo, buffer);
    
    return true;
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

// Función para procesar operaciones recursivamente
static void procesarOperacionRecursiva(GeneradorDeclaraciones* gen, NodoAST* nodo, char* codigo, int* regIndex) {
    if (!nodo) return;

    char buffer[256];

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // Procesar recursivamente los operandos
        procesarOperacionRecursiva(gen, izq, codigo, regIndex);
        int regIzq = *regIndex - 1;
        procesarOperacionRecursiva(gen, der, codigo, regIndex);
        int regDer = *regIndex - 1;

        // Generar código para la operación
        if (strcmp(nodo->valor, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add w%d, w%d, w%d\n", *regIndex, regIzq, regDer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub w%d, w%d, w%d\n", *regIndex, regIzq, regDer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul w%d, w%d, w%d\n", *regIndex, regIzq, regDer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv w%d, w%d, w%d\n", *regIndex, regIzq, regDer);
        }
        strcat(codigo, buffer);
        (*regIndex)++;
    } else if (nodo->tipo == AST_NUMERO) {
        // Cargar un número literal
        snprintf(buffer, sizeof(buffer), "    mov w%d, #%s\n", *regIndex, nodo->valor);
        strcat(codigo, buffer);
        (*regIndex)++;
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        // Cargar el valor de una variable
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr w%d, [x%d]\n",
            *regIndex, nodo->valor, *regIndex, *regIndex);
        strcat(codigo, buffer);
        (*regIndex)++;
    } else if (nodo->tipo == AST_CARACTER) {
        // Cargar el valor ASCII del carácter en el registro actual
        snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", *regIndex, (int)nodo->valor[0]);
        strcat(codigo, buffer);
        (*regIndex)++;
    }
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
        

        // **PRIORIDAD 1: Si es concatenación, intentar generar printf optimizado**
        if (valorNodo->tipo == AST_OPERACION && strcmp(valorNodo->valor, "+") == 0) {
            if (contieneStringEnOperacion(valorNodo)) {
                // Concatenación de strings (usa tu lógica actual)
                if (generarPrintfDesdeConcatenacion(gen, valorNodo, codigo)) {
                    printf("[DEBUG] Printf optimizado generado para concatenación\n");
                } else {
                    insertarConstantesBooleanas(gen);
                    // Fallback a StringBuilder si no se puede optimizar
                    printf("[DEBUG] Fallback a StringBuilder para concatenación compleja\n");
                    const char* nombreTemp = "__temp_print_str";
                    generarConcatenacionStringBuilder(gen, nombreTemp, valorNodo);
                    char formatoConst[64];
                    snprintf(formatoConst, sizeof(formatoConst), "format_string_%d", gen->admin_registros->contador_constantes++);
                    snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoConst);
                    insertarEnSeccionData(gen, buffer);
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    ldr x1, [x1]\n"
                        "    ldr x0, =%s\n"
                        "    bl printf\n",
                        nombreTemp, formatoConst);
                    strcat(codigo, buffer);
                }
            } else {
                // Operación aritmética (suma de números, chars, etc.)
                int regIndex = 1;
                procesarOperacionRecursiva(gen, valorNodo, codigo, &regIndex);
                char formatoInt[64];
                snprintf(formatoInt, sizeof(formatoInt), "format_int_%d", gen->admin_registros->contador_constantes++);
                snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoInt);
                insertarEnSeccionData(gen, buffer);
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    mov w1, w%d\n"
                    "    bl printf\n",
                    formatoInt, regIndex - 1);
                strcat(codigo, buffer);
            }

        } else if (tipo == TIPO_STRING) {
            // String simple (no concatenación)
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
        } else if (tipo == TIPO_FLOAT) {
            char formatoFloat[64];
            snprintf(formatoFloat, sizeof(formatoFloat), "format_float_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%\\n\"\n", formatoFloat);
            insertarEnSeccionData(gen, buffer);

            // Imprimir variable double/float
            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"      // x1 = dirección de la variable
                    "    ldr d0, [x1]\n"     // d0 = valor de la variable
                    "    ldr x0, =%s\n"      // x0 = formato
                    "    bl printf\n",
                    valorNodo->valor, formatoFloat);
                strcat(codigo, buffer);
            } else if (valorNodo->tipo == AST_NUMERO) {
                // Si es un literal, crea una constante en .rodata
                char nombreConst[64];
                snprintf(nombreConst, sizeof(nombreConst), "float_const_%d", gen->admin_registros->contador_constantes++);
                insertarConstanteDoubleRoData(gen, nombreConst, atof(valorNodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr d0, [x1]\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    nombreConst, formatoFloat);
                strcat(codigo, buffer);
            }
        } else if(tipo == TIPO_DOUBLE){
            char formatoDouble[64];
            snprintf(formatoDouble, sizeof(formatoDouble), "format_double_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .double \"%%\\n\"\n", formatoDouble);
            insertarEnSeccionData(gen, buffer);

            // Imprimir variable double/float
            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"      // x1 = dirección de la variable
                    "    ldr d0, [x1]\n"     // d0 = valor de la variable
                    "    ldr x0, =%s\n"      // x0 = formato
                    "    bl printf\n",
                    valorNodo->valor, formatoDouble);
                strcat(codigo, buffer);
            } else if (valorNodo->tipo == AST_NUMERO) {
                // Si es un literal, crea una constante en .rodata
                char nombreConst[64];
                snprintf(nombreConst, sizeof(nombreConst), "double_const_%d", gen->admin_registros->contador_constantes++);
                insertarConstanteDoubleRoData(gen, nombreConst, atof(valorNodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr d0, [x1]\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    nombreConst, formatoDouble);
                strcat(codigo, buffer);
            }

        }else if (tipo == TIPO_VECTOR && valorNodo->tipo == AST_IDENTIFICADOR) {
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