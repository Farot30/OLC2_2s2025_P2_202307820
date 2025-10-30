#include "Operadores_arm64.h"
#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include "StringBuilder.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <string.h>
#include <stdlib.h>
#include "../Funciones/Funciones_arm64.h"
extern PilaAmbitosRetorno pilaAmbitosRetorno;
extern NodoAST* astGlobal;
void generarCodigoRecursivoDouble(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen);
void generarCodigoRecursivoFloat(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen);
void generarCodigoRecursivoInt(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen);
void generarCodigoRecursivoChar(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen);
void generarCodigoRecursivoBoolean(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen);

TipoDato detectarTipoLiteral(const char* valor) {
    // Si contiene punto decimal, es float o double
    if (strchr(valor, '.') != NULL) {
        // Si termina en 'f' o 'F', es float
        int len = strlen(valor);
        if (len > 0 && (valor[len-1] == 'f' || valor[len-1] == 'F')) {
            return TIPO_FLOAT;
        }
        return TIPO_DOUBLE;
    }
    // Si no tiene punto decimal, es entero
    return TIPO_INT;
}
int contadorConstantesRoData = 0; // contador global para nombres únicos

void insertarConstanteDoubleRoData(GeneradorDeclaraciones* gen, const char* nombreBase, double valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .double %f\n", nombreBase, valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteFloatRoData(GeneradorDeclaraciones* gen, const char* nombreBase, float valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .float %f\n", nombreBase, valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteCharRoData(GeneradorDeclaraciones* gen, const char* nombreBase, char valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .byte %d\n", nombreBase, (int)valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteIntRoData(GeneradorDeclaraciones* gen, const char* nombreBase, int valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .word %d\n", nombreBase, valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

bool esOperacionCompleja(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_OPERACION) return false;
    
    // Si algún hijo es también una operación, entonces es compleja
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (nodo->hijos[i] && nodo->hijos[i]->tipo == AST_OPERACION) {
            return true;
        }
    }
    return false;
}


void generarCodigoRecursivoBoolean(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // **OPERADORES DE COMPARACIÓN** - Manejan números, devuelven booleanos
        if (strcmp(nodo->valor, "==") == 0 || strcmp(nodo->valor, "!=") == 0 ||
            strcmp(nodo->valor, "<") == 0 || strcmp(nodo->valor, "<=") == 0 ||
            strcmp(nodo->valor, ">") == 0 || strcmp(nodo->valor, ">=") == 0) {
            
            int regIzq = *regIndex;
            int regDer = *regIndex + 1;
            *regIndex += 2;

            // Cargar operando izquierdo
            if (izq->tipo == AST_LENGHT) {
                printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando izquierdo\n");
                ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, izq, regIndex);
                strcat(codigoOperacion, resLength.codigo);
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resLength.resultado);
                strcat(codigoOperacion, buffer);
                (*regIndex)++;
            } 
            else if (izq->tipo == AST_ACCESO) {
                printf("[DEBUG RECURSIVO BOOL] Procesando acceso en comparación izquierda\n");
                ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, izq, regIndex);
                strcat(codigoOperacion, resAcceso.codigo);
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resAcceso.resultado);
                strcat(codigoOperacion, buffer);
            }
            else if (izq->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regIzq, atoi(izq->valor));
                strcat(codigoOperacion, buffer);
            } else if (izq->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
                strcat(codigoOperacion, buffer);
            } else if (izq->tipo == AST_OPERACION) {
                generarCodigoRecursivoBoolean(izq, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
                strcat(codigoOperacion, buffer);
            }

            // Cargar operando derecho
            if (der->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando derecho\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
            }
            else if (der->tipo == AST_ACCESO) {
                printf("[DEBUG RECURSIVO BOOL] Procesando acceso en comparación derecha\n");
                ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, der, regIndex);
                strcat(codigoOperacion, resAcceso.codigo);
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resAcceso.resultado);
                strcat(codigoOperacion, buffer);
            }
            else if (der->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, atoi(der->valor));
                strcat(codigoOperacion, buffer);
            } else if (der->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n", regDer, der->valor, regDer, regDer);
                strcat(codigoOperacion, buffer);
            } else if (der->tipo == AST_OPERACION) {
                generarCodigoRecursivoBoolean(der, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
                strcat(codigoOperacion, buffer);
            }

            // Generar comparación
            if (strcmp(nodo->valor, "==") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, eq\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, "!=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, ne\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, "<") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, lt\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, "<=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, le\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, ">") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, gt\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, ">=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp w%d, w%d\n"
                    "    cset w0, ge\n", regIzq, regDer);
            }
            strcat(codigoOperacion, buffer);
        }
        // **OPERADORES LÓGICOS** - Manejan booleanos, devuelven booleanos
        else if (strcmp(nodo->valor, "&&") == 0 || strcmp(nodo->valor, "||") == 0) {
            int regIzq = *regIndex;
            int regDer = *regIndex + 1;
            *regIndex += 2;

            // Evaluar operando izquierdo (debe ser booleano)
            generarCodigoRecursivoBoolean(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            strcat(codigoOperacion, buffer);

            // Evaluar operando derecho (debe ser booleano)
            generarCodigoRecursivoBoolean(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            strcat(codigoOperacion, buffer);

            // Generar operación lógica
            if (strcmp(nodo->valor, "&&") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    and w0, w%d, w%d\n"
                    "    cmp w0, #0\n"
                    "    cset w0, ne\n", regIzq, regDer);
            } else if (strcmp(nodo->valor, "||") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    orr w0, w%d, w%d\n"
                    "    cmp w0, #0\n"
                    "    cset w0, ne\n", regIzq, regDer);
            }
            strcat(codigoOperacion, buffer);
        }
    } 
    // **NODOS HOJA**
    else if (nodo->tipo == AST_LENGHT) {
        printf("[DEBUG RECURSIVO INT] Procesando Array.length en nodo hoja\n");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resLength.codigo);
        snprintf(buffer, sizeof(buffer), "    mov w0, %s\n", resLength.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_ACCESO) {
        printf("[DEBUG RECURSIVO BOOL] Procesando acceso en nodo hoja\n");
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    mov w0, %s\n", resAcceso.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer),
            "    mov w0, #%d\n", atoi(nodo->valor));
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_BOOLEAN) {
        int valor = (strcmp(nodo->valor, "true") == 0) ? 1 : 0;
        snprintf(buffer, sizeof(buffer),
            "    mov w0, #%d\n", valor);
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldrb w0, [x1]\n", nodo->valor);
        strcat(codigoOperacion, buffer);
    }
}

