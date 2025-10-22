#include "Impresion_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Operadores_arm64.h"
#include "EscapeSecuencia_arm64.h" 
#include "Embebidas/EvaluadorExpr_arm64.h"
#include <stdio.h>
#include <string.h>

bool linea_vacia = false;
extern NodoAST* astGlobal;



// **ESTRUCTURAS PARA ANÁLISIS DE CONCATENACIÓN EN PRINTS**
#define MAX_PARTES_CONCAT 16
typedef struct {
    NodoAST* nodo;
    TipoDato tipo;
} ParteConcatenacion;

static void recolectarPartesRecursivas(NodoAST* nodo, ParteConcatenacion* partes, int* numPartes);
static bool procesarParteParaFormato(GeneradorDeclaraciones* gen, ParteConcatenacion* parte, char* formato, char* codigoArgs, int* regIndex);


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

static void escaparPorcentaje(char* destino, const char* origen, size_t tam) {
    size_t j = 0;
    for (size_t i = 0; origen[i] != '\0' && j < tam - 1; i++) {
        if (origen[i] == '%') {
            // Si el siguiente caracter es una letra de formato, NO escapamos
            char next = origen[i+1];
            if (next == 'd' || next == 's' || next == 'f' || next == 'c') {
                destino[j++] = '%';
            } else {
                // Escapamos solo si no es un especificador
                if (j < tam - 2) {
                    destino[j++] = '%';
                    destino[j++] = '%';
                }
            }
        } else {
            destino[j++] = origen[i];
        }
    }
    destino[j] = '\0';
}


static void insertarConstantesBooleanas(GeneradorDeclaraciones* gen) {
    if (!gen->temp_str_valueof_bss_insertado) {
        insertarEnSeccionBss(gen, "temp_str_valueof: .skip 32\n");
        gen->temp_str_valueof_bss_insertado = true;
    }
}


// En Impresion_arm64.c - AGREGAR ESTA FUNCIÓN
static bool esConcatenacionConStringValof(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_OPERACION || strcmp(nodo->valor, "+") != 0) 
        return false;
    
    // Verificar si algún lado tiene String.valueOf o es string
    for (int i = 0; i < nodo->num_hijos; i++) {
        NodoAST* hijo = nodo->hijos[i];
        if (hijo->tipo == AST_VALUEOF || 
            deducirTipoExprARM64(hijo, astGlobal) == TIPO_STRING) {
            return true;
        }
        // Recursivo para concatenaciones anidadas
        if (esConcatenacionConStringValof(hijo)) {
            return true;
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

// En Impresion_arm64.c - AGREGAR ESTA FUNCIÓN
static bool generarConcatenacionOptimizada(GeneradorDeclaraciones* gen, NodoAST* valorNodo, char* codigo) {
    // Recolectar todas las partes usando evaluador recursivo
    ParteConcatenacion partes[MAX_PARTES_CONCAT];
    int numPartes = 0;
    
    recolectarPartesRecursivas(valorNodo, partes, &numPartes);
    
    if (numPartes == 0) return false;
    
    // Construir formato dinámico
    char formato[512] = "";
    char codigoArgs[1024] = "";
    int regIndex = 1;
    
    for (int i = 0; i < numPartes; i++) {
        if (!procesarParteParaFormato(gen, &partes[i], formato, codigoArgs, &regIndex)) {
            printf("[DEPURACION] Concatenación NO soportada, no se imprime: formato=%s, args=%s\n", formato, codigoArgs);
            return false; // Activar StringBuilder como fallback
        }
    }
    
    strcat(formato, "\\n");

    char formatoEscapado[512];
    escaparPorcentaje(formatoEscapado, formato, sizeof(formatoEscapado));
    
    // Crear formato y generar código
    char nombreFormato[64];
    snprintf(nombreFormato, sizeof(nombreFormato), "format_concat_%d", gen->admin_registros->contador_formatos++);
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreFormato, formatoEscapado);
    insertarEnSeccionData(gen, buffer);
    
    // Código final
    strcat(codigo, "    // Printf optimizado para concatenación\n");
    strcat(codigo, codigoArgs);
    snprintf(buffer, sizeof(buffer), "    ldr x0, =%s\n    bl printf\n", nombreFormato);
    strcat(codigo, buffer);
    
    printf("[DEBUG] Printf optimizado generado para concatenación\n");
    printf("[DEPURACION] Concatenación soportada, se imprime: formato=%s, args=%s\n", formato, codigoArgs);
    return true;
}

// En Impresion_arm64.c - AGREGAR ESTA FUNCIÓN
static void recolectarPartesRecursivas(NodoAST* nodo, ParteConcatenacion* partes, int* numPartes) {
    if (!nodo || *numPartes >= MAX_PARTES_CONCAT) return;
    
    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "+") == 0) {
        // Si es concatenación, procesar ambos lados recursivamente
        for (int i = 0; i < nodo->num_hijos; i++) {
            recolectarPartesRecursivas(nodo->hijos[i], partes, numPartes);
        }
    } else {
        // Nodo hoja: literal, variable, o String.valueOf(expr)
        TipoDato tipo;
        
        if (nodo->tipo == AST_VALUEOF && nodo->num_hijos == 1) {
            tipo = deducirTipoExprARM64(nodo->hijos[0], astGlobal);
            printf("[DEBUG CONCAT] Parte: tipo=%d, valor=%s, deducido=%d\n", nodo->tipo, nodo->valor ? nodo->valor : "(null)", tipo);
            if (tipo == TIPO_BOOLEAN) {
                partes[*numPartes].nodo = nodo;
                partes[*numPartes].tipo = TIPO_BOOLEAN;
                (*numPartes)++;
                printf("[DEBUG CONCAT] Parte: tipo=%d, valor=%s, deducido=%d (forzado BOOL)\n", 
                    nodo->tipo, nodo->valor ? nodo->valor : "(null)", TIPO_BOOLEAN);
                return;
            }
        } else {
            tipo = deducirTipoExprARM64(nodo, astGlobal);
        }
        
        partes[*numPartes].nodo = nodo;
        partes[*numPartes].tipo = tipo;
        (*numPartes)++;
        
        printf("[DEBUG CONCAT] Parte: tipo=%d, valor=%s, deducido=%d\n", 
               nodo->tipo, nodo->valor ? nodo->valor : "(null)", tipo);
    }
}

