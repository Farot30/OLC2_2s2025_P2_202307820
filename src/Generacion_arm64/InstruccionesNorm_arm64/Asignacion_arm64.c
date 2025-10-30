#include "Asignacion_arm64.h"
#include "Operadores_arm64.h"
#include "declaracionConst_arm64.h"
#include "../../AST/err.h"  
#include "../../semantico/tipos.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../Embebidas/Embebidas_arm64.h"
#include "../Vectores_arm64/AccesoMatriz_arm64.h"
#include "../Funciones/FuncionesVoid_arm64.h"
#include "../Funciones/Funciones_arm64.h"
#include <stdio.h>
#include <string.h>

extern NodoAST* astGlobal;

void generarAsignacionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION) {
        printf("[DEBUG] Nodo inválido o tipo incorrecto en generarAsignacionARM64\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG] Nodo de asignación con menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* valorNodo = nodo->hijos[1];

    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo identificador inválido en generarAsignacionARM64\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    
    const char * nombreReal = nombreVariable;

    if (valorNodo->tipo == AST_OPERACION && strcmp(valorNodo->valor, "+") == 0 &&
        valorNodo->num_hijos == 2) {
        
        TipoDato tipoIzq = deducirTipoExprARM64(valorNodo->hijos[0], astGlobal);
        TipoDato tipoDer = deducirTipoExprARM64(valorNodo->hijos[1], astGlobal);
        
        if (tipoIzq == TIPO_STRING && tipoDer == TIPO_STRING) {
            char nombreBuffer[64];
            static int contador_concat = 0;
            snprintf(nombreBuffer, sizeof(nombreBuffer), "concat_buffer_%d", contador_concat++);
            
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "%s: .skip 256\n", nombreBuffer);
            insertarEnSeccionBss(gen, buffer);
            
            char codigo[512];
            
            if (valorNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x0, =%s\n"
                    "    ldr x0, [x0]\n",
                    valorNodo->hijos[0]->valor);
            } else if (valorNodo->hijos[0]->tipo == AST_CADENA) {
                char nombreLit[64];
                snprintf(nombreLit, sizeof(nombreLit), "str_lit_%d", contador_concat);
                snprintf(buffer, sizeof(buffer), "%s: .asciz %s\n", nombreLit, valorNodo->hijos[0]->valor);
                insertarEnSeccionData(gen, buffer);
                snprintf(codigo, sizeof(codigo), "    ldr x0, =%s\n", nombreLit);
            }
            
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =%s\n"
                "    mov x2, x1\n"
                "concat_loop_1_%d:\n"
                "    ldrb w3, [x0], #1\n"
                "    strb w3, [x2], #1\n"
                "    cbnz w3, concat_loop_1_%d\n"
                "    sub x2, x2, #1\n",
                nombreBuffer, contador_concat, contador_concat);
            strcat(codigo, buffer);
            
            if (valorNodo->hijos[1]->tipo == AST_IDENTIFICADOR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    ldr x0, [x0]\n",
                    valorNodo->hijos[1]->valor);
            } else if (valorNodo->hijos[1]->tipo == AST_CADENA) {
                char nombreLit2[64];
                snprintf(nombreLit2, sizeof(nombreLit2), "str_lit_%d", contador_concat + 1);
                char bufferData[128];
                snprintf(bufferData, sizeof(bufferData), "%s: .asciz \"%s\"\n", nombreLit2, valorNodo->hijos[1]->valor);
                insertarEnSeccionData(gen, bufferData);
                snprintf(buffer, sizeof(buffer), "    ldr x0, =%s\n", nombreLit2);
            }
            strcat(codigo, buffer);
            
            snprintf(buffer, sizeof(buffer),
                "concat_loop_2_%d:\n"
                "    ldrb w3, [x0], #1\n"
                "    strb w3, [x2], #1\n"
                "    cbnz w3, concat_loop_2_%d\n",
                contador_concat, contador_concat);
            strcat(codigo, buffer);
            
            //  Usar nombreReal en lugar de nombreVariable
            snprintf(buffer, sizeof(buffer),
                "    ldr x0, =%s\n"
                "    ldr x1, =%s\n"
                "    str x1, [x0]\n",
                nombreReal, nombreBuffer);
            strcat(codigo, buffer);
            
            insertarEnSeccionText(gen, codigo);
            return;
        }
    }

    if (valorNodo && valorNodo->tipo == AST_ADD) {
        printf("[DEBUG] Asignación con Arrays.add detectada para '%s'\n", nombreVariable);
        
        if (valorNodo->num_hijos >= 1 && valorNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
            const char* nombreVectorOrigen = valorNodo->hijos[0]->valor;
            
            NodoAST* vectorDestinoDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombreVariable);
            NodoAST* vectorOrigenDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombreVectorOrigen);
            
            if (!vectorDestinoDecl) {
                printf("[ERROR] Vector destino '%s' no encontrado en el AST\n", nombreVariable);
                return;
            }
            
            if (!vectorOrigenDecl) {
                printf("[ERROR] Vector origen '%s' no encontrado en el AST\n", nombreVectorOrigen);
                return;
            }

            TipoDato tipoDestino = obtenerTipoVectorDesdeAST(vectorDestinoDecl);
            TipoDato tipoOrigen = obtenerTipoVectorDesdeAST(vectorOrigenDecl);

            if (tipoDestino != tipoOrigen) {
                printf("[ERROR SEMÁNTICO] El vector '%s' es de tipo '%s', pero el vector origen '%s' es de tipo '%s'. Los tipos no coinciden.\n",
                       nombreVariable, obtenerNombreTipo(tipoDestino), nombreVectorOrigen, obtenerNombreTipo(tipoOrigen));
                char msgError[256];
                snprintf(msgError, sizeof(msgError), 
                         "El vector '%s' (%s) no puede ser asignado desde '%s' (%s). Tipos incompatibles.",
                         nombreVariable, obtenerNombreTipo(tipoDestino), nombreVectorOrigen, obtenerNombreTipo(tipoOrigen));
                addError(msgError, "global", nodo->linea, nodo->columna);
                return;
            }

            printf("[DEBUG] Verificación de tipos exitosa: '%s' y '%s' son ambos de tipo '%s'\n",
                   nombreVariable, nombreVectorOrigen, obtenerNombreTipo(tipoDestino));

            generarArraysAddOfArm64(gen, nombreVariable, valorNodo);
            return;
        } else {
            printf("[ERROR] El primer hijo de Arrays.add debe ser un identificador de vector\n");
            return;
        }
    }

    TipoDato tipoVariable = deducirTipoExprARM64(valorNodo, astGlobal);
    TipoDato tipoValor = deducirTipoExprARM64(valorNodo, astGlobal);

    if (tipoVariable != TIPO_DESCONOCIDO && tipoValor != TIPO_DESCONOCIDO && tipoVariable != tipoValor) {
        printf("[ERROR SEMÁNTICO] Variable '%s' es de tipo '%s', pero se le asigna tipo '%s'.\n",
               nombreVariable, obtenerNombreTipo(tipoVariable), obtenerNombreTipo(tipoValor));
        char msgError[256];
        snprintf(msgError, sizeof(msgError),
                 "Variable '%s' (%s) no puede ser asignada con tipo '%s'. Tipos incompatibles.",
                 nombreVariable, obtenerNombreTipo(tipoVariable), obtenerNombreTipo(tipoValor));
        addError(msgError, "global", nodo->linea, nodo->columna);
        return;
    }

    char codigo[512] = "";

    if (tipoVariable == TIPO_STRING && valorNodo->tipo == AST_CADENA) {
        char nombreLiteral[64];
        snprintf(nombreLiteral, sizeof(nombreLiteral), "str_lit_%d", gen->contador_literales_string++);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreLiteral, valorNodo->valor);
        insertarEnSeccionData(gen, buffer);
        
        //  Usar nombreReal
        snprintf(codigo, sizeof(codigo),
            "    ldr x2, =%s\n"
            "    ldr x1, =%s\n"
            "    str x2, [x1]\n",
            nombreLiteral, nombreReal);
        insertarEnSeccionText(gen, codigo);
        return;
    }

    int regIndex = 1;
    ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
    strcat(codigo, resultado.codigo);

    //  Usar nombreReal en todas las asignaciones
    if (tipoVariable == TIPO_INT) {
        if (valorNodo->tipo == AST_IDENTIFICADOR) {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x1, =%s\n"
                "    ldr w1, [x1]\n"
                "    ldr x2, =%s\n"
                "    str w1, [x2]\n",
                valorNodo->valor, nombreReal);
        } else {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    mov w1, %s\n"
                "    ldr x2, =%s\n"
                "    str w1, [x2]\n",
                resultado.resultado, nombreReal);
        }
    } else if (tipoVariable == TIPO_FLOAT) {
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    ldr x2, =%s\n"
            "    str s0, [x2]\n",
             nombreReal);
    } else if (tipoVariable == TIPO_DOUBLE) {
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    fmov d0, %s\n"
            "    ldr x2, =%s\n"
            "    str d0, [x2]\n",
            resultado.resultado, nombreReal);
    } else if (tipoVariable == TIPO_CHAR) {
        if (valorNodo->tipo == AST_IDENTIFICADOR) {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x1, =%s\n"
                "    ldrb w0, [x1]\n"
                "    ldr x2, =%s\n"
                "    strb w0, [x2]\n",
                valorNodo->valor, nombreReal);
        } else {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    mov w0, %s\n"
                "    ldr x2, =%s\n"
                "    strb w0, [x2]\n",
                resultado.resultado, nombreReal);
        }
    } else if (tipoVariable == TIPO_BOOLEAN) {
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    mov w1, %s\n"
            "    ldr x2, =%s\n"
            "    strb w1, [x2]\n",
            resultado.resultado, nombreReal);
    } else if (tipoVariable == TIPO_STRING) {
        if (valorNodo->tipo == AST_IDENTIFICADOR) {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x3, =%s\n"
                "    ldr x3, [x3]\n"
                "    ldr x2, =%s\n"
                "    str x3, [x2]\n",
                valorNodo->valor, nombreReal);
        } else if (valorNodo->tipo == AST_CADENA) {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x3, =%s\n"
                "    ldr x2, =%s\n"
                "    str x3, [x2]\n",
                resultado.resultado, nombreReal);
        } else {
            if (resultado.resultado[0] == 'x' || resultado.resultado[0] == 'w') {
                snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                    "    mov x3, %s\n"
                    "    ldr x2, =%s\n"
                    "    str x3, [x2]\n",
                    resultado.resultado, nombreReal);
            } else {
                snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                    "    ldr x3, =%s\n"
                    "    ldr x3, [x3]\n"
                    "    ldr x2, =%s\n"
                    "    str x3, [x2]\n",
                    resultado.resultado, nombreReal);
            }
        }
    }

    insertarEnSeccionText(gen, codigo);
}



void generarAsignacionCompuestaARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION_COMPUESTA) {
        printf("[DEBUG] Nodo inválido para asignación compuesta\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG] Asignación compuesta con menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];  // Variable destino
    NodoAST* valorNodo = nodo->hijos[1];  // Valor a operar
    
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo identificador inválido en asignación compuesta\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    const char* operador = nodo->valor;  // Operador como "+=", "-=", "*=", "/="
    
    char codigo[512];
    char buffer[256];
    
    // Extraer el operador base (+ de +=, - de -=, etc.)
    char opBase[3] = {0};
    if (strlen(operador) >= 2) {
        strncpy(opBase, operador, strlen(operador) - 1);
    }
    
    snprintf(buffer, sizeof(buffer), "    // Asignación compuesta: %s %s <valor>\n", nombreVariable, operador);
    strcpy(codigo, buffer);
    
    // **PASO 1: Cargar valor actual de la variable**
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    ldr w0, [x1]\n", nombreVariable);
    strcat(codigo, buffer);
    
    // **PASO 2: Cargar el valor operando**
    if (valorNodo->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", atoi(valorNodo->valor));
        strcat(codigo, buffer);
    } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x2, =%s\n"
            "    ldr w1, [x2]\n", valorNodo->valor);
        strcat(codigo, buffer);
    } else if (valorNodo->tipo == AST_OPERACION) {
        // Para operaciones más complejas, usar función recursiva
        int regIndex = 2;
        generarCodigoRecursivoOperacion(valorNodo, codigo, &regIndex);
        snprintf(buffer, sizeof(buffer), "    mov w1, w0\n    ldr x1, =%s\n    ldr w0, [x1]\n", nombreVariable);
        strcat(codigo, buffer);
    }
    
    // **PASO 3: Realizar la operación**
    if (strcmp(opBase, "+") == 0) {
        snprintf(buffer, sizeof(buffer), "    add w0, w0, w1\n");
    } else if (strcmp(opBase, "-") == 0) {
        snprintf(buffer, sizeof(buffer), "    sub w0, w0, w1\n");
    } else if (strcmp(opBase, "*") == 0) {
        snprintf(buffer, sizeof(buffer), "    mul w0, w0, w1\n");
    } else if (strcmp(opBase, "/") == 0) {
        snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w1\n");
    } else if (strcmp(opBase, "&") == 0) {
        snprintf(buffer, sizeof(buffer), "    and w0, w0, w1\n");
    } else if (strcmp(opBase, "|") == 0) {
        snprintf(buffer, sizeof(buffer), "    orr w0, w0, w1\n");
    } else if (strcmp(opBase, "^") == 0) {
        snprintf(buffer, sizeof(buffer), "    eor w0, w0, w1\n");
    }
    strcat(codigo, buffer);
    
    // **PASO 4: Guardar resultado de vuelta en la variable**
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    str w0, [x1]\n", nombreVariable);
    strcat(codigo, buffer);
    
    insertarEnSeccionText(gen, codigo);
}

