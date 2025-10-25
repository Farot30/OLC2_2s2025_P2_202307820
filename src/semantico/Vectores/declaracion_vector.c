#include "declaracion_vector.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h" 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 

int verificarDeclaracionVector(NodoAST* nodo) {
    printf("[DEBUG verificarDeclaracionVector] Iniciando verificación\n");
    
    // Verificar que el nodo no sea NULL
    if (!nodo) {
        printf("[ERROR] Nodo es NULL\n");
        return 0;
    }
    
    // Verificar que tenga suficientes hijos (mínimo 3)
    if (nodo->num_hijos < 3) {
        printf("[ERROR] Nodo no tiene suficientes hijos: %d\n", nodo->num_hijos);
        return 0;
    }
    
    NodoAST* tipo = nodo->hijos[0]; // El tipo está en el primer hijo
    NodoAST* identificador = nodo->hijos[1]; // El identificador está en el segundo hijo
    NodoAST* tercer_nodo = nodo->hijos[2]; // Puede ser tipoNew o lista de elementos
    
    // Verificar que los nodos no sean NULL
    if (!tipo || !identificador || !tercer_nodo) {
        printf("[ERROR] Alguno de los nodos hijo es NULL\n");
        return 0;
    }
    
    // Verificar que los valores no sean NULL
    if (!tipo->valor || !identificador->valor) {
        printf("[ERROR] Valor de tipo o identificador es NULL\n");
        return 0;
    }

    TipoDato tipoDeclarado = obtenerTipoDesdeString(tipo->valor);

    //printf("[DEBUG] Declarando vector '%s': tipo declarado=%s\n",
    //       identificador->valor, nombreTipo(tipoDeclarado));

    if (buscarSimbolo(identificador->valor)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' ya declarada", identificador->valor);
        addError(msg, ambitoActual(), identificador->linea, identificador->columna);
        printf("[ERROR] %s\n", msg);
        identificador->valido = 0;
        nodo->valido = 0;
        return 0;
    }

    // Verificar si el identificador ya está declarado
    if (tercer_nodo->tipo == AST_LISTA_ELEMENTOS) {
        // Caso: Declaración con lista de elementos directamente
        //printf("[DEBUG] Procesando vector con lista de elementos\n");
        NodoAST* contenido = tercer_nodo;

        // Verificar si el contenido tiene una lista anidada
        NodoAST* listaReal = NULL;
        int numElementos = 0;
        
        // Buscar dónde están realmente los elementos
        if (contenido->num_hijos == 1 && contenido->hijos[0]->tipo == AST_LISTA_ELEMENTOS) {
            // La lista real está en el primer hijo
            listaReal = contenido->hijos[0];
        } else if (contenido->num_hijos == 1 && contenido->hijos[0]->tipo == AST_INTERNO) {
            // Posiblemente sea una lista de elementos interna
            listaReal = contenido->hijos[0];
        } else {
            // Los elementos están directamente como hijos
            listaReal = contenido;
        }
        
        // Buscar recursivamente el nivel donde están los elementos reales
        while (listaReal->num_hijos == 1 && 
              (listaReal->hijos[0]->tipo == AST_LISTA_ELEMENTOS || 
               listaReal->hijos[0]->tipo == AST_INTERNO)) {
            listaReal = listaReal->hijos[0];
        }
        
        numElementos = listaReal->num_hijos;
        //printf("[DEBUG] Encontrados %d elementos en la lista\n", numElementos);

        char* valores = malloc(numElementos * 32 + 1); // Espacio para los valores y separadores
        if (!valores) {
            printf("[ERROR] No se pudo asignar memoria para el vector '%s'\n", identificador->valor);
            return 0;
        }
        valores[0] = '\0';

        // Usar un depurador más detallado para ver qué contiene cada elemento
        for (int i = 0; i < numElementos; i++) {
            NodoAST* elemento = listaReal->hijos[i];
            //printf("[DEBUG] Evaluando elemento %d tipo=%d valor=%s\n", 
            //    i, elemento->tipo, elemento->valor ? elemento->valor : "NULL");
            
            // Manejar cada tipo de elemento según su tipo de nodo
            if (elemento->tipo == AST_NUMERO && elemento->valor) {
                // Número literal
                switch (tipoDeclarado) {
                    case TIPO_INT: {
                        int val = atoi(elemento->valor);
                        char buffer[16];
                        snprintf(buffer, sizeof(buffer), "%d", val);
                        strcat(valores, buffer);
                        break;
                    }
                    case TIPO_FLOAT:
                    case TIPO_DOUBLE: {
                        double val = atof(elemento->valor);
                        char buffer[32];
                        snprintf(buffer, sizeof(buffer), "%f", val);
                        strcat(valores, buffer);
                        break;
                    }
                    default: {
                        strcat(valores, elemento->valor);
                        break;
                    }
                }
            } else if (elemento->tipo == AST_CADENA && elemento->valor) {
                // Cadena literal
                if (tipoDeclarado == TIPO_STRING) {
                    char buffer[1024]; // Tamaño suficiente para cadenas típicas
                    snprintf(buffer, sizeof(buffer), "\"%s\"", elemento->valor);
                    strcat(valores, buffer);
                } else {
                    printf("[ERROR] Tipo incompatible: string en vector de tipo %s\n", 
                        nombreTipo(tipoDeclarado));
                    nodo->valido = 0;
                    free(valores);
                    return 0;
                }
            } else if (elemento->tipo == AST_CARACTER && elemento->valor) {
                // Carácter literal
                if (tipoDeclarado == TIPO_CHAR) {
                    char buffer[16];
                    snprintf(buffer, sizeof(buffer), "'%s'", elemento->valor);
                    strcat(valores, buffer);
                } else {
                    printf("[ERROR] Tipo incompatible: char en vector de tipo %s\n", 
                        nombreTipo(tipoDeclarado));
                    nodo->valido = 0;
                    free(valores);
                    return 0;
                }
            } else if (elemento->tipo == AST_BOOLEAN && elemento->valor) {
                // Booleano literal
                if (tipoDeclarado == TIPO_BOOLEAN) {
                    strcat(valores, elemento->valor); // true o false
                } else {
                    printf("[ERROR] Tipo incompatible: boolean en vector de tipo %s\n", 
                        nombreTipo(tipoDeclarado));
                    nodo->valido = 0;
                    free(valores);
                    return 0;
                }
            } else if (elemento->tipo == AST_ADD) {
                ValorSimbolo resultado = evaluarExpr(elemento);
                if (resultado.inicializado && resultado.v.s) {
                    // El resultado es el string con el nuevo array, usarlo directamente
                    char formatoCompleto[numElementos * 32 + 64];
                    snprintf(formatoCompleto, sizeof(formatoCompleto), "%s", resultado.v.s);
                    
                    // Agregar el símbolo a la tabla
                    agregarSimboloDetallado(
                        identificador->valor,
                        SIMBOLO_VECTOR,
                        tipoDeclarado,
                        ambitoActual(),
                        identificador->linea,
                        identificador->columna
                    );
                    
                    Simbolo* simbolo = buscarSimbolo(identificador->valor);
                    if (simbolo) {
                        simbolo->valor.v.s = strdup(formatoCompleto);
                        simbolo->valor.inicializado = 1;
                        simbolo->valor.tipo = TIPO_STRING;

                        //printf("[DEBUG] Vector '%s' inicializado con resultado de add(): %s\n",
                        //    identificador->valor, simbolo->valor.v.s);
                    }
                    
                    free(valores);
                    return 1;
                }
            }else {
                // Para expresiones más complejas, usar evaluarExpr
                ValorSimbolo valorElemento = evaluarExpr(elemento);
                
                if (!valorElemento.inicializado) {
                    printf("[ERROR] Elemento %d no está inicializado\n", i);
                    // Intentemos obtener más información para depurar
                    //printf("[DEBUG] Tipo del elemento: %d, Valor: %s\n", 
                    //    elemento->tipo, elemento->valor ? elemento->valor : "NULL");
                    nodo->valido = 0;
                    free(valores);
                    return 0;
                }
                
                // Convertir según el tipo declarado
                char buffer[1024]; // Ampliamos el buffer para strings
                switch (tipoDeclarado) {
                    case TIPO_INT:
                        if (valorElemento.tipo != TIPO_INT && !esWidening(valorElemento.tipo, TIPO_INT)) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_INT));
                            free(valores);
                            return 0;
                        }
                        snprintf(buffer, sizeof(buffer), "%d", valorElemento.v.i);
                        break;
                    case TIPO_FLOAT:
                        if (valorElemento.tipo != TIPO_FLOAT && !esWidening(valorElemento.tipo, TIPO_FLOAT)) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_FLOAT));
                            free(valores);
                            return 0;
                        }
                        snprintf(buffer, sizeof(buffer), "%f", valorElemento.v.f);
                        break;
                    case TIPO_DOUBLE:
                        if (valorElemento.tipo != TIPO_DOUBLE && !esWidening(valorElemento.tipo, TIPO_DOUBLE)) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_DOUBLE));
                            free(valores);
                            return 0;
                        }
                        snprintf(buffer, sizeof(buffer), "%lf", valorElemento.v.d);
                        break;
                    case TIPO_CHAR:
                        if (valorElemento.tipo != TIPO_CHAR) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_CHAR));
                            free(valores);
                            return 0;
                        }
                        snprintf(buffer, sizeof(buffer), "'%c'", valorElemento.v.c);
                        break;
                    case TIPO_BOOLEAN:
                        if (valorElemento.tipo != TIPO_BOOLEAN) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_BOOLEAN));
                            free(valores);
                            return 0;
                        }
                        snprintf(buffer, sizeof(buffer), "%s", valorElemento.v.b ? "true" : "false");
                        break;
                    case TIPO_STRING:
                        if (valorElemento.tipo != TIPO_STRING) {
                            printf("[ERROR] Tipo incompatible en vector: %s no es compatible con %s\n",
                                nombreTipo(valorElemento.tipo), nombreTipo(TIPO_STRING));
                            free(valores);
                            return 0;
                        }
                        if (valorElemento.v.s) {
                            snprintf(buffer, sizeof(buffer), "\"%s\"", valorElemento.v.s);
                        } else {
                            snprintf(buffer, sizeof(buffer), "\"\"");
                        }
                        break;
                    default:
                        buffer[0] = '\0';
                        break;
                }
                strcat(valores, buffer);
            }
            
            if (i < numElementos - 1) strcat(valores, ",");
        }

        // Agregar el símbolo a la tabla
        char formatoCompleto[numElementos * 32 + 64];
        snprintf(formatoCompleto, sizeof(formatoCompleto), "[%s]", valores);
        
        // Agregar el símbolo a la tabla
        //printf("[DEBUG] Agregando vector '%s' a la tabla de símbolos con formato: %s\n", 
        //      identificador->valor, formatoCompleto);

        agregarSimboloDetallado(
            identificador->valor,
            SIMBOLO_VECTOR,
            tipoDeclarado,
            ambitoActual(),
            identificador->linea,
            identificador->columna
        );

        Simbolo* simbolo = buscarSimbolo(identificador->valor);
        if (simbolo) {
            simbolo->valor.v.s = strdup(formatoCompleto);
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING;

            //printf("[DEBUG] Vector '%s' inicializado con lista de elementos: %s\n",
            //    identificador->valor, simbolo->valor.v.s);
        }

        if (simbolo->clase == SIMBOLO_VECTOR && simbolo->valor.inicializado && simbolo->valor.v.s) {
            // Calcular y establecer longitud del vector
            char* vectorStr = strdup(simbolo->valor.v.s);
            int numElementos = 0;
            
            // Remover corchetes
            if (vectorStr[0] == '[') {
                memmove(vectorStr, vectorStr + 1, strlen(vectorStr));
                int len = strlen(vectorStr);
                if (len > 0 && vectorStr[len - 1] == ']') {
                    vectorStr[len - 1] = '\0';
                }
            }
            
            // Contar elementos separados por comas
            if (strlen(vectorStr) > 0) {
                char* token = strtok(vectorStr, ",");
                while (token != NULL) {
                    numElementos++;
                    token = strtok(NULL, ",");
                }
            }
            
            simbolo->longitud = numElementos;
            free(vectorStr);
            
            printf("[DEBUG] Longitud inicial del vector '%s' establecida en %d\n", 
                simbolo->nombre, simbolo->longitud);
        }

        free(valores);
    } else if (nodo->num_hijos >= 4) {
        // Caso: Declaración con 'new'
        NodoAST* tipoNew = tercer_nodo; // Tercer hijo es el tipo después de `new`
        NodoAST* contenido = nodo->hijos[3]; // Cuarto hijo es el contenido
        
        // Verificar que el tipo declarado coincida con el tipo después de `new`
        TipoDato tipoNewEnum = obtenerTipoDesdeString(tipoNew->valor);
        if (tipoDeclarado != tipoNewEnum) {
            char msg[128];
            snprintf(msg, sizeof(msg), "El tipo '%s' en 'new' no coincide con el tipo declarado '%s' en '%s'",
                     nombreTipo(tipoNewEnum), nombreTipo(tipoDeclarado), identificador->valor);
            addError(msg, ambitoActual(), tipoNew->linea, tipoNew->columna);
            printf("[ERROR] %s\n", msg);
            nodo->valido = 0;
            return 0;
        }

        //printf("[DEBUG] Tipos coinciden: '%s' == '%s'\n", nombreTipo(tipoDeclarado), nombreTipo(tipoNewEnum));

        if (contenido->tipo == AST_1DIMENSION) {
            //printf("[DEBUG] Procesando vector con dimensión (new)\n");

            // Verificar que tenga al menos un hijo
            if (contenido->num_hijos < 1) {
                printf("[ERROR] Nodo de dimensión no tiene hijos\n");
                return 0;
            }

            NodoAST* dimension = contenido->hijos[0]; // La dimensión está en el primer hijo de AST_1DIMENSION

            // Verificar que la dimensión sea válida
            TipoDato tipoDimension = deducirTipoExpr(dimension);
            if (tipoDimension != TIPO_INT) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Dimensión de vector '%s' debe ser de tipo int, no %s", 
                         identificador->valor, nombreTipo(tipoDimension));
                addError(msg, ambitoActual(), dimension->linea, dimension->columna);
                printf("%s\n", msg);
                dimension->valido = 0;
                nodo->valido = 0;
                return 0;
            }

            ValorSimbolo valorDimension = evaluarExpr(dimension);
            if (!valorDimension.inicializado || valorDimension.v.i <= 0) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Dimensión de vector '%s' debe ser mayor a 0", identificador->valor);
                addError(msg, ambitoActual(), dimension->linea, dimension->columna);
                printf("%s\n", msg);
                dimension->valido = 0;
                nodo->valido = 0;
                return 0;
            }

            // Agregar el símbolo a la tabla
            agregarSimboloDetallado(
                identificador->valor,
                SIMBOLO_VECTOR,
                tipoDeclarado,
                ambitoActual(),
                identificador->linea,
                identificador->columna
            );

            // Inicializar el vector con valores por defecto
            Simbolo* simbolo = buscarSimbolo(identificador->valor);
            if (simbolo) {
                int dimension_size = valorDimension.v.i;
                size_t buffer_size = (size_t)(dimension_size * 16 + 1); // +1 para el carácter nulo
                char* valores = malloc(buffer_size);

                if (!valores) {
                    printf("[ERROR] No se pudo asignar memoria para el vector '%s'\n", identificador->valor);
                    return 0;
                }

                valores[0] = '\0'; // Inicializa la cadena vacía

                // Inicializar todos los elementos con el valor por defecto según el tipo
                for (int i = 0; i < dimension_size; i++) {
                    char buffer[16];
                    switch (tipoDeclarado) {
                        case TIPO_INT:
                            snprintf(buffer, sizeof(buffer), "0");
                            break;
                        case TIPO_FLOAT:
                            snprintf(buffer, sizeof(buffer), "0.0");
                            break;
                        case TIPO_DOUBLE:
                            snprintf(buffer, sizeof(buffer), "0.0");
                            break;
                        case TIPO_CHAR:
                            snprintf(buffer, sizeof(buffer), "\\0000");
                            break;
                        case TIPO_BOOLEAN:
                            snprintf(buffer, sizeof(buffer), "false");
                            break;
                        case TIPO_STRING:
                            snprintf(buffer, sizeof(buffer), "\"\"");
                            break;
                        default:
                            snprintf(buffer, sizeof(buffer), "0");
                            break;
                    }
                    strcat(valores, buffer);
                    if (i < dimension_size - 1) strcat(valores, ",");
                }

                char formatoCompleto[buffer_size + 64];
                snprintf(formatoCompleto, sizeof(formatoCompleto), "[%s]", valores);
                
                simbolo->valor.v.s = strdup(formatoCompleto); // Usar el formato con dimensiones
                simbolo->valor.inicializado = 1;
                simbolo->valor.tipo = TIPO_STRING;

                printf("[DEBUG] Vector '%s' inicializado con %d elementos: %s\n",
                    identificador->valor, dimension_size, simbolo->valor.v.s);

                free(valores);
            }
        } else {
            printf("[ERROR] Tipo de contenido de vector desconocido: %d\n", contenido->tipo);
            return 0;
        }
    } else {
        printf("[ERROR] Estructura AST no reconocida para declaración de vector\n");
        return 0;
    }

    //printf("[DEBUG verificarDeclaracionVector] Finalizando verificación exitosamente\n");
    return 1;
}