// En Impresion_arm64.c - AGREGAR ESTA FUNCIÓN
static bool procesarParteParaFormato(GeneradorDeclaraciones* gen, ParteConcatenacion* parte, 
                                   char* formato, char* codigoArgs, int* regIndex) {
    NodoAST* nodo = parte->nodo;
    TipoDato tipo = parte->tipo;
    char buffer[256];
    printf("[DEBUG] procesarParteParaFormato: nodo tipo=%d, valor=%s, tipo deducido=%d\n",
       nodo->tipo, nodo->valor ? nodo->valor : "(null)", tipo);


    // 1. STRING LITERAL
    if (tipo == TIPO_STRING && nodo->tipo == AST_CADENA) {
        strcat(formato, nodo->valor);
        return true;
    }

    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "!") == 0) {
        // Es una negación booleana
        strcat(formato, "%d"); // O "%s" si quieres "true"/"false"
        ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoArgs, resultado.codigo);
        snprintf(buffer, sizeof(buffer), "    mov w1, %s\n", resultado.resultado);
        strcat(codigoArgs, buffer);
        return true;
    }
    
    // 2. STRING.VALUEOF(EXPR)
    if (nodo->tipo == AST_VALUEOF && nodo->num_hijos == 1) {
        // Evaluar expresión recursivamente
        ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
        strcat(codigoArgs, resultado.codigo);
        
        if (tipo == TIPO_INT) {
            strcat(formato, "%d");
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", *regIndex,resultado.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        } else if (tipo == TIPO_DOUBLE) {
            strcat(formato, "%f");
            snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", *regIndex - 1);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        } else if( tipo == TIPO_FLOAT) {
            strcat(formato, "%f");
                // Cargar el valor float en sN y convertir a double si printf lo requiere
                snprintf(buffer, sizeof(buffer),
                    "    fcvt d%d, s%d\n", // Convierte float a double para printf
                    *regIndex - 2 ,*regIndex - 2);
                strcat(codigoArgs, buffer);
                printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
                return true;

        } if (tipo == TIPO_CHAR) {
            strcat(formato, "%c");
            // Si el nodo es AST_VALUEOF, usar el resultado del hijo
            if (nodo->tipo == AST_VALUEOF && nodo->num_hijos == 1) {
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
                strcat(codigoArgs, resultado.codigo);
                // Mueve el valor al registro de argumento
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", *regIndex, resultado.resultado);
                strcat(codigoArgs, buffer);
                (*regIndex)++;
                return true;
            } else if (nodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldrb w%d, [x%d]\n",
                    *regIndex, nodo->valor, *regIndex, *regIndex);
                strcat(codigoArgs, buffer);
                (*regIndex)++;
                return true;
            }
        } 
        else if( tipo == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            strcat(formato, "%s");
            snprintf(buffer, sizeof(buffer),
                "    mov w0, %s\n"
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_bool\n"
                "    ldr x1, =temp_str_valueof\n",
                resultado.resultado);
        } else if (tipo == TIPO_STRING) {
            strcat(formato, "%s");
            snprintf(buffer, sizeof(buffer), "    mov x%d, %s\n", *regIndex - 1, resultado.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        } else {
            printf("[DEPURACION] Fallback en procesarParteParaFormato: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return false; // Tipo no soportado para String.valueOf
        }
        strcat(codigoArgs, buffer);
        printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
        return true;
    }
    
    // 3. OTROS TIPOS (int, float, string variables, etc.)
    switch (tipo) {
        case TIPO_INT:
            strcat(formato, "%d");
            // Usar generarExprEmbebidaARM64 para cualquier expresión int
            ResultadoExprEmbebida resInt = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resInt.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", *regIndex - 1, resInt.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
            
        case TIPO_FLOAT:
            strcat(formato, "%f");
            ResultadoExprEmbebida resFloat = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resFloat.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, %s\n", *regIndex - 1, resFloat.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        
        case TIPO_DOUBLE:
            strcat(formato, "%f");
            ResultadoExprEmbebida resDouble = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resDouble.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov s%d, %s\n", *regIndex - 1, resDouble.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        case TIPO_CHAR:
            strcat(formato, "%c");
            ResultadoExprEmbebida resChar = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resChar.codigo);

            // Si el resultado es un registro (ejemplo: resChar.resultado == "w1")
            snprintf(buffer, sizeof(buffer),
                "    mov w1, %s\n", resChar.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
            
        case TIPO_STRING:
            if (nodo->tipo == AST_IDENTIFICADOR || nodo->tipo == AST_CADENA) {
                strcat(formato, "%s");
               ResultadoExprEmbebida resStr = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resStr.codigo);
                // Solo agrega el bloque de carga si resStr.codigo está vacío (es decir, si no se generó ya)
                if (resStr.codigo[0] == '\0') {
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    ldr x1, [x1]\n",
                        resStr.resultado);
                    strcat(codigoArgs, buffer);
                }
                printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
                return true;
            }
            printf("[DEPURACION] Fallback en procesarParteParaFormato: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return false;
        case TIPO_BOOLEAN:
            insertarConstantesBooleanas(gen);
            strcat(formato, "%d");
            ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resultado.codigo);
            snprintf(buffer, sizeof(buffer),
                "    mov w0, %s\n"
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_bool\n"
                "    ldr x1, =temp_str_valueof\n",
                resultado.resultado);
            strcat(codigoArgs, buffer);
            printf("[DEPURACION] Parte soportada: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return true;
        default:
        printf("[DEPURACION] Fallback en procesarParteParaFormato: tipo=%d, nodo tipo=%d, valor=%s\n", tipo, nodo->tipo, nodo->valor ? nodo->valor : "(null)");
            return false; // Tipo no soportado
    }
}