void generarOperacionGeneral(const char* operador, const char* regResultado, const char* regIzq, const char* regDer, TipoDato tipo, char* codigoOperacion) {
    char buffer[512];
    
    if (tipo == TIPO_INT || tipo == TIPO_CHAR) {
        // **OPERADORES ARITMÉTICOS**
        if (strcmp(operador, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "%") == 0) {
            // Cálculo del módulo: a % b = a - (a / b) * b
            snprintf(buffer, sizeof(buffer),
                "    sdiv %s, %s, %s\n"   // regResultado = regIzq / regDer
                "    mul x9, %s, %s\n"     // x9 = (regIzq / regDer) * regDer
                "    sub %s, %s, x9\n",    // regResultado = regIzq - x9
                regResultado, regIzq, regDer,
                regResultado, regDer,
                regResultado, regIzq);
        }
        // **OPERADORES DE COMPARACIÓN**
        else if (strcmp(operador, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, eq\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, ne\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, lt\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, le\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, gt\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, ge\n", regIzq, regDer, regResultado);
        }
        // **OPERADORES LÓGICOS**
        else if (strcmp(operador, "&&") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    and %s, %s, %s\n"
                "    cmp %s, #0\n"
                "    cset %s, ne\n", regResultado, regIzq, regDer, regResultado, regResultado);
        } else if (strcmp(operador, "||") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    orr %s, %s, %s\n"
                "    cmp %s, #0\n"
                "    cset %s, ne\n", regResultado, regIzq, regDer, regResultado, regResultado);
        }
        // **OPERADORES BIT A BIT**
        else if (strcmp(operador, "&") == 0) {
            snprintf(buffer, sizeof(buffer), "    and %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "|") == 0) {
            snprintf(buffer, sizeof(buffer), "    orr %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "^") == 0) {
            snprintf(buffer, sizeof(buffer), "    eor %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, "<<") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsl %s, %s, %s\n", regResultado, regIzq, regDer);
        } else if (strcmp(operador, ">>") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsr %s, %s, %s\n", regResultado, regIzq, regDer);
        }
    } else if (tipo == TIPO_FLOAT || tipo == TIPO_DOUBLE) {
        const char* sufijo = (tipo == TIPO_FLOAT) ? "s" : "d";
        
        // **OPERADORES ARITMÉTICOS**
        if (strcmp(operador, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    fadd %s0, %s0, %s1\n", sufijo, sufijo, sufijo);
        } else if (strcmp(operador, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    fsub %s0, %s0, %s1\n", sufijo, sufijo, sufijo);
        } else if (strcmp(operador, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    fmul %s0, %s0, %s1\n", sufijo, sufijo, sufijo);
        } else if (strcmp(operador, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    fdiv %s0, %s0, %s1\n", sufijo, sufijo, sufijo);
        } else if (strcmp(operador, "%") == 0) {
            // No hay operador de módulo para float/double en ARM64
            // Se puede implementar como: a - b * trunc(a / b)
            snprintf(buffer, sizeof(buffer),
                "    fdiv %s0, %s0, %s1\n"      // temp = a / b
                "    frintz %s2, %s0\n"         // temp2 = trunc(temp)
                "    fmul %s2, %s2, %s1\n"      // temp2 = temp2 * b
                "    fsub %s0, %s0, %s2\n",     // result = a - temp2
                sufijo, sufijo, sufijo,
                sufijo, sufijo,
                sufijo, sufijo, sufijo,
                sufijo, sufijo);
        }
        // **OPERADORES DE COMPARACIÓN** (para float/double)
        else if (strcmp(operador, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, eq\n", sufijo, sufijo);
        } else if (strcmp(operador, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, ne\n", sufijo, sufijo);
        } else if (strcmp(operador, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, lt\n", sufijo, sufijo);
        } else if (strcmp(operador, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, le\n", sufijo, sufijo);
        } else if (strcmp(operador, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, gt\n", sufijo, sufijo);
        } else if (strcmp(operador, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp %s0, %s1\n"
                "    cset w0, ge\n", sufijo, sufijo);
        }
    } else if (tipo == TIPO_BOOLEAN) {
        // **OPERADORES LÓGICOS**
        if (strcmp(operador, "&&") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    and %s, %s, %s\n"
                "    cmp %s, #0\n"
                "    cset %s, ne\n", regResultado, regIzq, regDer, regResultado, regResultado);
        } else if (strcmp(operador, "||") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    orr %s, %s, %s\n"
                "    cmp %s, #0\n"
                "    cset %s, ne\n", regResultado, regIzq, regDer, regResultado, regResultado);
        }
        // **OPERADORES DE COMPARACIÓN**
        else if (strcmp(operador, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, eq\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, ne\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, lt\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, le\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, gt\n", regIzq, regDer, regResultado);
        } else if (strcmp(operador, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp %s, %s\n"
                "    cset %s, ge\n", regIzq, regDer, regResultado);
        }
    }
    
    strcat(codigoOperacion, buffer);
}
void cargarOperandoGeneral(NodoAST* nodo, const char* registro, TipoDato tipoDestino, GeneradorDeclaraciones* gen, char* codigoOperacion) {
    char buffer[256];
    
    if (nodo->tipo == AST_NUMERO) {
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);
        
        if (tipoDestino == TIPO_INT || tipoDestino == TIPO_CHAR) {
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer), "    mov %s, #%d\n", registro, atoi(nodo->valor));
            } else if (tipoLiteral == TIPO_FLOAT || tipoLiteral == TIPO_DOUBLE) {
                // Convertir float/double a int
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr d2, [x2]\n"
                    "    fcvtzs %s, d2\n", nombreConst, registro);
            }
        } else if (tipoDestino == TIPO_FLOAT) {
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w2, #%d\n"
                    "    scvtf %s, w2\n", atoi(nodo->valor), registro);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr %s, [x2]\n", nombreConst, registro);
            } else if (tipoLiteral == TIPO_DOUBLE) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr d2, [x2]\n"
                    "    fcvt %s, d2\n", nombreConst, registro);
            }
        } else if (tipoDestino == TIPO_DOUBLE) {
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w2, #%d\n"
                    "    scvtf %s, w2\n", atoi(nodo->valor), registro);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr s2, [x2]\n"
                    "    fcvt %s, s2\n", nombreConst, registro);
            } else if (tipoLiteral == TIPO_DOUBLE) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr %s, [x2]\n", nombreConst, registro);
            }
        } else if (tipoDestino == TIPO_BOOLEAN) {
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer), "    mov %s, #%d\n", registro, atoi(nodo->valor) != 0);
            } else if (tipoLiteral == TIPO_FLOAT || tipoLiteral == TIPO_DOUBLE) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr d2, [x2]\n"
                    "    fcvtzs w2, d2\n"
                    "    cmp w2, #0\n"
                    "    cset %s, ne\n", nombreConst, registro);
            }
        }
    } else if (nodo->tipo == AST_BOOLEAN) {  // Agregar este caso
        if (tipoDestino == TIPO_BOOLEAN) {
            int valor = (strcmp(nodo->valor, "true") == 0) ? 1 : 0;
            snprintf(buffer, sizeof(buffer), "    mov %s, #%d\n", registro, valor);
        }
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        TipoDato tipoVar = deducirTipoExprARM64(nodo, astGlobal);
        if (tipoDestino == TIPO_DOUBLE) {
            if (tipoVar == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr w2, [x2]\n"
                    "    scvtf %s, w2\n", nodo->valor, registro);
            } else if (tipoVar == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr %s, [x2]\n", nodo->valor, registro);
            } else if (tipoVar == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr s2, [x2]\n"
                    "    fcvt %s, s2\n", nodo->valor, registro);
            }
        } else if (tipoDestino == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x2, =%s\n"
                "    ldrb %s, [x2]\n", nodo->valor, registro);  // ldrb para boolean
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x2, =%s\n"
                "    ldr %s, [x2]\n", nodo->valor, registro);
        }
    } else if (nodo->tipo == AST_ACCESO) {
        int regIndexTmp = 1;
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, &regIndexTmp);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    mov %s, %s\n", registro, resAcceso.resultado);
        strcat(codigoOperacion, buffer);
        return;
    } else if (nodo->tipo == AST_LENGHT) {
        int regIndexTmp = 1;
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, &regIndexTmp);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    mov %s, %s\n", registro, resAcceso.resultado);
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_CAST) {
        // El nodo AST_CAST tiene:
        // - nodo->valor: tipo destino del cast (ej: "double", "int")
        // - nodo->hijos[0]: la expresión a castear (puede ser AST_LENGHT, AST_ACCESO, etc.)
        
        TipoDato tipoCast = obtenerTipoDesdeString(nodo->valor);
        NodoAST* expr = nodo->num_hijos >= 1 ? nodo->hijos[0] : NULL;
        
        if (!expr) {
            printf("[ERROR CAST] Cast sin expresión\n");
            strcat(codigoOperacion, "    // ERROR: Cast sin expresión\n");
            return;
        }
        
        TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);
        
        printf("[DEBUG CAST] cargarOperandoGeneral: Evaluando cast de tipo %d (%s) a %d (%s)\n",
            tipoExpr, obtenerNombreTipo(tipoExpr), tipoCast, obtenerNombreTipo(tipoCast));
        
        // 1. EVALUAR LA EXPRESIÓN INTERNA (puede ser AST_LENGHT, AST_ACCESO, etc.)
        int regIndexTmp = 2;
        ResultadoExprEmbebida resExpr = generarExprEmbebidaARM64(gen, expr, &regIndexTmp);
        
        // Copiar el código generado
        strcat(codigoOperacion, resExpr.codigo);
        
        printf("[DEBUG CAST] Expresión evaluada: resultado=%s, tipo=%d\n", 
            resExpr.resultado, resExpr.tipo);
        
        // 2. APLICAR LA CONVERSIÓN DE TIPO
        // El resultado viene en resExpr.resultado (puede ser wN, sN, dN, xN)
        
        if (tipoDestino == TIPO_DOUBLE) {
            // Conversión a double
            if (resExpr.tipo == TIPO_INT || tipoExpr == TIPO_INT) {
                // int -> double: scvtf dN, wM
                snprintf(buffer, sizeof(buffer), "    scvtf %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_FLOAT || tipoExpr == TIPO_FLOAT) {
                // float -> double: fcvt dN, sM
                snprintf(buffer, sizeof(buffer), "    fcvt %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_DOUBLE || tipoExpr == TIPO_DOUBLE) {
                // double -> double: fmov dN, dM
                snprintf(buffer, sizeof(buffer), "    fmov %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_CHAR || tipoExpr == TIPO_CHAR) {
                // char -> double: scvtf dN, wM
                snprintf(buffer, sizeof(buffer), "    scvtf %s, %s\n", registro, resExpr.resultado);
            }
        } else if (tipoDestino == TIPO_FLOAT) {
            // Conversión a float
            if (resExpr.tipo == TIPO_INT || tipoExpr == TIPO_INT) {
                snprintf(buffer, sizeof(buffer), "    scvtf %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_FLOAT || tipoExpr == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer), "    fmov %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_DOUBLE || tipoExpr == TIPO_DOUBLE) {
                // double -> float: fcvt sN, dM
                snprintf(buffer, sizeof(buffer), "    fcvt %s, %s\n", registro, resExpr.resultado);
            }
        } else if (tipoDestino == TIPO_INT) {
            // Conversión a int
            if (resExpr.tipo == TIPO_INT || tipoExpr == TIPO_INT) {
                snprintf(buffer, sizeof(buffer), "    mov %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_FLOAT || tipoExpr == TIPO_FLOAT) {
                // float -> int: fcvtzs wN, sM
                snprintf(buffer, sizeof(buffer), "    fcvtzs %s, %s\n", registro, resExpr.resultado);
            } else if (resExpr.tipo == TIPO_DOUBLE || tipoExpr == TIPO_DOUBLE) {
                // double -> int: fcvtzs wN, dM
                snprintf(buffer, sizeof(buffer), "    fcvtzs %s, %s\n", registro, resExpr.resultado);
            }
        }
        
        strcat(codigoOperacion, buffer);
        printf("[DEBUG CAST] Código de conversión generado:\n%s\n", buffer);
        return;
    }
    
    strcat(codigoOperacion, buffer);
}

void generarCodigoRecursivoOperacion(NodoAST* nodo, char* codigoOperacion, int* esPrimera) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // Procesar lado izquierdo
        if (izq->tipo == AST_NUMERO) {
            int valorIzq = atoi(izq->valor);
            snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", valorIzq);
            strcat(codigoOperacion, buffer);
        } else if (izq->tipo == AST_OPERACION) {
            generarCodigoRecursivoOperacion(izq, codigoOperacion, esPrimera);
        }

        // Procesar lado derecho
        if (der->tipo == AST_NUMERO) {
            int valorDer = atoi(der->valor);
            snprintf(buffer, sizeof(buffer), "    mov w2, #%d\n", valorDer);
            strcat(codigoOperacion, buffer);
        } else if (der->tipo == AST_OPERACION) {
            generarCodigoRecursivoOperacion(der, codigoOperacion, esPrimera);
        }

        // Generar la operación
        if (strcmp(nodo->valor, "+") == 0) {
            if (*esPrimera) {
                snprintf(buffer, sizeof(buffer), "    add w0, w1, w2\n");
                *esPrimera = 0;
            } else {
                snprintf(buffer, sizeof(buffer), "    add w0, w0, w2\n");
            }
            strcat(codigoOperacion, buffer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            if (*esPrimera) {
                snprintf(buffer, sizeof(buffer), "    sub w0, w1, w2\n");
                *esPrimera = 0;
            } else {
                snprintf(buffer, sizeof(buffer), "    sub w0, w0, w2\n");
            }
            strcat(codigoOperacion, buffer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            if (*esPrimera) {
                snprintf(buffer, sizeof(buffer), "    mul w0, w1, w2\n");
                *esPrimera = 0;
            } else {
                snprintf(buffer, sizeof(buffer), "    mul w0, w0, w2\n");
            }
            strcat(codigoOperacion, buffer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            if (*esPrimera) {
                snprintf(buffer, sizeof(buffer), "    sdiv w0, w1, w2\n");
                *esPrimera = 0;
            } else {
                snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w2\n");
            }
            strcat(codigoOperacion, buffer);
        } else if(strcmp(nodo->valor, "%") == 0) {
            if (*esPrimera) {
                snprintf(buffer, sizeof(buffer),
                    "    sdiv w3, w1, w2\n"
                    "    mul w3, w3, w2\n"
                    "    sub w0, w1, w3\n");
                *esPrimera = 0;
            } else {
                snprintf(buffer, sizeof(buffer),
                    "    sdiv w3, w0, w2\n"
                    "    mul w3, w3, w2\n"
                    "    sub w0, w0, w3\n");
            }
            strcat(codigoOperacion, buffer);
        }
    }
}

void generarCodigoRecursivoDouble(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // **PASO 1: Evaluar operando izquierdo**
        int regIzq = *regIndex;
        if (izq->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO DOUBLE] Procesando Array.length en operando izquierdo\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, %s\n", regIzq, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }else if (izq->tipo == AST_CAST) {
            printf("[DEBUG RECURSIVO DOUBLE] Procesando cast explícito en operando izquierdo\n");
            
            // CORRECCIÓN: El tipo está en izq->valor, la expresión en izq->hijos[0]
            TipoDato tipoCast = obtenerTipoDesdeString(izq->valor);
            NodoAST* expr = izq->num_hijos >= 1 ? izq->hijos[0] : NULL;
            
            if (!expr) {
                printf("[ERROR] Cast sin expresión en operando izquierdo\n");
                return;
            }
            
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);
            printf("[DEBUG CAST] Cast de tipo %d (%s) a %d (%s)\n", 
                tipoExpr, obtenerNombreTipo(tipoExpr),
                tipoCast, obtenerNombreTipo(tipoCast));
            
            // 1. Evaluar la expresión según su tipo ORIGINAL
            if (tipoExpr == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regIzq);
            } else if (tipoExpr == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regIzq);
            } else if (tipoExpr == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            } else if (tipoExpr == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            }
            strcat(codigoOperacion, buffer);

            // 2. Convertir al tipo DESTINO de la operación actual (double en este caso)
            // El contexto es generarCodigoRecursivoDouble, así que necesitamos double
            if (esWidening(tipoExpr, TIPO_DOUBLE)) {
                if (tipoExpr == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != TIPO_DOUBLE) {
                // Narrowing (el cast ya debería haber convertido al tipo correcto)
                // Solo necesitamos asegurar que esté en double para esta operación
                if (tipoCast == TIPO_FLOAT) {
                    // float -> double
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regIzq, regIzq);
                } else if (tipoCast == TIPO_INT) {
                    // int -> double
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                }
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (izq->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO DOUBLE] Procesando acceso en operando izquierdo\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, %s\n", regIzq, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (izq->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(izq->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf d%d, w%d\n", regIzq, atoi(izq->valor), regIzq, regIzq);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(izq->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regIzq, nombreConst, regIzq, regIzq, regIzq, regIzq);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(izq->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n", regIzq, nombreConst, regIzq, regIzq);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_OPERACION) {
            generarCodigoRecursivoDouble(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_IDENTIFICADOR) {
            TipoDato tipoVar = deducirTipoExprARM64(izq, astGlobal);
            if (tipoVar == TIPO_INT) {
                // Cargar int y convertir a double
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n"
                    "    scvtf d%d, w%d\n", regIzq, izq->valor, regIzq, regIzq, regIzq, regIzq);
            } else if (tipoVar == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
            } else if (tipoVar == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regIzq, izq->valor, regIzq, regIzq, regIzq, regIzq);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 2: Evaluar operando derecho**
        int regDer = *regIndex;
        if (der->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando derecho\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }else if (der->tipo == AST_CAST) {
            printf("[DEBUG RECURSIVO DOUBLE] Procesando cast explícito en operando derecho\n");
            
            TipoDato tipoCast = obtenerTipoDesdeString(der->valor);
            NodoAST* expr = der->num_hijos >= 1 ? der->hijos[0] : NULL;
            
            if (!expr) {
                printf("[ERROR] Cast sin expresión en operando derecho\n");
                return;
            }
            
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);
            
            printf("[DEBUG CAST] Cast de tipo %d a %d (operando derecho)\n", tipoExpr, tipoCast);
            
            // Evaluar la expresión según su tipo original
            if (tipoExpr == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDer);
            } else if (tipoExpr == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regDer);
            } else if (tipoExpr == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            } else if (tipoExpr == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, regIndex, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            }
            strcat(codigoOperacion, buffer);
            
            // Convertir a double para la operación
            if (esWidening(tipoExpr, TIPO_DOUBLE)) {
                if (tipoExpr == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != TIPO_DOUBLE) {
                if (tipoCast == TIPO_FLOAT) {
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regDer, regDer);
                } else if (tipoCast == TIPO_INT) {
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                }
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }

        else if (der->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO DOUBLE] Procesando acceso en operando derecho\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, %s\n", regDer, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (der->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(der->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf d%d, w%d\n", regDer, atoi(der->valor), regDer, regDer);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(der->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regDer, nombreConst, regDer, regDer, regDer, regDer);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(der->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n", regDer, nombreConst, regDer, regDer);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_OPERACION) {
            generarCodigoRecursivoDouble(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_IDENTIFICADOR) {
            TipoDato tipoVar = deducirTipoExprARM64(der, astGlobal);
            if (tipoVar == TIPO_INT) {
                // Cargar int y convertir a double
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n"
                    "    scvtf d%d, w%d\n", regDer, der->valor, regDer, regDer, regDer, regDer);
            } else if (tipoVar == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n", regDer, der->valor, regDer, regDer);
            } else if (tipoVar == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regDer, der->valor, regDer, regDer, regDer, regDer);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 3: Generar la operación entre los dos operandos**
        if (strcmp(nodo->valor, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    fadd d0, d%d, d%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    fsub d0, d%d, d%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    fmul d0, d%d, d%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    fdiv d0, d%d, d%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "%") == 0) {
            // Cálculo del módulo para double: a % b = a - (a / b) * b
            snprintf(buffer, sizeof(buffer),
                "    fdiv d9, d%d, d%d\n"   // d9 = dIzq / dDer
                "    frintz d9, d9\n"        // Redondear hacia cero
                "    fmul d9, d9, d%d\n"     // d9 = (dIzq / dDer) * dDer
                "    fsub d0, d%d, d9\n",    // d0 = dIzq - d9
                regIzq, regDer,
                regDer,
                regIzq);
        
        } else if (strcmp(nodo->valor, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, eq\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, ne\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, lt\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, le\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, gt\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp d%d, d%d\n"
                "    cset w0, ge\n"
                "    scvtf d0, w0\n", regIzq, regDer);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_LENGHT) {
        printf("[DEBUG RECURSIVO DOUBLE] Procesando Array.length en nodo hoja\n");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resLength.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov d0, %s\n", resLength.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_ACCESO) {
        printf("[DEBUG RECURSIVO DOUBLE] Procesando acceso en nodo hoja\n");
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov d0, %s\n", resAcceso.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_NUMERO) {
        // **NODO HOJA: Cargar directamente en d0**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    scvtf d0, w1\n", atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr s1, [x1]\n"
                "    fcvt d0, s1\n", nombreConst);
        } else { // TIPO_DOUBLE
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData);
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr d0, [x1]\n", nombreConst);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr d0, [x1]\n", nodo->valor);
        strcat(codigoOperacion, buffer);
    }
}