void generarAsignacionCompuestaEnFuncion(GeneradorDeclaraciones* gen, NodoAST* nodo, 
                                        PilaAmbitos* pilaAmbitos) {
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION_COMPUESTA) {
        printf("[ERROR] Nodo inválido para asignación compuesta\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Asignación compuesta con menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* valorNodo = nodo->hijos[1];
    
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Nodo identificador inválido en asignación compuesta\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    const char* operador = nodo->valor;
    
    AmbitoFuncion* ambito = getAmbitoActual(pilaAmbitos);
    if (!ambito) {
        printf("[ERROR] No hay ámbito actual\n");
        return;
    }
    
    // ✅ Buscar si es un parámetro
    ParametroInfo* param = buscarParametroEnAmbito(pilaAmbitos, nombreVariable);
    
    // ✅ Buscar si es variable local (dentro de la función)
    VariableLocal* varLocal = buscarVariableEnAmbito(pilaAmbitos, nombreVariable);
    
    char codigo[1024];
    char buffer[1024];
    
    snprintf(buffer, sizeof(buffer), "    // Asignación compuesta: %s %s ...\n", 
             nombreVariable, operador);
    strcpy(codigo, buffer);
    
    // ✅ CASO 1: Es un parámetro (puede ser por referencia o por valor)
    if (param) {
        // Calcular offset en el stack
        int offset = 0;
        for (int i = 0; i < ambito->numParametros; i++) {
            if (strcmp(ambito->parametros[i].nombre, nombreVariable) == 0) {
                break;
            }
            int tamParam = obtenerTamanioTipo(ambito->parametros[i].tipo);
            if (ambito->parametros[i].esVector || ambito->parametros[i].esMatriz) tamParam = 8;
            offset += (tamParam + 7) & ~7;
        }
        
        // ✅ STRING POR REFERENCIA (concatenación)
        if (param->tipo == TIPO_STRING && param->esPorReferencia && strcmp(operador, "+=") == 0) {
            static int contador_buffer = 0;
            char nombreBuffer[64];
            snprintf(nombreBuffer, sizeof(nombreBuffer), "concat_buffer_%d", contador_buffer);

            // Reservar buffer en .bss
            snprintf(buffer, sizeof(buffer), "%s: .skip 256\n", nombreBuffer);
            insertarEnSeccionBss(gen, buffer);

            // Generar literal si es necesario
            if (valorNodo->tipo == AST_CADENA) {
                char nombreLiteral[64];
                snprintf(nombreLiteral, sizeof(nombreLiteral), "str_concat_lit_%d", contador_buffer);
                snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreLiteral, valorNodo->valor);
                insertarEnSeccionData(gen, buffer);

                // Código de concatenación que MODIFICA el original
                snprintf(buffer, sizeof(buffer),
                    "    // Concatenación por referencia: %s += literal\n"
                    "    ldr x0, [sp, #%d]      // Cargar puntero al String original\n"
                    "    ldr x0, [x0]           // Obtener dirección del string\n"
                    "    ldr x1, =%s            // Buffer destino\n"
                    "    mov x2, x0             // Puntero fuente\n"
                    "    mov x3, x1             // Puntero destino\n"
                    "concat_copy_orig_%d:\n"
                    "    ldrb w4, [x2], #1\n"
                    "    strb w4, [x3], #1\n"
                    "    cbnz w4, concat_copy_orig_%d\n"
                    "    sub x3, x3, #1         // Retroceder al null\n"
                    "    ldr x2, =%s            // Literal a concatenar\n"
                    "concat_copy_new_%d:\n"
                    "    ldrb w4, [x2], #1\n"
                    "    strb w4, [x3], #1\n"
                    "    cbnz w4, concat_copy_new_%d\n"
                    "    // Actualizar puntero del parámetro\n"
                    "    ldr x0, [sp, #%d]\n"
                    "    ldr x1, =%s\n"
                    "    str x1, [x0]\n",
                    nombreVariable, offset, nombreBuffer,
                    contador_buffer, contador_buffer,
                    nombreLiteral, contador_buffer, contador_buffer,
                    offset, nombreBuffer);
                strcat(codigo, buffer);
            }
            contador_buffer++;
        }
        // ✅ TIPOS PRIMITIVOS POR VALOR (int, float, etc.)
        else {
            // Cargar valor actual del parámetro
            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr w0, [sp, #%d]  // cargar parámetro '%s'\n", 
                    offset, nombreVariable);
            } else if (param->tipo == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr d0, [sp, #%d]  // cargar parámetro '%s'\n", 
                    offset, nombreVariable);
            } else if (param->tipo == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr s0, [sp, #%d]  // cargar parámetro '%s'\n", 
                    offset, nombreVariable);
            }
            strcat(codigo, buffer);
            
            // Cargar el valor operando
            if (valorNodo->tipo == AST_NUMERO) {
                snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", atoi(valorNodo->valor));
                strcat(codigo, buffer);
            } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
                // Buscar si es parámetro o variable local
                ParametroInfo* paramVal = buscarParametroEnAmbito(pilaAmbitos, valorNodo->valor);
                VariableLocal* varLocalVal = buscarVariableEnAmbito(pilaAmbitos, valorNodo->valor);
                
                char nombreRealVal[128];
                if (varLocalVal) {
                    snprintf(nombreRealVal, sizeof(nombreRealVal), "%s_%s", 
                            ambito->nombreFuncion, valorNodo->valor);
                } else {
                    strncpy(nombreRealVal, valorNodo->valor, sizeof(nombreRealVal) - 1);
                }
                
                snprintf(buffer, sizeof(buffer),
                    "    ldr x2, =%s\n"
                    "    ldr w1, [x2]\n", nombreRealVal);
                strcat(codigo, buffer);
            }
            
            // Realizar la operación
            char opBase[3] = {0};
            if (strlen(operador) >= 2) {
                strncpy(opBase, operador, strlen(operador) - 1);
            }
            
            if (strcmp(opBase, "+") == 0) {
                snprintf(buffer, sizeof(buffer), "    add w0, w0, w1\n");
            } else if (strcmp(opBase, "-") == 0) {
                snprintf(buffer, sizeof(buffer), "    sub w0, w0, w1\n");
            } else if (strcmp(opBase, "*") == 0) {
                snprintf(buffer, sizeof(buffer), "    mul w0, w0, w1\n");
            } else if (strcmp(opBase, "/") == 0) {
                snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w1\n");
            }
            strcat(codigo, buffer);
            
            // ✅ GUARDAR EN STACK (pero NO afecta al original porque es copia)
            snprintf(buffer, sizeof(buffer),
                "    str w0, [sp, #%d]  // modificar copia local del parámetro\n", offset);
            strcat(codigo, buffer);
        }
    }
    // ✅ CASO 2: Es variable local (dentro de la función) - USAR NOMBRE ÚNICO
    else if (varLocal) {
        char nombreUnico[128];
        snprintf(nombreUnico, sizeof(nombreUnico), "%s_%s", 
                ambito->nombreFuncion, nombreVariable);
        
        // Cargar variable local (global con nombre único)
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n", nombreUnico);
        strcat(codigo, buffer);
        
        // Cargar el valor operando
        if (valorNodo->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", atoi(valorNodo->valor));
            strcat(codigo, buffer);
        } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
            VariableLocal* varLocalVal = buscarVariableEnAmbito(pilaAmbitos, valorNodo->valor);
            
            char nombreRealVal[128];
            if (varLocalVal) {
                snprintf(nombreRealVal, sizeof(nombreRealVal), "%s_%s", 
                        ambito->nombreFuncion, valorNodo->valor);
            } else {
                strncpy(nombreRealVal, valorNodo->valor, sizeof(nombreRealVal) - 1);
            }
            
            snprintf(buffer, sizeof(buffer),
                "    ldr x2, =%s\n"
                "    ldr w1, [x2]\n", nombreRealVal);
            strcat(codigo, buffer);
        }
        
        // Realizar la operación
        char opBase[3] = {0};
        if (strlen(operador) >= 2) {
            strncpy(opBase, operador, strlen(operador) - 1);
        }
        
        if (strcmp(opBase, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add w0, w0, w1\n");
        } else if (strcmp(opBase, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub w0, w0, w1\n");
        } else if (strcmp(opBase, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul w0, w0, w1\n");
        } else if (strcmp(opBase, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w1\n");
        }
        strcat(codigo, buffer);
        
        // Guardar resultado
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str w0, [x1]\n", nombreUnico);
        strcat(codigo, buffer);
    }
    // ✅ CASO 3: Es variable global
    else {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n", nombreVariable);
        strcat(codigo, buffer);
        
        // Cargar el valor operando
        if (valorNodo->tipo == AST_NUMERO) {
            snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", atoi(valorNodo->valor));
            strcat(codigo, buffer);
        } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x2, =%s\n"
                "    ldr w1, [x2]\n", valorNodo->valor);
            strcat(codigo, buffer);
        }
        
        // Realizar la operación
        char opBase[3] = {0};
        if (strlen(operador) >= 2) {
            strncpy(opBase, operador, strlen(operador) - 1);
        }
        
        if (strcmp(opBase, "+") == 0) {
            snprintf(buffer, sizeof(buffer), "    add w0, w0, w1\n");
        } else if (strcmp(opBase, "-") == 0) {
            snprintf(buffer, sizeof(buffer), "    sub w0, w0, w1\n");
        } else if (strcmp(opBase, "*") == 0) {
            snprintf(buffer, sizeof(buffer), "    mul w0, w0, w1\n");
        } else if (strcmp(opBase, "/") == 0) {
            snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w1\n");
        }
        strcat(codigo, buffer);
        
        // Guardar resultado
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str w0, [x1]\n", nombreVariable);
        strcat(codigo, buffer);
    }
    
    insertarEnSeccionText(gen, codigo);
}

void generarAsignacionCompuestaEnFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* nodo, 
                                                PilaAmbitosRetorno* pilaRetorno) {
    printf("[DEPURACIÓN] Entrando a generarAsignacionCompuestaEnFuncionRetorno\n");
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION_COMPUESTA) {
        printf("[DEPURACIÓN] Parámetros inválidos\n");
        return;
    }
    if (nodo->num_hijos < 2) {
        printf("[DEPURACIÓN] Menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* valorNodo = nodo->hijos[1];
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEPURACIÓN] idNodo inválido\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    const char* operador = nodo->valor;

    AmbitoFuncionRetorno* ambito = getAmbitoRetornoActual(pilaRetorno);
    if (!ambito) {
        printf("[DEPURACIÓN] Ámbito de retorno nulo\n");
        return;
    }

    printf("[DEPURACIÓN] Nombre de variable: %s\n", nombreVariable);

    char codigo[1024] = "";
    char buffer[256];
    
    // ✅ Buscar si es parámetro
    ParametroInfo* param = NULL;
    for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
        if (strcmp(ambito->ambito_base.parametros[i].nombre, nombreVariable) == 0) {
            param = &ambito->ambito_base.parametros[i];
            break;
        }
    }
    
    // ✅ Buscar si es variable local
    VariableLocal* varLocal = NULL;
    for (int i = 0; i < ambito->ambito_base.numVariables; i++) {
        if (strcmp(ambito->ambito_base.variables[i].nombre, nombreVariable) == 0) {
            varLocal = &ambito->ambito_base.variables[i];
            break;
        }
    }

    snprintf(buffer, sizeof(buffer), "    // Asignación compuesta: %s %s ...\n", 
             nombreVariable, operador);
    strcpy(codigo, buffer);
    
    // ✅ CASO 1: Es parámetro (en stack)
    if (param) {
        int offset = 0;
        for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
            if (strcmp(ambito->ambito_base.parametros[i].nombre, nombreVariable) == 0) {
                break;
            }
            int tamParam = obtenerTamanioTipo(ambito->ambito_base.parametros[i].tipo);
            if (ambito->ambito_base.parametros[i].esVector || ambito->ambito_base.parametros[i].esMatriz) {
                tamParam = 8;
            }
            offset += (tamParam + 7) & ~7;
        }
        
        printf("[DEBUG] Parámetro encontrado en offset: %d\n", offset);
        
        // Cargar valor del parámetro
        if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer),
                "    ldr w0, [sp, #%d]  // cargar parámetro '%s'\n", 
                offset, nombreVariable);
        } else if (param->tipo == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer),
                "    ldr d0, [sp, #%d]  // cargar parámetro '%s'\n", 
                offset, nombreVariable);
        } else if (param->tipo == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer),
                "    ldr s0, [sp, #%d]  // cargar parámetro '%s'\n", 
                offset, nombreVariable);
        }
        strcat(codigo, buffer);
    }
    // ✅ CASO 2: Es variable local (con nombre único)
    else if (varLocal) {
        char nombreUnico[128];
        snprintf(nombreUnico, sizeof(nombreUnico), "%s_%s", 
                ambito->ambito_base.nombreFuncion, nombreVariable);
        
        printf("[DEBUG] Variable local con nombre único: %s\n", nombreUnico);
        
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n", nombreUnico);
        strcat(codigo, buffer);
    }
    // ✅ CASO 3: Es variable global
    else {
        printf("[DEBUG] Variable global: %s\n", nombreVariable);
        
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n", nombreVariable);
        strcat(codigo, buffer);
    }

    // Evaluar el valor operando
    int regIndex = 2;
    ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
    
    if (!resultado.resultado || strlen(resultado.resultado) == 0) {
        printf("[ERROR] resultado.resultado inválido\n");
        return;
    }
    
    printf("[DEPURACIÓN] Resultado de expresión: %s (tipo: %d)\n", resultado.resultado, resultado.tipo);
    
    strcat(codigo, resultado.codigo);

    // Determinar el operador base
    char opBase[3] = {0};
    if (strlen(operador) >= 2) {
        strncpy(opBase, operador, strlen(operador) - 1);
    }

    // Generar la operación compuesta
    if (strcmp(opBase, "+") == 0) {
        snprintf(buffer, sizeof(buffer), "    add w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "-") == 0) {
        snprintf(buffer, sizeof(buffer), "    sub w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "*") == 0) {
        snprintf(buffer, sizeof(buffer), "    mul w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "/") == 0) {
        snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "%") == 0) {
        snprintf(buffer, sizeof(buffer),
            "    sdiv w10, w0, %s\n"
            "    mul w11, w10, %s\n"
            "    sub w0, w0, w11\n", 
            resultado.resultado, resultado.resultado);
    } else if (strcmp(opBase, "&") == 0) {
        snprintf(buffer, sizeof(buffer), "    and w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "|") == 0) {
        snprintf(buffer, sizeof(buffer), "    orr w0, w0, %s\n", resultado.resultado);
    } else if (strcmp(opBase, "^") == 0) {
        snprintf(buffer, sizeof(buffer), "    eor w0, w0, %s\n", resultado.resultado);
    } else {
        printf("[ERROR] Operador compuesto no soportado: %s\n", operador);
        return;
    }
    
    strcat(codigo, buffer);

    // ✅ Guardar resultado
    if (param) {
        int offset = 0;
        for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
            if (strcmp(ambito->ambito_base.parametros[i].nombre, nombreVariable) == 0) {
                break;
            }
            int tamParam = obtenerTamanioTipo(ambito->ambito_base.parametros[i].tipo);
            if (ambito->ambito_base.parametros[i].esVector || ambito->ambito_base.parametros[i].esMatriz) {
                tamParam = 8;
            }
            offset += (tamParam + 7) & ~7;
        }
        
        snprintf(buffer, sizeof(buffer),
            "    str w0, [sp, #%d]  // guardar parámetro '%s'\n", 
            offset, nombreVariable);
    } else if (varLocal) {
        char nombreUnico[128];
        snprintf(nombreUnico, sizeof(nombreUnico), "%s_%s", 
                ambito->ambito_base.nombreFuncion, nombreVariable);
        
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str w0, [x1]\n", nombreUnico);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str w0, [x1]\n", nombreVariable);
    }
    strcat(codigo, buffer);

    printf("[DEBUG] Código generado para asignación compuesta:\n%s\n", codigo);
    insertarEnSeccionText(gen, codigo);
}