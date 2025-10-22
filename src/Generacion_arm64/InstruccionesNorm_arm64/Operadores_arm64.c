#include "Operadores_arm64.h"
#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include "StringBuilder.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

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
void insertarConstanteDoubleRoData(GeneradorDeclaraciones* gen, const char* nombre, double valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .double %f\n", nombre, valor);
    // Usa la función de insertar en .rodata de tu generador de constantes
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteFloatRoData(GeneradorDeclaraciones* gen, const char* nombre, float valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .float %f\n", nombre, valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteCharRoData(GeneradorDeclaraciones* gen, const char* nombre, char valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .byte %d\n", nombre, (int)valor);
    insertarEnSeccionRoData((GeneradorConstantesARM64*)gen, buffer);
}

void insertarConstanteIntRoData(GeneradorDeclaraciones* gen, const char* nombre, int valor) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s: .word %d\n", nombre, valor);
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
            if (izq->tipo == AST_NUMERO) {
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
            if (der->tipo == AST_NUMERO) {
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
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr %s, [x2]\n", nombreConst, registro);
            } else if (tipoLiteral == TIPO_DOUBLE) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr s2, [x2]\n"
                    "    fcvt %s, s2\n", nombreConst, registro);
            } else if (tipoLiteral == TIPO_DOUBLE) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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
        if (izq->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(izq->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf d%d, w%d\n", regIzq, atoi(izq->valor), regIzq, regIzq);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(izq->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regIzq, nombreConst, regIzq, regIzq, regIzq, regIzq);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(izq->valor));
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
        if (der->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(der->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf d%d, w%d\n", regDer, atoi(der->valor), regDer, regDer);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(der->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n"
                    "    fcvt d%d, s%d\n", regDer, nombreConst, regDer, regDer, regDer, regDer);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(der->valor));
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
    } else if (nodo->tipo == AST_NUMERO) {
        // **NODO HOJA: Cargar directamente en d0**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    scvtf d0, w1\n", atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr s1, [x1]\n"
                "    fcvt d0, s1\n", nombreConst);
        } else { // TIPO_DOUBLE
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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
        if (izq->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(izq->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf s%d, w%d\n", regIzq, atoi(izq->valor), regIzq, regIzq);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(izq->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(izq->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n", regIzq, nombreConst, regIzq, regIzq);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(izq->valor));
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
        if (der->tipo == AST_NUMERO) {
            TipoDato tipoLiteral = detectarTipoLiteral(der->valor);
            if (tipoLiteral == TIPO_INT) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%d\n"
                    "    scvtf s%d, w%d\n", regDer, atoi(der->valor), regDer, regDer);
            } else if (tipoLiteral == TIPO_FLOAT) {
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(der->valor));
                for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
                insertarConstanteFloatRoData(gen, nombreConst, atof(der->valor));
                snprintf(buffer, sizeof(buffer),
                    "    ldr x%d, =%s\n"
                    "    ldr s%d, [x%d]\n", regDer, nombreConst, regDer, regDer);
            } else { // TIPO_DOUBLE
                char nombreConst[32];
                snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(der->valor));
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
    } else if (nodo->tipo == AST_NUMERO) {
        // **NODO HOJA: Cargar directamente en s0**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    scvtf s0, w1\n", atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    ldr s0, [x1]\n", nombreConst);
        } else { // TIPO_DOUBLE
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
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

        // **PASO 1: Evaluar operando izquierdo**
        int regIzq = *regIndex;
        if (izq->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regIzq, atoi(izq->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_IDENTIFICADOR) {
            // **NUEVO: Cargar valor de variable**
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n", regIzq, izq->valor, regIzq, regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (izq->tipo == AST_OPERACION) {
            generarCodigoRecursivoInt(izq, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regIzq);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        }

        // **PASO 2: Evaluar operando derecho**
        int regDer = *regIndex;
        if (der->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", regDer, atoi(der->valor));
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_IDENTIFICADOR) {
            // **NUEVO: Cargar valor de variable**
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n", regDer, der->valor, regDer, regDer);
            strcat(codigoOperacion, buffer);
            (*regIndex)++;
        } else if (der->tipo == AST_OPERACION) {
            generarCodigoRecursivoInt(der, codigoOperacion, regIndex, gen);
            snprintf(buffer, sizeof(buffer), "    mov w%d, w0\n", regDer);
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
    } else if (nodo->tipo == AST_NUMERO) {
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
        if (izq->tipo == AST_NUMERO) {
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
        if (der->tipo == AST_NUMERO) {
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
    } else if (nodo->tipo == AST_NUMERO) {
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
        // Si es simple, genera printf optimizado aquí o deja que lo maneje el evaluador
        printf("[DEBUG] Detectada concatenación SIMPLE de strings para variable '%s'\n", nombreVar);
        // Aquí puedes llamar a tu generador de printf optimizado, o dejar que el evaluador lo maneje
        // ...
    }

    // **RESTO DE OPERACIONES (CÓDIGO EXISTENTE SIMPLIFICADO)**
    char codigoOperacion[2048] = "";
    char buffer[256];

    // Comentario inicial
    snprintf(buffer, sizeof(buffer), "    // Operación: %s = <expresión>\n", nombreVar);
    strcat(codigoOperacion, buffer);

    // **DETECTAR SI ES OPERACIÓN COMPUESTA**
    bool esOperacionCompuesta = esOperacionCompleja(operacionNodo);

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

        // Determinar registros según el tipo
        const char* regIzq = (tipoVar == TIPO_FLOAT) ? "s0" : (tipoVar == TIPO_DOUBLE) ? "d0" : "w0";
        const char* regDer = (tipoVar == TIPO_FLOAT) ? "s1" : (tipoVar == TIPO_DOUBLE) ? "d1" : "w1";
        const char* regRes = (tipoVar == TIPO_FLOAT) ? "s0" : (tipoVar == TIPO_DOUBLE) ? "d0" : "w0";

        // Cargar operandos con conversiones automáticas
        cargarOperandoGeneral(izq, regIzq, tipoVar, gen, codigoOperacion);
        cargarOperandoGeneral(der, regDer, tipoVar, gen, codigoOperacion);

        // Generar operación
        generarOperacionGeneral(operacionNodo->valor, regRes, regIzq, regDer, tipoVar, codigoOperacion);

        // Guardar resultado
        const char* instruccionStore = (tipoVar == TIPO_FLOAT) ? "str s0" : 
                                    (tipoVar == TIPO_DOUBLE) ? "str d0" : 
                                    (tipoVar == TIPO_CHAR) ? "strb w0" : 
                                    (tipoVar == TIPO_BOOLEAN) ? "strb w0" : "str w0";
        snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    %s, [x1]\n", nombreVar, instruccionStore);
        strcat(codigoOperacion, buffer);
    }

    insertarEnSeccionText(gen, codigoOperacion);
}