void generarCodigoRecursivoFloat(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // **PASO 1: Evaluar operando izquierdo**
        int regIzq = *regIndex;

        if (izq->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO FLOAT] Procesando Array.length en operando izquierdo\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    scvtf s%d, %s\n", regIzq, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_CAST) {
            NodoAST* tipoDestino = izq->hijos[0];
            NodoAST* expr = izq->hijos[1];
            TipoDato tipoCast = obtenerTipoDesdeString(tipoDestino->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);

            // Recursivamente genera el código para la expresión
            int regTmp = regIzq;
            if (tipoCast == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regIzq);
            } else if (tipoCast == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regIzq);
            } else if (tipoCast == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            } else if (tipoCast == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            }
            strcat(codigoOperacion, buffer);

            // Widening/Narrowing
            if (esWidening(tipoExpr, tipoCast)) {
                if (tipoExpr == TIPO_INT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    // Widening char->int ya está en w%d\n", regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != tipoCast) {
                // Narrowing
                if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt s%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    // Narrowing int->char ya está en w%d\n", regIzq);
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (izq->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO FLOAT] Procesando acceso en operando izquierdo\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov s%d, %s\n", regIzq, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (izq->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(izq->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf s%d, w%d\n", regIzq, atoi(izq->valor), regIzq, regIzq);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(izq->valor), contadorConstantesRoData++);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n", regIzq, nombreConst, regIzq, regIzq);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(izq->valor), contadorConstantesRoData);
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n"
                    "    fcvt s%d, d%d\n", regIzq, nombreConst, regIzq, regIzq, regIzq, regIzq);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_OPERACION) {
            generarCodigoRecursivoFloat(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 2: Evaluar operando derecho**
        int regDer = *regIndex;
        if (der->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO FLOAT] Procesando Array.length en operando derecho\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    scvtf s%d, %s\n", regDer, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (der->tipo == AST_CAST) {
            NodoAST* tipoDestino = der->hijos[0];
            NodoAST* expr = der->hijos[1];
            TipoDato tipoCast = obtenerTipoDesdeString(tipoDestino->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);

            int regTmp = regDer;
            // Recursivamente genera el código para la expresión
            if (tipoCast == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDer);
            } else if (tipoCast == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regDer);
            } else if (tipoCast == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            } else if (tipoCast == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            }
            strcat(codigoOperacion, buffer);

            // Widening/Narrowing
            if (esWidening(tipoExpr, tipoCast)) {
                if (tipoExpr == TIPO_INT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    // Widening char->int ya está en w%d\n", regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != tipoCast) {
                // Narrowing
                if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt s%d, d%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, s%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, d%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    // Narrowing int->char ya está en w%d\n", regDer);
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (der->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO FLOAT] Procesando acceso en operando derecho\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    fmov s%d, %s\n", regDer, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (der->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(der->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf s%d, w%d\n", regDer, atoi(der->valor), regDer, regDer);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(der->valor), contadorConstantesRoData++ );
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n", regDer, nombreConst, regDer, regDer);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(der->valor), contadorConstantesRoData++ );
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteDoubleRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr d%d, [x%d]\n"
                    "    fcvt s%d, d%d\n", regDer, nombreConst, regDer, regDer, regDer, regDer);
            }
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_OPERACION) {
            generarCodigoRecursivoFloat(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n", regDer, der->valor, regDer, regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 3: Generar la operación entre los dos operandos**
        if (strcmp(nodo->valor, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    fadd s0, s%d, s%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    fsub s0, s%d, s%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    fmul s0, s%d, s%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    fdiv s0, s%d, s%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, eq\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, ne\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, lt\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, le\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, gt\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    fcmp s%d, s%d\n"
                "    cset w0, ge\n"
                "    scvtf s0, w0\n", regIzq, regDer);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_LENGHT) {
        printf("[DEBUG RECURSIVO DOUBLE] Procesando Array.length en nodo hoja\n");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resLength.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov d0, %s\n", resLength.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_ACCESO) {
        printf("[DEBUG RECURSIVO FLOAT] Procesando acceso en nodo hoja\n");
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov s0, %s\n", resAcceso.resultado);
        strcat(codigoOperacion, buffer);
    } 
    else if (nodo->tipo == AST_NUMERO) {
        // **NODO HOJA: Cargar directamente en s0**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    scvtf s0, w1\n", atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr s0, [x1]\n", nombreConst);
        } else { // TIPO_DOUBLE
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g_%d", atof(nodo->valor), contadorConstantesRoData++);
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr d1, [x1]\n"
                "    fcvt s0, d1\n", nombreConst);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr s0, [x1]\n", nodo->valor);
        strcat(codigoOperacion, buffer);
    }
}

