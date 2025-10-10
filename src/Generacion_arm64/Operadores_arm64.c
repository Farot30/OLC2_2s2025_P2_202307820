#include "Operadores_arm64.h"
#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "../semantico/tipos.h"
#include "../semantico/semantico.h"
#include <string.h>
#include <stdlib.h>

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
void cargarOperandoInt(NodoAST* nodo, const char* registro, GeneradorDeclaraciones* gen, char* codigoOperacion) {
    char buffer[256];

    if (nodo->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer), "    mov %s, #%d\n", registro, atoi(nodo->valor));
    } else if (nodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr %s, [x1]\n", nodo->valor, registro);
    }
    strcat(codigoOperacion, buffer);
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
        if (tipoDestino == TIPO_BOOLEAN) {
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
        }
    }
}

void generarCodigoRecursivoDouble(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        generarCodigoRecursivoDouble(nodo->hijos[0], codigoOperacion, regIndex, gen);
        generarCodigoRecursivoDouble(nodo->hijos[1], codigoOperacion, regIndex, gen);

        // Operación
        if (strcmp(nodo->valor, "+") == 0)
            snprintf(buffer, sizeof(buffer), "    fadd d0, d0, d%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "-") == 0)
            snprintf(buffer, sizeof(buffer), "    fsub d0, d0, d%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "*") == 0)
            snprintf(buffer, sizeof(buffer), "    fmul d0, d0, d%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "/") == 0)
            snprintf(buffer, sizeof(buffer), "    fdiv d0, d0, d%d\n", *regIndex - 1);

        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_NUMERO) {
        // **DETECTAR EL TIPO REAL DEL LITERAL**
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            // Es un entero, hacer conversión explícita a double
            snprintf(buffer, sizeof(buffer),
                "    mov w%d, #%d\n"
                "    scvtf d%d, w%d\n", *regIndex, atoi(nodo->valor), *regIndex, *regIndex);
            strcat(codigoOperacion, buffer);
        } else if (tipoLiteral == TIPO_FLOAT) {
            // Es un float, cargar como float y convertir a double
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n"
                "    fcvt d%d, s%d\n", *regIndex, nombreConst, *regIndex, *regIndex, *regIndex, *regIndex);
            strcat(codigoOperacion, buffer);
        } else { // TIPO_DOUBLE
            // Es un double, cargar directamente
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n", *regIndex, nombreConst, *regIndex, *regIndex);
            strcat(codigoOperacion, buffer);
        }

        // Si es el primer operando, mueve a d0
        if (*regIndex == 1) {
            snprintf(buffer, sizeof(buffer), "    fmov d0, d1\n");
            strcat(codigoOperacion, buffer);
        }
        (*regIndex)++;
    }
}

void generarCodigoRecursivoFloat(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        generarCodigoRecursivoFloat(nodo->hijos[0], codigoOperacion, regIndex, gen);
        generarCodigoRecursivoFloat(nodo->hijos[1], codigoOperacion, regIndex, gen);

        if (strcmp(nodo->valor, "+") == 0)
            snprintf(buffer, sizeof(buffer), "    fadd s0, s0, s%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "-") == 0)
            snprintf(buffer, sizeof(buffer), "    fsub s0, s0, s%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "*") == 0)
            snprintf(buffer, sizeof(buffer), "    fmul s0, s0, s%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "/") == 0)
            snprintf(buffer, sizeof(buffer), "    fdiv s0, s0, s%d\n", *regIndex - 1);

        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_NUMERO) {
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w%d, #%d\n"
                "    scvtf s%d, w%d\n", *regIndex, atoi(nodo->valor), *regIndex, *regIndex);
        } else if (tipoLiteral == TIPO_FLOAT) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteFloatRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n", *regIndex, nombreConst, *regIndex, *regIndex);
        } else if (tipoLiteral == TIPO_DOUBLE) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n"
                "    fcvt s%d, d%d\n", *regIndex, nombreConst, *regIndex, *regIndex, *regIndex, *regIndex);
        }
        strcat(codigoOperacion, buffer);

        if (*regIndex == 1) {
            snprintf(buffer, sizeof(buffer), "    fmov s0, s1\n");
            strcat(codigoOperacion, buffer);
        }
        (*regIndex)++;
    }
}

