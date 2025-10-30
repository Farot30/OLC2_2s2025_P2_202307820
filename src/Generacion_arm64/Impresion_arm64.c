#include "Impresion_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Operadores_arm64.h"
#include "EscapeSecuencia_arm64.h" 
#include "Embebidas/EvaluadorExpr_arm64.h"
#include "Vectores_arm64/AccesoVector_arm64.h"
#include "Vectores_arm64/AccesoMatriz_arm64.h"
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
    
    // ✅ AGREGAR str_true Y str_false EN .data
    if (!strstr(gen->buffer, "str_true:")) {
        insertarEnSeccionData(gen, "str_true: .asciz \"true\"\n");
    }
    if (!strstr(gen->buffer, "str_false:")) {
        insertarEnSeccionData(gen, "str_false: .asciz \"false\"\n");
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

static void generarImpresionMatrizCompleta(GeneradorDeclaraciones* gen, const char* nombreMatriz, 
                                          TipoDato tipoBase, int filas, int columnas) {
    char codigo[2048] = "";
    char buffer[256];
    
    // Crear formatos necesarios
    snprintf(buffer, sizeof(buffer), "format_mat_open: .asciz \"[\\n\"\n");
    insertarEnSeccionData(gen, buffer);
    snprintf(buffer, sizeof(buffer), "format_mat_close: .asciz \"]\\n\"\n");
    insertarEnSeccionData(gen, buffer);
    snprintf(buffer, sizeof(buffer), "format_mat_row_open: .asciz \"  [\"\n");
    insertarEnSeccionData(gen, buffer);
    snprintf(buffer, sizeof(buffer), "format_mat_row_close: .asciz \"]\\n\"\n");
    insertarEnSeccionData(gen, buffer);
    snprintf(buffer, sizeof(buffer), "format_mat_sep: .asciz \", \"\n");
    insertarEnSeccionData(gen, buffer);
    
    // Formato según el tipo
    const char* formatoElem;
    const char* instrLoad;
    int tamElem;
    const char* regType;
    
    switch (tipoBase) {
        case TIPO_INT:
            formatoElem = "format_mat_int";
            instrLoad = "ldr w";
            tamElem = 4;
            regType = "w";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\"\n", formatoElem);
            break;
        case TIPO_FLOAT:
            formatoElem = "format_mat_float";
            instrLoad = "ldr s";
            tamElem = 4;
            regType = "s";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\"\n", formatoElem);
            break;
        case TIPO_DOUBLE:
            formatoElem = "format_mat_double";
            instrLoad = "ldr d";
            tamElem = 8;
            regType = "d";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\"\n", formatoElem);
            break;
        case TIPO_CHAR:
            formatoElem = "format_mat_char";
            instrLoad = "ldrb w";
            tamElem = 1;
            regType = "w";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\"\n", formatoElem);
            break;
        case TIPO_STRING:
            formatoElem = "format_mat_str";
            instrLoad = "ldr x";
            tamElem = 8;
            regType = "x";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\"\n", formatoElem);
            break;
        default:
            formatoElem = "format_mat_int";
            instrLoad = "ldr w";
            tamElem = 4;
            regType = "w";
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\"\n", formatoElem);
    }
    insertarEnSeccionData(gen, buffer);
    
    // 1. Imprimir apertura "[\n"
    snprintf(buffer, sizeof(buffer),
        "    ldr x0, =format_mat_open\n"
        "    bl printf\n");
    strcat(codigo, buffer);
    
    // 2. Variables: tamaño matriz, contador fila, contador columna
    snprintf(buffer, sizeof(buffer),
        "    mov w19, #%d\n"        // Total filas
        "    mov w20, #%d\n"        // Total columnas
        "    mov w21, #0\n"         // Contador fila
        "    ldr x22, =%s\n",       // Base de la matriz
        filas, columnas, nombreMatriz);
    strcat(codigo, buffer);
    
    // 3. Loop de filas
    snprintf(buffer, sizeof(buffer),
        "print_mat_row_loop_%s:\n"
        "    cmp w21, w19\n"
        "    bge print_mat_end_%s\n"
        "    ldr x0, =format_mat_row_open\n"
        "    bl printf\n"
        "    mov w23, #0\n",        // Contador columna
        nombreMatriz, nombreMatriz);
    strcat(codigo, buffer);
    
    // 4. Loop de columnas
    snprintf(buffer, sizeof(buffer),
        "print_mat_col_loop_%s:\n"
        "    cmp w23, w20\n"
        "    bge print_mat_row_end_%s\n",
        nombreMatriz, nombreMatriz);
    strcat(codigo, buffer);
    
    // 5. Calcular offset: (fila * columnas + columna) * tamElem
    snprintf(buffer, sizeof(buffer),
        "    mul w24, w21, w20\n"   // fila * columnas
        "    add w24, w24, w23\n"   // + columna
        "    mov w25, #%d\n"        // tamaño elemento
        "    mul w24, w24, w25\n"   // offset total
        "    add x26, x22, w24, sxtw\n", // dirección elemento
        tamElem);
    strcat(codigo, buffer);
    
    // 6. Cargar e imprimir elemento
    if (tipoBase == TIPO_FLOAT) {
        snprintf(buffer, sizeof(buffer),
            "    %s1, [x26]\n"
            "    fcvt d1, s1\n"      // Convertir float a double para printf
            "    ldr x0, =%s\n"
            "    bl printf\n",
            instrLoad, formatoElem);
    } else if (tipoBase == TIPO_DOUBLE) {
        snprintf(buffer, sizeof(buffer),
            "    %s1, [x26]\n"
            "    ldr x0, =%s\n"
            "    bl printf\n",
            instrLoad, formatoElem);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    %s1, [x26]\n"
            "    ldr x0, =%s\n"
            "    bl printf\n",
            instrLoad, formatoElem);
    }
    strcat(codigo, buffer);
    
    // 7. Separador entre elementos (si no es el último)
    snprintf(buffer, sizeof(buffer),
        "    add w23, w23, #1\n"
        "    cmp w23, w20\n"
        "    bge print_mat_col_loop_%s\n"
        "    ldr x0, =format_mat_sep\n"
        "    bl printf\n"
        "    b print_mat_col_loop_%s\n",
        nombreMatriz, nombreMatriz);
    strcat(codigo, buffer);
    
    // 8. Cerrar fila
    snprintf(buffer, sizeof(buffer),
        "print_mat_row_end_%s:\n"
        "    ldr x0, =format_mat_row_close\n"
        "    bl printf\n"
        "    add w21, w21, #1\n"
        "    b print_mat_row_loop_%s\n",
        nombreMatriz, nombreMatriz);
    strcat(codigo, buffer);
    
    // 9. Cerrar matriz
    snprintf(buffer, sizeof(buffer),
        "print_mat_end_%s:\n"
        "    ldr x0, =format_mat_close\n"
        "    bl printf\n",
        nombreMatriz);
    strcat(codigo, buffer);
    
    insertarEnSeccionText(gen, codigo);
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