void generarCodigoRecursivoInt(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];


        printf("[DEBUG RECURSIVO INT] Operación '%s' detectada\n", nodo->valor);
        printf("[DEBUG RECURSIVO INT] Operando izquierdo: tipo=%d, valor=%s\n", 
               izq->tipo, izq->valor ? izq->valor : "(null)");
        printf("[DEBUG RECURSIVO INT] Operando derecho: tipo=%d, valor=%s\n", 
               der->tipo, der->valor ? der->valor : "(null)");
        // **PASO 1: Evaluar operando izquierdo**
        int regIzq = *regIndex;
        
        if (izq->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando izquierdo\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_CAST) {
            NodoAST* tipoDestino = izq->hijos[0];
            NodoAST* expr = izq->hijos[1];
            TipoDato tipoCast = obtenerTipoDesdeString(tipoDestino->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);

            // Recursivamente genera el código para la expresión
            int regTmp = regIzq;
            if (tipoCast == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regIzq);
            } else if (tipoCast == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regIzq);
            } else if (tipoCast == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            } else if (tipoCast == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            }
            strcat(codigoOperacion, buffer);

            // Widening/Narrowing
            if (esWidening(tipoExpr, tipoCast)) {
                if (tipoExpr == TIPO_INT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    // Widening char->int ya está en w%d\n", regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != tipoCast) {
                // Narrowing
                if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt s%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    // Narrowing int->char ya está en w%d\n", regIzq);
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (izq->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO INT] Procesando acceso en operando izquierdo\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            printf("[DEBUG RECURSIVO INT] Código de acceso izquierdo generado: %s\n", resAcceso.codigo);
            (*regIndex)++;
        }
        else if (izq->tipo == AST_NUMERO) {
            printf("[DEBUG RECURSIVO INT] Cargando número literal izquierdo: %s\n", izq->valor);
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regIzq, atoi(izq->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_IDENTIFICADOR) {
            printf("[DEBUG RECURSIVO INT] Cargando variable izquierda: %s\n", izq->valor);
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_OPERACION) {
            printf("[DEBUG RECURSIVO INT] Evaluando operación anidada izquierda\n");
            generarCodigoRecursivoInt(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 2: Evaluar operando derecho**
        int regDer = *regIndex;
        
        if (der->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando derecho\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_CAST) {
            printf("[DEBUG RECURSIVO INT] Procesando cast explícito en operando derecho\n");
            NodoAST* tipoCastNodo = der->hijos[0];
            NodoAST* expr = der->num_hijos >= 1 ? der->hijos[0] : NULL;
            
            if (!expr) {
                printf("[ERROR] Cast sin expresión\n");
                return;
            }
            
            // El valor del nodo es el tipo destino (ej: "int", "float")
            TipoDato tipoCast = obtenerTipoDesdeString(der->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);
            
            printf("[DEBUG CAST] Cast de tipo %d a %d\n", tipoExpr, tipoCast);
            
            // Evaluar la expresión recursivamente
            int regTmp = regDer;
            if (expr->tipo == AST_LENGHT) {
                ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, expr, regIndex);
                strcat(codigoOperacion, resLength.codigo);
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resLength.resultado);
                strcat(codigoOperacion, buffer);
            } else if (expr->tipo == AST_ACCESO) {
                ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, expr, regIndex);
                strcat(codigoOperacion, resAcceso.codigo);
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resAcceso.resultado);
                strcat(codigoOperacion, buffer);
            } else if (expr->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr w%d, [x%d]\n", regDer, expr->valor, regDer, regDer);
                strcat(codigoOperacion, buffer);
            } else if (expr->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, atoi(expr->valor));
                strcat(codigoOperacion, buffer);
            }
            
            // No hacemos conversión aquí porque es int a int (narrowing se maneja en el destino)
            (*regIndex)++;
        }
        else if (der->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO INT] Procesando acceso en operando derecho\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            printf("[DEBUG RECURSIVO INT] Código de acceso derecho generado: %s\n", resAcceso.codigo);
            (*regIndex)++;
        }
        else if (der->tipo == AST_NUMERO) {
            printf("[DEBUG RECURSIVO INT] Cargando número literal derecho: %s\n", der->valor);
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, atoi(der->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_IDENTIFICADOR) {
            printf("[DEBUG RECURSIVO INT] Cargando variable derecha: %s\n", der->valor);
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n", regDer, der->valor, regDer, regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_OPERACION) {
            printf("[DEBUG RECURSIVO INT] Evaluando operación anidada derecha\n");
            generarCodigoRecursivoInt(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 3: Generar la operación entre los dos operandos**
        printf("[DEBUG RECURSIVO INT] Generando operación '%s' entre w%d y w%d\n", 
               nodo->valor, regIzq, regDer);
        if (strcmp(nodo->valor, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "&") == 0) {
            snprintf(buffer, sizeof(buffer), "    and w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "|") == 0) {
            snprintf(buffer, sizeof(buffer), "    orr w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "^") == 0) {
            snprintf(buffer, sizeof(buffer), "    eor w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<<") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsl w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">>") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsr w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "%") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    sdiv w1, w%d, w%d\n"
                "    msub w0, w1, w%d, w%d\n", regIzq, regDer, regDer, regIzq);
        } else if (strcmp(nodo->valor, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, eq\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, ne\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, lt\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, le\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, gt\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, ge\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "&&") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    and w0, w%d, w%d\n"
                "    cmp w0, #0\n"
                "    cset w0, ne\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "||") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    orr w0, w%d, w%d\n"
                "    cmp w0, #0\n"
                "    cset w0, ne\n", regIzq, regDer);
        }
        strcat(codigoOperacion, buffer);
        printf("[DEBUG RECURSIVO INT] Código de operación generado: %s\n", buffer);
    } else if (nodo->tipo == AST_LENGHT) {
        printf("[DEBUG RECURSIVO DOUBLE] Procesando Array.length en nodo hoja\n");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resLength.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov d0, %s\n", resLength.resultado);
        strcat(codigoOperacion, buffer);
    } 
    else if (nodo->tipo == AST_ACCESO) {
        printf("[DEBUG RECURSIVO INT] Procesando acceso en nodo hoja\n");
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    mov w0, %s\n", resAcceso.resultado);
        strcat(codigoOperacion, buffer);
        printf("[DEBUG RECURSIVO INT] Código de acceso hoja generado: %s\n", resAcceso.codigo);
    }  
    else if (nodo->tipo == AST_NUMERO) {
        printf("[DEBUG RECURSIVO INT] Nodo hoja: número literal %s\n", nodo->valor);
        // **NODO HOJA: Cargar número directamente en w0**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            
            snprintf(buffer, sizeof(buffer), "    mov w0, #%d\n", atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT || tipoLiteral == TIPO_DOUBLE) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr d1, [x1]\n"
                "    fcvtzs w0, d1\n", nombreConst);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        printf("[DEBUG RECURSIVO INT] Nodo hoja: identificador %s\n", nodo->valor);
        // **NODO HOJA: Cargar variable directamente en w0**
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n", nodo->valor);
        strcat(codigoOperacion, buffer);
    }
}