void generarCodigoRecursivoInt(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        generarCodigoRecursivoInt(nodo->hijos[0], codigoOperacion, regIndex, gen);
        generarCodigoRecursivoInt(nodo->hijos[1], codigoOperacion, regIndex, gen);

        if (strcmp(nodo->valor, "+") == 0)
            snprintf(buffer, sizeof(buffer), "    add w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "-") == 0)
            snprintf(buffer, sizeof(buffer), "    sub w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "*") == 0)
            snprintf(buffer, sizeof(buffer), "    mul w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "/") == 0)
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "&") == 0)
            snprintf(buffer, sizeof(buffer), "    and w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "|") == 0)
            snprintf(buffer, sizeof(buffer), "    orr w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "^") == 0)
            snprintf(buffer, sizeof(buffer), "    eor w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "<<") == 0)
            snprintf(buffer, sizeof(buffer), "    lsl w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, ">>") == 0)
            snprintf(buffer, sizeof(buffer), "    lsr w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "==") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, eq\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "!=") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, ne\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "<") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, lt\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "<=") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, le\n", *regIndex - 1);
        else if (strcmp(nodo->valor, ">") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, gt\n", *regIndex - 1);
        else if (strcmp(nodo->valor, ">=") == 0)
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, w%d\n"
                "    cset w0, ge\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "&&") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    and w0, w0, w%d\n"
                "    cmp w0, #0\n"
                "    cset w0, ne\n", *regIndex - 1);
            strcat(codigoOperacion, buffer);
        } else if (strcmp(nodo->valor, "||") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    orr w0, w0, w%d\n"
                "    cmp w0, #0\n"
                "    cset w0, ne\n", *regIndex - 1);
            strcat(codigoOperacion, buffer);
        } else if (strcmp(nodo->valor, "!") == 0) {
            snprintf(buffer, sizeof(buffer),
                "    cmp w0, #0\n"
                "    cset w0, eq\n");
            strcat(codigoOperacion, buffer);
        }
        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_NUMERO) {
        TipoDato tipoLiteral = detectarTipoLiteral(nodo->valor);

        if (tipoLiteral == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w%d, #%d\n", *regIndex, atoi(nodo->valor));
        } else if (tipoLiteral == TIPO_FLOAT || tipoLiteral == TIPO_DOUBLE) {
            char nombreConst[32];
            snprintf(nombreConst, sizeof(nombreConst), "valor%g", atof(nodo->valor));
            for (int i = 0; nombreConst[i]; i++) if (nombreConst[i] == '.') nombreConst[i] = '_';
            insertarConstanteDoubleRoData(gen, nombreConst, atof(nodo->valor));
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n"
                "    fcvtzs w%d, d%d\n", *regIndex, nombreConst, *regIndex, *regIndex, *regIndex, *regIndex);
        }
        strcat(codigoOperacion, buffer);

        if (*regIndex == 1) {
            snprintf(buffer, sizeof(buffer), "    mov w0, w1\n");
            strcat(codigoOperacion, buffer);
        }
        (*regIndex)++;
    }
}

void generarCodigoRecursivoChar(NodoAST* nodo, char* codigoOperacion, int* regIndex, GeneradorDeclaraciones* gen) {
    char buffer[256];

    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        generarCodigoRecursivoChar(nodo->hijos[0], codigoOperacion, regIndex, gen);
        generarCodigoRecursivoChar(nodo->hijos[1], codigoOperacion, regIndex, gen);

        if (strcmp(nodo->valor, "+") == 0)
            snprintf(buffer, sizeof(buffer), "    add w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "-") == 0)
            snprintf(buffer, sizeof(buffer), "    sub w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "*") == 0)
            snprintf(buffer, sizeof(buffer), "    mul w0, w0, w%d\n", *regIndex - 1);
        else if (strcmp(nodo->valor, "/") == 0)
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w%d\n", *regIndex - 1);

        strcat(codigoOperacion, buffer);
    } else if (nodo->tipo == AST_NUMERO || nodo->tipo == AST_CARACTER) {
        int valor = nodo->tipo == AST_CARACTER ? nodo->valor[0] : atoi(nodo->valor);
        snprintf(buffer, sizeof(buffer),
            "    mov w%d, #%d\n", *regIndex, valor);
        strcat(codigoOperacion, buffer);

        if (*regIndex == 1) {
            snprintf(buffer, sizeof(buffer), "    mov w0, w1\n");
            strcat(codigoOperacion, buffer);
        }
        (*regIndex)++;
    }
}

void generarCodigoOperacionARM64(GeneradorDeclaraciones* gen, const char* nombreVar, NodoAST* operacionNodo, TipoDato tipoVar) {
    if (!gen || !nombreVar || !operacionNodo || operacionNodo->tipo != AST_OPERACION) return;

    char codigoOperacion[2048] = "";
    char buffer[256];

    // Comentario inicial
    snprintf(buffer, sizeof(buffer), "    // Operación: %s = <expresión>\n", nombreVar);
    strcat(codigoOperacion, buffer);

    // **DETECTAR SI ES OPERACIÓN COMPUESTA**
    bool esOperacionCompuesta = esOperacionCompleja(operacionNodo);

    if (esOperacionCompuesta) {
        // **USAR FUNCIONES RECURSIVAS** (ya optimizadas en tus funciones actuales)
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

        // Generar operación (funciona para TODOS los operadores)
        generarOperacionGeneral(operacionNodo->valor, regRes, regIzq, regDer, tipoVar, codigoOperacion);

        // Guardar resultado
        const char* instruccionStore = (tipoVar == TIPO_FLOAT) ? "str s0" : 
                                    (tipoVar == TIPO_DOUBLE) ? "str d0" : 
                                    (tipoVar == TIPO_CHAR) ? "strb w0" : 
                                    (tipoVar == TIPO_BOOLEAN) ? "strb w0" : "str w0";  // Agregar esta línea
        snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n    %s, [x1]\n", nombreVar, instruccionStore);
        strcat(codigoOperacion, buffer);
    }

    insertarEnSeccionText(gen, codigoOperacion);
}