static bool procesarParteParaFormato(GeneradorDeclaraciones* gen, ParteConcatenacion* parte, 
                                   char* formato, char* codigoArgs, int* regIndex) {
    NodoAST* nodo = parte->nodo;
    TipoDato tipo = parte->tipo;
    char buffer[256];
    
    // Contar argumentos ya procesados en el formato
    int numArgs = 0;
    for (int i = 0; formato[i]; i++) {
        if (formato[i] == '%' && formato[i+1] != '%' && 
            (formato[i+1] == 'd' || formato[i+1] == 's' || formato[i+1] == 'f' || formato[i+1] == 'c')) {
            numArgs++;
        }
    }
    int regArg = numArgs + 1; // x1 para el primer argumento, x2 para el segundo, etc.

    printf("[DEBUG procesarParteParaFormato] nodo tipo=%d, valor=%s, tipo=%d, regArg=%d, numArgs=%d\n",
           nodo->tipo, nodo->valor ? nodo->valor : "(null)", tipo, regArg, numArgs);

    // 1. STRING LITERAL - No consume registros
    if (tipo == TIPO_STRING && nodo->tipo == AST_CADENA) {
        strcat(formato, nodo->valor);
        return true;
    }

    // 2. OPERADORES LÓGICOS
    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "!") == 0) {
        strcat(formato, "%d");
        ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoArgs, resultado.codigo);
        snprintf(buffer, sizeof(buffer), 
            "    mov w3, %s\n"
            "    mov w%d, w3\n", 
            resultado.resultado, regArg);
        strcat(codigoArgs, buffer);
        return true;
    }
    
    // 3. STRING.VALUEOF
    if (nodo->tipo == AST_VALUEOF && nodo->num_hijos == 1) {
        TipoDato tipoInterno = deducirTipoExprARM64(nodo->hijos[0], astGlobal);
        
        if (tipoInterno == TIPO_INT) {
            strcat(formato, "%d");
            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr w3, [x3]\n"
                    "    mov w%d, w3\n",
                    nodo->hijos[0]->valor, regArg);
            } else if (nodo->hijos[0]->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%s\n",
                    regArg, nodo->hijos[0]->valor);
            } else {
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
                strcat(codigoArgs, resultado.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n",
                    resultado.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
        } if (tipoInterno == TIPO_DOUBLE) {
            strcat(formato, "%f");
            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                // ✅ CORRECCIÓN: Usar d0 para el primer double, d1 para el segundo
                int regDouble = (numArgs == 0) ? 0 : 1; // d0 si es el primero, d1 si es el segundo
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr d3, [x3]\n"
                    "    fmov d%d, d3\n",
                    nodo->hijos[0]->valor, regDouble);
            } else {
                int regDouble = (numArgs == 0) ? 0 : 1;
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
                strcat(codigoArgs, resultado.codigo);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDouble);
            }
            strcat(codigoArgs, buffer);
            return true;
        } if (tipoInterno == TIPO_FLOAT) {
            strcat(formato, "%f");
            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                // ✅ CORRECCIÓN: Convertir a double y usar d0/d1
                int regDouble = (numArgs == 0) ? 0 : 1;
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr s3, [x3]\n"
                    "    fcvt d%d, s3\n",
                    nodo->hijos[0]->valor, regDouble);
            } else {
                int regDouble = (numArgs == 0) ? 0 : 1;
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
                strcat(codigoArgs, resultado.codigo);
                snprintf(buffer, sizeof(buffer), "    fcvt d%d, s0\n", regDouble);
            }
            strcat(codigoArgs, buffer);
            return true;
        } else if (tipoInterno == TIPO_CHAR) {
            strcat(formato, "%c");
            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldrb w3, [x3]\n"
                    "    mov w%d, w3\n",
                    nodo->hijos[0]->valor, regArg);
            } else {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%s\n", regArg, nodo->hijos[0]->valor);
            }
            strcat(codigoArgs, buffer);
            return true;
        } else if (tipoInterno == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            strcat(formato, "%s");
            ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
            strcat(codigoArgs, resultado.codigo);
            snprintf(buffer, sizeof(buffer),
                "    mov w0, %s\n"
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_bool\n"
                "    mov x3, x0\n"
                "    mov x%d, x3\n",
                resultado.resultado, regArg);
            strcat(codigoArgs, buffer);
            return true;
        } else if (tipoInterno == TIPO_STRING) {
            strcat(formato, "%s");
            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr x3, [x3]\n"
                    "    mov x%d, x3\n",
                    nodo->hijos[0]->valor, regArg);
            } else {
                ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, nodo->hijos[0], regIndex);
                strcat(codigoArgs, resultado.codigo);
                snprintf(buffer, sizeof(buffer), "    mov x%d, %s\n", regArg, resultado.resultado);
            }
            strcat(codigoArgs, buffer);
            return true;
        }
        return false;
    }

    // 4. ACCESO A VECTOR/MATRIZ
    if (nodo->tipo == AST_ACCESO) {
        int numDimensiones = contarDimensionesAcceso(nodo);
        
        if (numDimensiones == 2) {
            // Acceso a matriz
            const char* nombreMatriz = nodo->hijos[0]->valor;
            NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreMatriz);
            if (!matrizDecl) return false;
            
            TipoDato tipoMatriz = obtenerTipoMatrizDesdeAST(matrizDecl);
            ResultadoExprEmbebida resAcceso = generarAccesoMatrizARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resAcceso.codigo);
            
            if (tipoMatriz == TIPO_INT) {
                strcat(formato, "%d");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipoMatriz == TIPO_FLOAT) {
                strcat(formato, "%f");
                snprintf(buffer, sizeof(buffer), 
                    "    fcvt d3, %s\n"
                    "    fmov d%d, d3\n", 
                    resAcceso.resultado, regArg - 1);
            } else if (tipoMatriz == TIPO_DOUBLE) {
                strcat(formato, "%f");
                snprintf(buffer, sizeof(buffer), 
                    "    fmov d3, %s\n"
                    "    fmov d%d, d3\n", 
                    resAcceso.resultado, regArg - 1);
            } else if (tipoMatriz == TIPO_CHAR) {
                strcat(formato, "%c");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipoMatriz == TIPO_STRING) {
                strcat(formato, "%s");
                snprintf(buffer, sizeof(buffer), 
                    "    mov x3, %s\n"
                    "    mov x%d, x3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipoMatriz == TIPO_BOOLEAN) {
                strcat(formato, "%s");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w0, %s\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    bl valueOf_bool\n"
                    "    mov x3, x0\n"
                    "    mov x%d, x3\n",
                    resAcceso.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
        } else if (numDimensiones == 1) {
            // Acceso a vector
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
            strcat(codigoArgs, resAcceso.codigo);
            
            if (tipo == TIPO_INT) {
                strcat(formato, "%d");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipo == TIPO_CHAR) {
                strcat(formato, "%c");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipo == TIPO_STRING) {
                strcat(formato, "%s");
                snprintf(buffer, sizeof(buffer), 
                    "    mov x3, %s\n"
                    "    mov x%d, x3\n", 
                    resAcceso.resultado, regArg);
            } else if (tipo == TIPO_FLOAT) {
                strcat(formato, "%f");
                snprintf(buffer, sizeof(buffer), 
                    "    fcvt d3, %s\n"
                    "    fmov d%d, d3\n", 
                    resAcceso.resultado, regArg - 1);
            } else if (tipo == TIPO_DOUBLE) {
                strcat(formato, "%f");
                snprintf(buffer, sizeof(buffer), 
                    "    fmov d3, %s\n"
                    "    fmov d%d, d3\n", 
                    resAcceso.resultado, regArg - 1);
            } else if (tipo == TIPO_BOOLEAN) {
                strcat(formato, "%s");
                snprintf(buffer, sizeof(buffer), 
                    "    mov w0, %s\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    bl valueOf_bool\n"
                    "    mov x3, x0\n"
                    "    mov x%d, x3\n",
                    resAcceso.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
        }
    }

    // 5. ARRAY.LENGTH
    if (nodo->tipo == AST_LENGHT) {
        strcat(formato, "%d");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoArgs, resLength.codigo);
        snprintf(buffer, sizeof(buffer), 
            "    mov w3, %s\n"
            "    mov w%d, w3\n", 
            resLength.resultado, regArg);
        strcat(codigoArgs, buffer);
        return true;
    }

    // 6. INDEXOF
    if (nodo->tipo == AST_INDEXOF) {
        strcat(formato, "%d");
        ResultadoExprEmbebida resIndexOf = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoArgs, resIndexOf.codigo);
        snprintf(buffer, sizeof(buffer), 
            "    mov w3, %s\n"
            "    mov w%d, w3\n", 
            resIndexOf.resultado, regArg);
        strcat(codigoArgs, buffer);
        return true;
    }
    
    // 7. OTROS TIPOS (int, float, double, string, char, boolean)
    switch (tipo) {
        case TIPO_INT:
            strcat(formato, "%d");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr w3, [x3]\n"
                    "    mov w%d, w3\n",
                    nodo->valor, regArg);
            } else if (nodo->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%s\n", regArg, nodo->valor);
            } else {
                ResultadoExprEmbebida resInt = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resInt.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n",
                    resInt.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
            
        case TIPO_DOUBLE:
            strcat(formato, "%f");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                // ✅ CORRECCIÓN: Usar d0 para el primer double, d1 para el segundo
                int regDouble = (numArgs == 0) ? 0 : 1;
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr d3, [x3]\n"
                    "    fmov d%d, d3\n",
                    nodo->valor, regDouble);
            } else {
                int regDouble = (numArgs == 0) ? 0 : 1;
                ResultadoExprEmbebida resDouble = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resDouble.codigo);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDouble);
            }
            strcat(codigoArgs, buffer);
            return true;
        case TIPO_FLOAT:
            strcat(formato, "%f");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                // ✅ CORRECCIÓN: Convertir a double y usar d0/d1
                int regDouble = (numArgs == 0) ? 0 : 1;
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr s3, [x3]\n"
                    "    fcvt d%d, s3\n",
                    nodo->valor, regDouble);
            } else {
                int regDouble = (numArgs == 0) ? 0 : 1;
                ResultadoExprEmbebida resFloat = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resFloat.codigo);
                snprintf(buffer, sizeof(buffer), "    fcvt d%d, s0\n", regDouble);
            }
            strcat(codigoArgs, buffer);
            return true;
        
        case TIPO_CHAR:
            strcat(formato, "%c");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldrb w3, [x3]\n"
                    "    mov w%d, w3\n",
                    nodo->valor, regArg);
            } else if (nodo->tipo == AST_CADENA && nodo->valor && strlen(nodo->valor) > 0) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regArg, (int)nodo->valor[0]);
            } else {
                ResultadoExprEmbebida resChar = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resChar.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov w3, %s\n"
                    "    mov w%d, w3\n",
                    resChar.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
            
        case TIPO_STRING:
            strcat(formato, "%s");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldr x3, [x3]\n"
                    "    mov x%d, x3\n",
                    nodo->valor, regArg);
            } else if (nodo->tipo == AST_CADENA) {
                char* cadenaEscapada = limpiarCadenaParaASM(nodo->valor);
                char nombreConst[64];
                snprintf(nombreConst, sizeof(nombreConst), "str_concat_%d", gen->admin_registros->contador_constantes++);
                char bufferData[256];
                snprintf(bufferData, sizeof(bufferData), "%s: .asciz \"%s\"\n", nombreConst, cadenaEscapada);
                insertarEnSeccionData(gen, bufferData);
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    mov x%d, x3\n",
                    nombreConst, regArg);
                free(cadenaEscapada);
            } else {
                ResultadoExprEmbebida resStr = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resStr.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov x3, %s\n"
                    "    mov x%d, x3\n",
                    resStr.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
            
        case TIPO_BOOLEAN:
            insertarConstantesBooleanas(gen);
            strcat(formato, "%s");
            if (nodo->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    ldrb w0, [x3]\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    bl valueOf_bool\n"
                    "    mov x3, x0\n"
                    "    mov x%d, x3\n",
                    nodo->valor, regArg);
            } else if (nodo->tipo == AST_BOOLEAN) {
                const char* strBool = (strcmp(nodo->valor, "true") == 0) ? "str_true" : "str_false";
                snprintf(buffer, sizeof(buffer),
                    "    ldr x3, =%s\n"
                    "    mov x%d, x3\n",
                    strBool, regArg);
            } else {
                ResultadoExprEmbebida resBool = generarExprEmbebidaARM64(gen, nodo, regIndex);
                strcat(codigoArgs, resBool.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    mov w0, %s\n"
                    "    ldr x1, =temp_str_valueof\n"
                    "    bl valueOf_bool\n"
                    "    mov x3, x0\n"
                    "    mov x%d, x3\n",
                    resBool.resultado, regArg);
            }
            strcat(codigoArgs, buffer);
            return true;
            
        default:
            return false;
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
        
        if (tipo == TIPO_DESCONOCIDO && valorNodo->tipo == AST_IDENTIFICADOR) {
            // Intentar buscar como matriz
            NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, valorNodo->valor);
            if (matrizDecl) {
                printf("[DEBUG IMPRESION] Detectada impresión de matriz completa: '%s'\n", valorNodo->valor);
                
                TipoDato tipoBase = obtenerTipoMatrizDesdeAST(matrizDecl);
                int filas = 0, columnas = 0;
                obtenerDimensionesMatriz(matrizDecl, &filas, &columnas);
                
                if (filas > 0 && columnas > 0) {
                    generarImpresionMatrizCompleta(gen, valorNodo->valor, tipoBase, filas, columnas);
                    return;
                } else {
                    printf("[ERROR] No se pudieron determinar las dimensiones de la matriz '%s'\n", valorNodo->valor);
                }
            }
        }


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
        else if (valorNodo->tipo == AST_ACCESO) {
            if (!valorNodo->hijos[0] || !valorNodo->hijos[0]->valor) {
                printf("[ERROR] Nodo de acceso inválido en impresión\n");
                return;
            }
            
            // **DETECCIÓN AUTOMÁTICA: Vector o Matriz**
            int numDimensiones = contarDimensionesAcceso(valorNodo);
            const char* nombreArray = valorNodo->hijos[0]->valor;
            
            if (numDimensiones == 2) {
                // **ACCESO A MATRIZ**
                printf("[DEBUG IMPRESION] Acceso a matriz detectado\n");
                
                NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreArray);
                if (!matrizDecl) {
                    printf("[ERROR] Matriz '%s' no encontrada para impresión\n", nombreArray);
                    return;
                }
                
                TipoDato tipoBase = obtenerTipoMatrizDesdeAST(matrizDecl);
                ResultadoExprEmbebida resAcceso = generarAccesoMatrizARM64(gen, valorNodo, &(int){1});
                
                if (resAcceso.resultado[0] == '\0') {
                    printf("[ERROR] No se pudo generar el acceso a matriz para impresión\n");
                    return;
                }
                
                char formatoMat[64];
                snprintf(formatoMat, sizeof(formatoMat), "format_mat_elem_%d", gen->admin_registros->contador_constantes++);
                
                switch (tipoBase) {
                    case TIPO_INT:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoMat);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoMat);
                        break;
                    case TIPO_FLOAT:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoMat);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    fcvt d1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoMat);
                        break;
                    case TIPO_DOUBLE:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoMat);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    fmov d1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoMat);
                        break;
                    case TIPO_CHAR:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\\n\"\n", formatoMat);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoMat);
                        break;
                    case TIPO_STRING:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoMat);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov x1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoMat);
                        break;
                    default:
                        snprintf(buffer, sizeof(buffer), "    // Tipo no soportado para impresión de matriz\n");
                }
                
                strcat(codigo, resAcceso.codigo);
                strcat(codigo, buffer);
                insertarEnSeccionText(gen, codigo);
                return;
                
            } else if (numDimensiones == 1) {
                        // **ACCESO A VECTOR**
                printf("[DEBUG IMPRESION] Acceso a vector detectado\n");
                
                NodoAST* vectorDecl = buscarVectorEnAST(nombreArray);
                if (!vectorDecl) {
                    printf("[ERROR] Vector '%s' no encontrado en AST para impresión\n", nombreArray);
                    return;
                }
                TipoDato tipoBase = obtenerTipoVectorDesdeAST(vectorDecl);
                if (tipoBase == TIPO_DESCONOCIDO) {
                    printf("[ERROR] No se pudo deducir el tipo base del vector '%s' para impresión\n", nombreArray);
                    return;
                }
                
                // ✅ GENERAR CÓDIGO DE ACCESO
                int regIndex = 1;
                ResultadoExprEmbebida resAcceso = generarAccesoVectorARM64(gen, valorNodo, &regIndex);
                
                if (resAcceso.resultado[0] == '\0') {
                    printf("[ERROR] No se pudo generar el acceso a vector para impresión\n");
                    return;
                }
                
                // ✅ INSERTAR CÓDIGO DE ACCESO
                strcat(codigo, resAcceso.codigo);
                
                // ✅ CREAR FORMATO Y GENERAR IMPRESIÓN
                char formatoVec[64];
                snprintf(formatoVec, sizeof(formatoVec), "format_vec_elem_%d", gen->admin_registros->contador_constantes++);
                
                switch (tipoBase) {
                    case TIPO_INT:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_CHAR:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_FLOAT:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    fcvt d1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_DOUBLE:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%f\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    fmov d1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_STRING:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov x1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_BOOLEAN:
                    insertarConstantesBooleanas(gen);
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w0, %s\n"
                            "    ldr x1, =temp_str_valueof\n"
                            "    bl valueOf_bool\n"
                            "    mov x1, x0\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    default:
                        snprintf(buffer, sizeof(buffer), "    // Tipo no soportado para impresión de acceso vector\n");
                        strcat(codigo, buffer);
                }
                insertarEnSeccionText(gen, codigo);
                return;

            }
        } else if (valorNodo->tipo == AST_LENGHT) {
            // Imprimir el resultado de .length
            char formatoLen[64];
            snprintf(formatoLen, sizeof(formatoLen), "format_len_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoLen);
            insertarEnSeccionData(gen, buffer);

            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, valorNodo, &(int){1});
            strcat(codigo, resLength.codigo);
            snprintf(buffer, sizeof(buffer),
                "    mov w1, %s\n"
                "    ldr x0, =%s\n"
                "    bl printf\n",
                resLength.resultado, formatoLen);
            strcat(codigo, buffer);
            insertarEnSeccionText(gen, codigo);
            return;
        }
        else if (valorNodo->tipo == AST_INDEXOF) {
            // Imprimir el resultado de .indexOf
            char formatoIdx[64];
            snprintf(formatoIdx, sizeof(formatoIdx), "format_idx_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoIdx);
            insertarEnSeccionData(gen, buffer);

            ResultadoExprEmbebida resIndexOf = generarExprEmbebidaARM64(gen, valorNodo, &(int){1});
            strcat(codigo, resIndexOf.codigo);
            snprintf(buffer, sizeof(buffer),
                "    mov w1, %s\n"
                "    ldr x0, =%s\n"
                "    bl printf\n",
                resIndexOf.resultado, formatoIdx);
            strcat(codigo, buffer);
            insertarEnSeccionText(gen, codigo);
            return;
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

            if (valorNodo->tipo == AST_IDENTIFICADOR) {
                // ✅ ES UNA VARIABLE
                snprintf(buffer, sizeof(buffer),
                    "    ldr x1, =%s\n"
                    "    ldrb w1, [x1]\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",  
                    valorNodo->valor, formatoChar);
            } else if (valorNodo->tipo == AST_CARACTER) {
                // ✅ ES UN LITERAL: 'X' se convierte a código ASCII
                int ascii = (int)valorNodo->valor[0];
                snprintf(buffer, sizeof(buffer),
                    "    mov w1, #%d\n"
                    "    ldr x0, =%s\n"
                    "    bl printf\n",  
                    ascii, formatoChar);
            }
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

            if (valorNodo->hijos[0]->tipo == AST_ACCESO) {
                ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, valorNodo->hijos[0], &(int){1});
                strcat(codigo, resAcceso.codigo);

                TipoDato tipoBase = tipo;

                char formatoVec[64];
                snprintf(formatoVec, sizeof(formatoVec), "format_vec_elem_%d", gen->admin_registros->contador_constantes++);
                switch (tipoBase) {
                    case TIPO_INT:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%d\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_CHAR:
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%c\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w1, %s\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    case TIPO_BOOLEAN:
                        // Si tienes valueOf_bool implementado, puedes usarlo aquí
                        insertarConstantesBooleanas(gen);
                        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%%s\\n\"\n", formatoVec);
                        insertarEnSeccionData(gen, buffer);
                        snprintf(buffer, sizeof(buffer),
                            "    mov w0, %s\n"
                            "    ldr x1, =temp_str_valueof\n"
                            "    bl valueOf_bool\n"
                            "    ldr x1, =temp_str_valueof\n"
                            "    ldr x0, =%s\n"
                            "    bl printf\n",
                            resAcceso.resultado, formatoVec);
                        strcat(codigo, buffer);
                        break;
                    default:
                        snprintf(buffer, sizeof(buffer), "    // Tipo no soportado para String.valueOf(acceso vector)\n");
                        strcat(codigo, buffer);
                }
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
                    "    ldr %s, [x1]\n"
                    "    fmov d1, %s\n"     // ✅ MOVER A d1 PARA printf
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    valorNodo->valor,
                    (tipo == TIPO_DOUBLE ? "d0" : "s0"),
                    (tipo == TIPO_DOUBLE ? "d0" : "d0"),  // Si es float, convertir a double
                    formatoFloat);
                
                // Si es float, necesitas convertir de s0 a d0 primero
                if (tipo == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    ldr s0, [x1]\n"
                        "    fcvt d0, s0\n"  // ✅ Convertir float a double
                        "    fmov d1, d0\n"  // ✅ Mover a d1
                        "    ldr x0, =%s\n"
                        "    bl printf\n",
                        valorNodo->valor, formatoFloat);
                }
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
                
                // ✅ MOVER A d1 PARA printf
                snprintf(buffer, sizeof(buffer),
                    "    fmov d1, d0\n");
                strcat(codigo, buffer);

                // Llamar a printf
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    bl printf\n",
                    formatoFloat);
                strcat(codigo, buffer);
            }
            strcat(codigo, buffer);
        } else if (tipo == TIPO_BOOLEAN) {
            insertarConstantesBooleanas(gen);
            
            // ✅ AGREGAR DECLARACIÓN DE CONSTANTES str_true Y str_false
            if (!strstr(gen->buffer, "str_true:")) {
                insertarEnSeccionData(gen, "str_true: .asciz \"true\"\n");
            }
            if (!strstr(gen->buffer, "str_false:")) {
                insertarEnSeccionData(gen, "str_false: .asciz \"false\"\n");
            }
            
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

            NodoAST* vectorDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, valorNodo->valor);
    
            if (vectorDecl) {
                // Intentar obtener el tamaño desde la dimensión actualizada
                if (vectorDecl->num_hijos >= 4 && vectorDecl->hijos[3]->tipo == AST_1DIMENSION) {
                    NodoAST* dimensionNodo = vectorDecl->hijos[3]->hijos[0];
                    tam = atoi(dimensionNodo->valor);
                    printf("[DEBUG IMPRESION] Tamaño actualizado desde dimensión: %d\n", tam);
                } else if (vectorDecl->num_hijos >= 3) {
                    // Usar la lógica existente para Arrays.add
                    NodoAST* listaElementos = vectorDecl->hijos[2];
                    
                    if (listaElementos->tipo == AST_LISTA_ELEMENTOS && 
                        listaElementos->num_hijos == 1 && 
                        listaElementos->hijos[0]->tipo == AST_ADD) {
                        
                        NodoAST* addNodo = listaElementos->hijos[0];
                        
                        if (addNodo->num_hijos >= 1 && addNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                            const char* vectorOriginal = addNodo->hijos[0]->valor;
                            int tamOriginal = getVectorSize(astGlobal, vectorOriginal);
                            tam = tamOriginal + 1;
                            printf("[DEBUG IMPRESION] Vector '%s' es resultado de Arrays.add(), tamaño: %d\n", 
                                valorNodo->valor, tam);
                        }
                    }
                }
            }
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
        }else {
            // Tipo no soportado
            snprintf(buffer, sizeof(buffer),
                "    // Tipo no soportado para impresión\n");
            strcat(codigo, buffer);
        }
    }

    // Agregar el código generado a la sección .text
    insertarEnSeccionText(gen, codigo);
}