void generarCodigoRecursivoChar(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        NodoAST* izq = nodo->hijos[0];
        NodoAST* der = nodo->hijos[1];

        // **PASO 1: Evaluar operando izquierdo**
        int regIzq = *regIndex;
        if (izq->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando izquierdo\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_CAST) {
            NodoAST* tipoDestino = izq->hijos[0];
            NodoAST* expr = izq->hijos[1];
            TipoDato tipoCast = obtenerTipoDesdeString(tipoDestino->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);

            // Recursivamente genera el código para la expresión
            int regTmp = regIzq;
            if (tipoCast == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regIzq);
            } else if (tipoCast == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regIzq);
            } else if (tipoCast == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            } else if (tipoCast == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            }
            strcat(codigoOperacion, buffer);

            // Widening/Narrowing
            if (esWidening(tipoExpr, tipoCast)) {
                if (tipoExpr == TIPO_INT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    // Widening char->int ya está en w%d\n", regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regIzq, regIzq);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != tipoCast) {
                // Narrowing
                if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt s%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, s%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, d%d\n", regIzq, regIzq);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    // Narrowing int->char ya está en w%d\n", regIzq);
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (izq->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO CHAR] Procesando acceso en operando izquierdo\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, izq, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regIzq, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (izq->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regIzq, atoi(izq->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_CARACTER) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regIzq, (int)izq->valor[0]);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_OPERACION) {
            generarCodigoRecursivoChar(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldrb w%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 2: Evaluar operando derecho**
        int regDer = *regIndex;

        if (der->tipo == AST_LENGHT) {
            printf("[DEBUG RECURSIVO INT] Procesando Array.length en operando derecho\n");
            ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resLength.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resLength.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }else if (der->tipo == AST_CAST) {
            NodoAST* tipoDestino = der->hijos[0];
            NodoAST* expr = der->hijos[1];
            TipoDato tipoCast = obtenerTipoDesdeString(tipoDestino->valor);
            TipoDato tipoExpr = deducirTipoExprARM64(expr, astGlobal);

            int regTmp = regDer;
            // Recursivamente genera el código para la expresión
            if (tipoCast == TIPO_DOUBLE) {
                generarCodigoRecursivoDouble(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", regDer);
            } else if (tipoCast == TIPO_FLOAT) {
                generarCodigoRecursivoFloat(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", regDer);
            } else if (tipoCast == TIPO_INT) {
                generarCodigoRecursivoInt(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            } else if (tipoCast == TIPO_CHAR) {
                generarCodigoRecursivoChar(expr, codigoOperacion, &regTmp, gen);
                snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            }
            strcat(codigoOperacion, buffer);

            // Widening/Narrowing
            if (esWidening(tipoExpr, tipoCast)) {
                if (tipoExpr == TIPO_INT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    fcvt d%d, s%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    // Widening char->int ya está en w%d\n", regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    scvtf s%d, w%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_CHAR && tipoCast == TIPO_DOUBLE)
                    snprintf(buffer, sizeof(buffer), "    scvtf d%d, w%d\n", regDer, regDer);
                strcat(codigoOperacion, buffer);
            } else if (tipoExpr != tipoCast) {
                // Narrowing
                if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_FLOAT)
                    snprintf(buffer, sizeof(buffer), "    fcvt s%d, d%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_FLOAT && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, s%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_DOUBLE && tipoCast == TIPO_INT)
                    snprintf(buffer, sizeof(buffer), "    fcvtzs w%d, d%d\n", regDer, regDer);
                else if (tipoExpr == TIPO_INT && tipoCast == TIPO_CHAR)
                    snprintf(buffer, sizeof(buffer), "    // Narrowing int->char ya está en w%d\n", regDer);
                strcat(codigoOperacion, buffer);
            }
            (*regIndex)++;
        }
        else if (der->tipo == AST_ACCESO) {
            printf("[DEBUG RECURSIVO CHAR] Procesando acceso en operando derecho\n");
            ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, der, regIndex);
            strcat(codigoOperacion, resAcceso.codigo);
            snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", regDer, resAcceso.resultado);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }
        else if (der->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, atoi(der->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_CARACTER) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, (int)der->valor[0]);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_OPERACION) {
            generarCodigoRecursivoChar(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldrb w%d, [x%d]\n", regDer, der->valor, regDer, regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 3: Generar la operación entre los dos operandos**
        if (strcmp(nodo->valor, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w%d, w%d\n", regIzq, regDer);
        } else if(strcmp(nodo->valor, "%") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    sdiv w1, w%d, w%d\n"
                "    msub w0, w1, w%d, w%d\n", regIzq, regDer, regDer, regIzq);
        } else if (strcmp(nodo->valor, "&") == 0) {
            snprintf(buffer, sizeof(buffer), "    and w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "|") == 0) {
            snprintf(buffer, sizeof(buffer), "    orr w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "^") == 0) {
            snprintf(buffer, sizeof(buffer), "    eor w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<<") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsl w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">>") == 0) {
            snprintf(buffer, sizeof(buffer), "    lsr w0, w%d, w%d\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "==") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, eq\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "!=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, ne\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, lt\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, "<=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, le\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, gt\n", regIzq, regDer);
        } else if (strcmp(nodo->valor, ">=") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w%d, w%d\n"
                "    cset w0, ge\n", regIzq, regDer);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_LENGHT) {
        printf("[DEBUG RECURSIVO DOUBLE] Procesando Array.length en nodo hoja\n");
        ResultadoExprEmbebida resLength = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resLength.codigo);
        snprintf(buffer, sizeof(buffer), "    fmov d0, %s\n", resLength.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_ACCESO) {
        printf("[DEBUG RECURSIVO CHAR] Procesando acceso en nodo hoja\n");
        ResultadoExprEmbebida resAcceso = generarExprEmbebidaARM64(gen, nodo, regIndex);
        strcat(codigoOperacion, resAcceso.codigo);
        snprintf(buffer, sizeof(buffer), "    mov w0, %s\n", resAcceso.resultado);
        strcat(codigoOperacion, buffer);
    }
    else if (nodo->tipo == AST_NUMERO) {
        // **NODO HOJA: Cargar directamente en w0**
        snprintf(buffer, sizeof(buffer), "    mov w0, #%d\n", atoi(nodo->valor));
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_CARACTER) {
        // **NODO HOJA: Cargar carácter directamente en w0**
        snprintf(buffer, sizeof(buffer), "    mov w0, #%d\n", (int)nodo->valor[0]);
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldrb w0, [x1]\n", nodo->valor);
        strcat(codigoOperacion, buffer);
    }
}

void generarCodigoOperacionARM64(GeneradorDeclaraciones* gen, const char* nombreVar, NodoAST* operacionNodo, TipoDato tipoVar) {
    if (!gen || !nombreVar || !operacionNodo || operacionNodo->tipo != AST_OPERACION) return;

    // **CASO ESPECIAL: CONCATENACIÓN DE STRINGS**
    if (tipoVar == TIPO_STRING && strcmp(operacionNodo->valor, "+") == 0) {
        if (!esConcatenacionSimple(operacionNodo)) {
            printf("[DEBUG] Detectada concatenación COMPLEJA de strings para variable '%s'\n", nombreVar);
            generarConcatenacionStringBuilder(gen, nombreVar, operacionNodo);
            return;
        }
        printf("[DEBUG] Detectada concatenación SIMPLE de strings para variable '%s'\n", nombreVar);
    }

    // **RESTO DE OPERACIONES**
    char codigoOperacion[2048] = "";
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "    // Operación: %s = <expresión>\n", nombreVar);
    strcat(codigoOperacion, buffer);

    printf("[DEBUG OP] Generando operación para '%s', tipo=%d, operador='%s'\n",
        nombreVar, tipoVar, operacionNodo->valor ? operacionNodo->valor : "(null)");

    // ✅ NUEVO: SI ESTAMOS EN UNA FUNCIÓN Y ES OPERACIÓN SIMPLE ENTRE PARÁMETROS, OPTIMIZAR
    if (gen->dentro_de_funcion && gen->nombre_funcion_actual[0] != '\0') {
        extern PilaAmbitosRetorno pilaAmbitosRetorno;
        AmbitoFuncionRetorno* ambito = getAmbitoRetornoActual(&pilaAmbitosRetorno);
        
        if (ambito && operacionNodo->num_hijos == 2) {
            NodoAST* izq = operacionNodo->hijos[0];
            NodoAST* der = operacionNodo->hijos[1];
            printf("[DEPURACIÓN OP] Operando izquierdo: tipo=%d, valor=%s\n", izq->tipo, izq->valor);
        printf("[DEPURACIÓN OP] Operando derecho: tipo=%d, valor=%s\n", der->tipo, der->valor);
            
            // ✅ VERIFICAR SI AMBOS SON IDENTIFICADORES (pueden ser parámetros o variables)
            if (izq->tipo == AST_IDENTIFICADOR && der->tipo == AST_IDENTIFICADOR) {
                int offsetIzq = -1, offsetDer = -1;
                bool izqEsParam = false, derEsParam = false;
                
                // ✅ BUSCAR AMBOS OPERANDOS EN PARÁMETROS
                for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
                    if (strcmp(ambito->ambito_base.parametros[i].nombre, izq->valor) == 0) {
                        offsetIzq = i * 8;
                        izqEsParam = true;
                        printf("[DEBUG OP] Operando izquierdo '%s' es parámetro en offset %d\n", 
                               izq->valor, offsetIzq);
                    }
                    if (strcmp(ambito->ambito_base.parametros[i].nombre, der->valor) == 0) {
                        offsetDer = i * 8;
                        derEsParam = true;
                        printf("[DEBUG OP] Operando derecho '%s' es parámetro en offset %d\n", 
                               der->valor, offsetDer);
                    }
                }
                
                // ✅ SI AMBOS SON PARÁMETROS, GENERAR CÓDIGO OPTIMIZADO
                if (izqEsParam && derEsParam) {
                    printf("[DEBUG OP] GENERANDO CÓDIGO PARA PARÁMETROS: %s %s %s\n", 
                           izq->valor, operacionNodo->valor, der->valor);
                    
                    // ✅ DETERMINAR NOMBRE REAL DE LA VARIABLE (puede ser local)
                    char nombreReal[128];
                    bool esVarLocal = false;
                    for (int i = 0; i < ambito->ambito_base.numVariables; i++) {
                        if (strcmp(ambito->ambito_base.variables[i].nombre, nombreVar) == 0) {
                            snprintf(nombreReal, sizeof(nombreReal), "%s_%s", 
                                    gen->nombre_funcion_actual, nombreVar);
                            esVarLocal = true;
                            printf("[DEBUG OP] Variable '%s' es local, usando nombre: %s\n", 
                                   nombreVar, nombreReal);
                            break;
                        }
                    }
                    if (!esVarLocal) {
                        strncpy(nombreReal, nombreVar, sizeof(nombreReal) - 1);
                        printf("[DEBUG OP] Variable '%s' es global\n", nombreVar);
                    }
                    
                    // Generar código optimizado para parámetros
                    if (strcmp(operacionNodo->valor, "+") == 0) {
                        snprintf(buffer, sizeof(buffer),
                            "    ldr w1, [sp, #%d]  // cargar '%s'\n"
                            "    ldr w2, [sp, #%d]  // cargar '%s'\n"
                            "    add w0, w1, w2\n"
                            "    ldr x1, =%s\n"
                            "    str w0, [x1]\n",
                            offsetIzq, izq->valor, offsetDer, der->valor, nombreReal);
                    } else if (strcmp(operacionNodo->valor, "-") == 0) {
                        snprintf(buffer, sizeof(buffer),
                            "    ldr w1, [sp, #%d]  // cargar '%s'\n"
                            "    ldr w2, [sp, #%d]  // cargar '%s'\n"
                            "    sub w0, w1, w2\n"
                            "    ldr x1, =%s\n"
                            "    str w0, [x1]\n",
                            offsetIzq, izq->valor, offsetDer, der->valor, nombreReal);
                    } else if (strcmp(operacionNodo->valor, "*") == 0) {
                        snprintf(buffer, sizeof(buffer),
                            "    ldr w1, [sp, #%d]  // cargar '%s'\n"
                            "    ldr w2, [sp, #%d]  // cargar '%s'\n"
                            "    mul w0, w1, w2\n"
                            "    ldr x1, =%s\n"
                            "    str w0, [x1]\n",
                            offsetIzq, izq->valor, offsetDer, der->valor, nombreReal);
                    } else if (strcmp(operacionNodo->valor, "/") == 0) {
                        snprintf(buffer, sizeof(buffer),
                            "    ldr w1, [sp, #%d]  // cargar '%s'\n"
                            "    ldr w2, [sp, #%d]  // cargar '%s'\n"
                            "    sdiv w0, w1, w2\n"
                            "    ldr x1, =%s\n"
                            "    str w0, [x1]\n",
                            offsetIzq, izq->valor, offsetDer, der->valor, nombreReal);
                    } else {
                        printf("[WARNING] Operador '%s' no soportado en optimización de parámetros\n", 
                               operacionNodo->valor);
                        goto usar_recursivo; // Saltar a código recursivo
                    }
                    
                    strcat(codigoOperacion, buffer);
                    insertarEnSeccionText(gen, codigoOperacion);
                    printf("[DEBUG OP] Código optimizado generado:\n%s\n", codigoOperacion);
                    return; // ✅ SALIR AQUÍ
                }
            }
        }
    }

usar_recursivo:
    printf("[DEBUG OP] Usando generación recursiva para '%s'\n", nombreVar);
    
    // **DETECTAR SI ES OPERACIÓN COMPUESTA**
    bool esOperacionCompuesta = esOperacionCompleja(operacionNodo);
    printf("[DEBUG OP] esOperacionCompuesta=%d\n", esOperacionCompuesta);

    if (esOperacionCompuesta) {
        // **USAR FUNCIONES RECURSIVAS**
        int regIndex = 1;
        if (tipoVar == TIPO_DOUBLE) {
            generarCodigoRecursivoDouble(operacionNodo, codigoOperacion, &regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    str d0, [x1]\n", nombreVar);
        } else if (tipoVar == TIPO_FLOAT) {
            generarCodigoRecursivoFloat(operacionNodo, codigoOperacion, &regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    str s0, [x1]\n", nombreVar);
        } else if (tipoVar == TIPO_INT || tipoVar == TIPO_CHAR) {
            generarCodigoRecursivoInt(operacionNodo, codigoOperacion, &regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    str w0, [x1]\n", nombreVar);
        } else if (tipoVar == TIPO_BOOLEAN) {
            generarCodigoRecursivoBoolean(operacionNodo, codigoOperacion, &regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    strb w0, [x1]\n", nombreVar);
        }
        strcat(codigoOperacion, buffer);
    } else {
        // **OPERACIÓN BINARIA SIMPLE**
        NodoAST* izq = operacionNodo->hijos[0];
        NodoAST* der = operacionNodo->hijos[1];

        const char* regIzq = (tipoVar == TIPO_FLOAT) ? "s0" : (tipoVar == TIPO_DOUBLE) ? "d0" : "w0";
        const char* regDer = (tipoVar == TIPO_FLOAT) ? "s1" : (tipoVar == TIPO_DOUBLE) ? "d1" : "w1";
        const char* regRes = (tipoVar == TIPO_FLOAT) ? "s0" : (tipoVar == TIPO_DOUBLE) ? "d0" : "w0";

        cargarOperandoGeneral(izq, regIzq, tipoVar, gen, codigoOperacion);
        cargarOperandoGeneral(der, regDer, tipoVar, gen, codigoOperacion);
        generarOperacionGeneral(operacionNodo->valor, regRes, regIzq, regDer, tipoVar, codigoOperacion);

        const char* instruccionStore = (tipoVar == TIPO_FLOAT) ? "str s0" : 
                                    (tipoVar == TIPO_DOUBLE) ? "str d0" : 
                                    (tipoVar == TIPO_CHAR) ? "strb w0" : 
                                    (tipoVar == TIPO_BOOLEAN) ? "strb w0" : "str w0";
        snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    %s, [x1]\n", nombreVar, instruccionStore);
        strcat(codigoOperacion, buffer);
    }

    printf("[DEBUG OP] Código generado para '%s':\n%s\n", nombreVar, codigoOperacion);
    insertarEnSeccionText(gen, codigoOperacion);
}