bool esConcatenacion = false;

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
            if (esConcatenacionConStringValof(valorNodo)) {
                if (generarConcatenacionOptimizada(gen, valorNodo, codigo)) {
                    printf("[DEPURACION] Insertando impresión ARM64: \n%s\n", codigo);
                    insertarEnSeccionText(gen, codigo);
                    return;
                }
            }
        }
        // PRIORIDAD 2: String simple
        else if (tipo == TIPO_STRING) {
            char formatoConst[64];
            snprintf(formatoConst, sizeof(formatoConst), "format_string_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoConst);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                bool esAsciz = esVariableAscizImpresion(gen, valorNodo->valor);
                if (esAsciz) {
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    ldr x0, =%s\n"
                        "    bl printf\n",
                        valorNodo->valor, formatoConst);
                } else {
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    ldr x1, [x1]\n"
                        "    ldr x0, =%s\n"
                        "    bl printf\n",
                        valorNodo->valor, formatoConst);
                }
                strcat(codigo, buffer);
            } else {
                char* cadenaEscapada = limpiarCadenaParaASM(valorNodo->valor);
                char cadenaPorcentaje[256];
                escaparPorcentaje(cadenaPorcentaje, cadenaEscapada, sizeof(cadenaPorcentaje));
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
        }
        // PRIORIDAD 3: Caracter
        else if (tipo == TIPO_CHAR) {
            char formatoChar[64];
            snprintf(formatoChar, sizeof(formatoChar), "format_char_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\\n\"\n", formatoChar);
            insertarEnSeccionData(gen, buffer);

            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldrb w1, [x1]\n"
                "    ldr x0, =%s\n"
                "    bl printf\n",  
                valorNodo->valor, formatoChar);
            strcat(codigo, buffer);
        }
        // PRIORIDAD 4: Entero
        else if (tipo == TIPO_INT) {
            char formatoInt[64];
            snprintf(formatoInt, sizeof(formatoInt), "format_int_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoInt);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr w1, [x1]\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    valorNodo->valor, formatoInt);
            } else {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    mov w1, #%d\n"
                    "    bl printf\n",
                    formatoInt, atoi(valorNodo->valor));
            }
            strcat(codigo, buffer);
        }
        else if (valorNodo->tipo == AST_VALUEOF && valorNodo->num_hijos == 1) {
            TipoDato tipoValueOf = deducirTipoExprARM64(valorNodo->hijos[0], astGlobal);

            if (tipoValueOf == TIPO_FLOAT || tipoValueOf == TIPO_DOUBLE) {
                char formatoFloat[64];
                snprintf(formatoFloat, sizeof(formatoFloat), "format_float_%d", gen->admin_registros->contador_constantes++);
                snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoFloat);
                insertarEnSeccionData(gen, buffer);

                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo->hijos[0], &(int){1});
                strcat(codigo, resultado.codigo);

                // Cargar el valor en s0/d0 y convertir si es float
                if (tipoValueOf == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer),
                        "    fcvt d0, s0\n");
                    strcat(codigo, buffer);
                }

                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    formatoFloat);
                strcat(codigo, buffer);
                insertarEnSeccionText(gen, codigo);
                return;
            }
            // Puedes agregar otros tipos aquí si lo necesitas
        }

        // PRIORIDAD 5: Float/Double
        else if (tipo == TIPO_FLOAT || tipo == TIPO_DOUBLE) {
            char formatoFloat[64];
            snprintf(formatoFloat, sizeof(formatoFloat), "format_float_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoFloat);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                // Cargar el valor en d0
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr %s, [x1]\n",
                    valorNodo->valor,
                    (tipo == TIPO_DOUBLE ? "d0" : "s0"));
                strcat(codigo, buffer);

                // Si es float, convertir a double antes de imprimir
                if (tipo == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer),
                        "    fcvt d0, s0\n");
                    strcat(codigo, buffer);
                }

                // Llamar a printf
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    formatoFloat);
                strcat(codigo, buffer);
            } else if (valorNodo->tipo == AST_NUMERO) {
                // Crear constante en .data/.rodata
                char nombreConst[64];
                snprintf(nombreConst, sizeof(nombreConst), "%s_const_%d", (tipo == TIPO_FLOAT ? "float" : "double"), gen->admin_registros->contador_constantes++);
                if (tipo == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer), "%s: .float %s\n", nombreConst, valorNodo->valor);
                } else {
                    snprintf(buffer, sizeof(buffer), "%s: .double %s\n", nombreConst, valorNodo->valor);
                }
                insertarEnSeccionData(gen, buffer);

                // Cargar el valor en d0/s0
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr %s, [x1]\n",
                    nombreConst,
                    (tipo == TIPO_DOUBLE ? "d0" : "s0"));
                strcat(codigo, buffer);

                // Si es float, convertir a double antes de imprimir
                if (tipo == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer),
                        "    fcvt d0, s0\n");
                    strcat(codigo, buffer);
                }

                // Llamar a printf
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    formatoFloat);
                strcat(codigo, buffer);
            }
        } else if (tipo == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            char formatoBool[64];
            snprintf(formatoBool, sizeof(formatoBool), "format_bool_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoBool);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldrb w1, [x1]\n"
                    "    cmp w1, #0\n"
                    "    ldr x2, =str_false\n"
                    "    ldr x3, =str_true\n"
                    "    csel x1, x3, x2, ne\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    valorNodo->valor, formatoBool);
            } else if (valorNodo->tipo == AST_BOOLEAN) {
                const char* strBool = (strcmp(valorNodo->valor, "true") == 0) ? "str_true" : "str_false";
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    strBool, formatoBool);
            }
            strcat(codigo, buffer);
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
        } else if (tipo == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            char formatoBool[64];
            snprintf(formatoBool, sizeof(formatoBool), "format_bool_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoBool);
            insertarEnSeccionData(gen, buffer);

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldrb w1, [x1]\n"
                    "    cmp w1, #0\n"
                    "    ldr x2, =str_false\n"
                    "    ldr x3, =str_true\n"
                    "    csel x1, x3, x2, ne\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    valorNodo->valor, formatoBool);
            } else if (valorNodo->tipo == AST_BOOLEAN) {
                const char* strBool = (strcmp(valorNodo->valor, "true") == 0) ? "str_true" : "str_false";
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    strBool, formatoBool);
            } else {
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo, &(int){1});
                strcat(codigo, resultado.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov w0, %s\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    bl valueOf_bool\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    resultado.resultado, formatoBool);
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