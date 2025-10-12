#include "tabla_simbolo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tipos.h"
#include "stdbool.h"
#include "../simbolos/Evaluador.h"

static Simbolo* tabla = NULL;
int contadorSimbolos = 0;

// tabla_simbolo.c
char* pilaAmbitos[MAX_SCOPES];
int topeAmbitos = 0;

void entrarAmbito(const char* nombre) {
    if (topeAmbitos < MAX_SCOPES) {
        pilaAmbitos[topeAmbitos++] = strdup(nombre);
        printf("[DEBUG] Entrando al ámbito: %s\n", nombre);
    }
}

Simbolo* buscarSimboloPorId(int id) {
    Simbolo* actual = tabla;
    while (actual) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

void salirAmbito(void) {
    if (topeAmbitos > 0) {
        printf("[DEBUG] Saliendo del ámbito: %s\n", pilaAmbitos[topeAmbitos - 1]);
        free(pilaAmbitos[--topeAmbitos]);
    }
}

const char* ambitoActual(void) {
    return (topeAmbitos > 0) ? pilaAmbitos[topeAmbitos-1] : "global";
}

void inicializarTabla() {
    tabla = NULL;
    contadorSimbolos = 0;
}

void liberarTabla() {
    Simbolo* actual = tabla;
    while (actual) {
        Simbolo* temp = actual;
        actual = actual->siguiente;
        
        // Liberar nombre
        if (temp->nombre) {
            free(temp->nombre);
            temp->nombre = NULL;
        }
        
        // Liberar ámbito
        if (temp->ambito) {
            free(temp->ambito);
            temp->ambito = NULL;
        }
        
        // Liberar valor string con verificación adicional
        if (temp->tipo == TIPO_STRING && temp->valor.v.s) {
            free(temp->valor.v.s);
            temp->valor.v.s = NULL;
        } else if (temp->clase == SIMBOLO_VECTOR && temp->valor.inicializado && temp->valor.v.s) {
            free(temp->valor.v.s);
            temp->valor.v.s = NULL;
        } else if (temp->clase == SIMBOLO_MATRIZ && temp->valor.inicializado && temp->valor.v.s) {
            free(temp->valor.v.s);
            temp->valor.v.s = NULL;
        } else if (temp->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL && temp->valor.inicializado && temp->valor.v.s) {
            free(temp->valor.v.s);
            temp->valor.v.s = NULL;
        }
        
        free(temp);
    }
    tabla = NULL;
    contadorSimbolos = 0; // Reset del contador
}
Simbolo* buscarSimboloEnAmbito(const char* nombre, const char* ambito) {
    if (!nombre || !ambito) return NULL;
    
    Simbolo* actual = tabla;
    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0 && 
            strcmp(actual->ambito, ambito) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}
Simbolo* buscarSimboloConAmbito(const char* nombre) {
    if (!nombre) return NULL;
    
    // Primero buscar en el ámbito actual (PRIORIDAD LOCAL)
    const char* ambitoActivo = ambitoActual();
    Simbolo* simboloLocal = buscarSimboloEnAmbito(nombre, ambitoActivo);
    if (simboloLocal) {
        printf("[DEBUG buscarSimboloConAmbito] Encontrado '%s' en ámbito actual '%s' con valor %d\n", 
               nombre, ambitoActivo, simboloLocal->valor.v.i);
        return simboloLocal;
    }
    
    // Buscar en ámbitos superiores (pila de ámbitos)
    for (int i = topeAmbitos - 2; i >= 0; i--) {
        Simbolo* simboloSuperior = buscarSimboloEnAmbito(nombre, pilaAmbitos[i]);
        if (simboloSuperior) {
            printf("[DEBUG buscarSimboloConAmbito] Encontrado '%s' en ámbito superior '%s'\n", 
                   nombre, pilaAmbitos[i]);
            return simboloSuperior;
        }
    }
    
    printf("[DEBUG buscarSimboloConAmbito] Símbolo '%s' no encontrado\n", nombre);
    return NULL;
}

bool existeSimboloEnAmbitoActual(const char* nombre) {
    return buscarSimboloEnAmbito(nombre, ambitoActual()) != NULL;
}

void agregarSimboloSiNoExiste(const char* nombre, TipoSimbolo clase, TipoDato tipo,
                              const char* ambito, int linea, int columna) {
    // Verificar si ya existe en el ámbito actual
    if (buscarSimboloEnAmbito(nombre, ambito)) {
        printf("[DEBUG agregarSimboloSiNoExiste] Símbolo '%s' ya existe en ámbito '%s', no se crea duplicado\n", 
               nombre, ambito);
        return;
    }
    
    // Si no existe, crear nuevo símbolo
    printf("[DEBUG agregarSimboloSiNoExiste] Creando nuevo símbolo '%s' en ámbito '%s'\n", 
           nombre, ambito);
    agregarSimboloDetallado(nombre, clase, tipo, ambito, linea, columna);
}

void limpiarSimbolosDeAmbito(const char* ambito) {
    if (!ambito) return;
    
    Simbolo* actual = tabla;
    Simbolo* anterior = NULL;
    
    printf("[DEBUG limpiarSimbolosDeAmbito] Limpiando símbolos del ámbito: %s\n", ambito);
    
    while (actual) {
        if (strcmp(actual->ambito, ambito) == 0) {
            printf("[DEBUG limpiarSimbolosDeAmbito] Eliminando símbolo '%s' del ámbito '%s'\n", 
                   actual->nombre, ambito);
            
            // Desenlazar de la lista
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                tabla = actual->siguiente;
            }
            
            // Liberar memoria
            Simbolo* aEliminar = actual;
            actual = actual->siguiente; // Avanzar antes de liberar
            
            if (aEliminar->nombre) free(aEliminar->nombre);
            if (aEliminar->ambito) free(aEliminar->ambito);
            if (aEliminar->tipo == TIPO_STRING && aEliminar->valor.v.s) {
                free(aEliminar->valor.v.s);
            }
            free(aEliminar);
            
            // No actualizar anterior porque eliminamos el nodo actual
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}


static Simbolo* crearSimbolo(const char* nombre, TipoSimbolo clase, TipoDato tipo,
                             const char* ambito, int linea, int columna) {
    Simbolo* nuevo = (Simbolo*)malloc(sizeof(Simbolo));
    
    // INICIALIZACIÓN EXPLÍCITA DE TODOS LOS CAMPOS
    memset(nuevo, 0, sizeof(Simbolo)); // Limpia toda la memoria
    
    nuevo->id = ++contadorSimbolos;
    nuevo->nombre = strdup(nombre);
    nuevo->clase = clase;
    nuevo->tipo = tipo;
    nuevo->ambito = ambito ? strdup(ambito) : strdup("global");
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    nuevo->nodo_bloque = NULL;
    nuevo->nodo_return = NULL;
    
    // INICIALIZACIÓN CRÍTICA: es_constante SIEMPRE debe ser 0 por defecto
    nuevo->es_constante = 0;
    
    // Inicializar valor
    nuevo->valor.tipo = tipo;
    nuevo->valor.inicializado = 0;
    nuevo->valor.es_cast = 0;

    switch(tipo){
        case TIPO_INT:    nuevo->valor.v.i = 0; break;
        case TIPO_FLOAT:  nuevo->valor.v.f = 0.0f; break;
        case TIPO_DOUBLE: nuevo->valor.v.d = 0.0; break;
        case TIPO_CHAR:   nuevo->valor.v.c = '\0'; break;
        case TIPO_BOOLEAN:nuevo->valor.v.b = 0; break;
        case TIPO_STRING: nuevo->valor.v.s = strdup(""); break;
        case TIPO_VOID:  nuevo->valor.v.s = NULL; break;
        case TIPO_FUNCION: nuevo->valor.v.s = NULL; break;
        default: break;
    }
    
    printf("[DEBUG crearSimbolo] Creado símbolo '%s': id=%d, es_constante=%d, tipo=%d\n", 
           nombre, nuevo->id, nuevo->es_constante, tipo);
    
    return nuevo;
}
void marcarComoConstante(Simbolo* simbolo) {
    if (simbolo) {
        simbolo->es_constante = 1;
        printf("[DEBUG marcarComoConstante] Símbolo '%s' marcado como constante\n", simbolo->nombre);
    }
}

void validarSimbolo(Simbolo* simbolo, const char* operacion) {
    if (!simbolo) {
        printf("[DEBUG validarSimbolo] ERROR: Símbolo NULL en operación '%s'\n", operacion);
        return;
    }
    
    printf("[DEBUG validarSimbolo] Símbolo '%s' para '%s': es_constante=%d, inicializado=%d\n",
           simbolo->nombre, operacion, simbolo->es_constante, simbolo->valor.inicializado);
    
    // Verificar integridad de memoria
    if (simbolo->nombre == NULL) {
        printf("[DEBUG validarSimbolo] ERROR: Nombre del símbolo es NULL\n");
    }
    
    if (simbolo->ambito == NULL) {
        printf("[DEBUG validarSimbolo] ERROR: Ámbito del símbolo es NULL\n");
    }
}

void eliminarSimbolo(const char* nombre) {
    Simbolo* actual = tabla;
    Simbolo* anterior = NULL;
    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0) {
            // Desenlazar
            if (anterior) {
                anterior->siguiente = actual->siguiente;
            } else {
                tabla = actual->siguiente;
            }
            // Liberar memoria
            if (actual->nombre) free(actual->nombre);
            if (actual->ambito) free(actual->ambito);
            if (actual->tipo == TIPO_STRING && actual->valor.v.s) free(actual->valor.v.s);
            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

void agregarSimboloDetallado(const char* nombre, TipoSimbolo clase, TipoDato tipo,
                             const char* ambito, int linea, int columna) {
    Simbolo* nuevo = crearSimbolo(nombre, clase, tipo, ambito, linea, columna);
    nuevo->siguiente = NULL;
    
    if (!tabla) {
        tabla = nuevo;
    } else {
        Simbolo* actual = tabla;
        while (actual->siguiente) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    
    printf("[DEBUG agregarSimboloDetallado] Símbolo '%s' agregado a tabla con es_constante=%d\n", 
           nombre, nuevo->es_constante);
}

void depurarTodosSimbolos(const char* momento) {
    printf("[DEBUG depurarTodosSimbolos] === ESTADO DE SÍMBOLOS EN: %s ===\n", momento);
    Simbolo* actual = tabla;
    int contador = 0;
    
    while (actual && contador < 20) { // Limitar para evitar loops infinitos
        printf("[DEBUG] Símbolo %d: nombre='%s', es_constante=%d, id=%d, memoria=0x%p\n",
               contador, actual->nombre ? actual->nombre : "NULL", 
               actual->es_constante, actual->id, (void*)actual);
        actual = actual->siguiente;
        contador++;
    }
    printf("[DEBUG depurarTodosSimbolos] === FIN DEBUG ===\n");
}

Simbolo* buscarSimboloConDepuracion(const char* nombre) {
    printf("[DEBUG buscarSimboloConDepuracion] Buscando símbolo '%s'\n", nombre);
    
    Simbolo* resultado = buscarSimboloConAmbito(nombre);
    
    if (resultado) {
        printf("[DEBUG buscarSimboloConDepuracion] Encontrado '%s': es_constante=%d, memoria=0x%p\n", 
               nombre, resultado->es_constante, (void*)resultado);
        
        // Verificación adicional para 'mensaje'
        if (strcmp(nombre, "mensaje") == 0) {
            printf("[DEBUG MENSAJE ESPECIAL] es_constante=%d, clase=%d, tipo=%d\n",
                   resultado->es_constante, resultado->clase, resultado->tipo);
                   
            // Verificar si hay corrupción de memoria
            if (resultado->es_constante != 0 && resultado->es_constante != 1) {
                printf("[DEBUG MENSAJE ESPECIAL] ¡¡¡MEMORIA CORRUPTA!!! es_constante=%d (no es 0 ni 1)\n",
                       resultado->es_constante);
                // Forzar a 0 para corregir
                resultado->es_constante = 0;
                printf("[DEBUG MENSAJE ESPECIAL] Corregido a es_constante=0\n");
            }
        }
    } else {
        printf("[DEBUG buscarSimboloConDepuracion] NO encontrado '%s'\n", nombre);
    }
    
    return resultado;
}

Simbolo* buscarSimbolo(const char* nombre) {
    Simbolo* actual = tabla;
    while (actual) {
        if (strcmp(actual->nombre, nombre) == 0) return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

void salirAmbitoForLoop(const char* nombreVariableIteracion) {
    if (topeAmbitos > 0) {
        const char* ambitoASalir = pilaAmbitos[topeAmbitos - 1];
        const char* ambitoPadre = (topeAmbitos > 1) ? pilaAmbitos[topeAmbitos - 2] : "global";
        
        printf("[DEBUG salirAmbitoForLoop] Saliendo del ámbito: %s, moviendo '%s' a ámbito padre: %s\n", 
               ambitoASalir, nombreVariableIteracion, ambitoPadre);
        
        // Buscar la variable de iteración en el ámbito actual
        Simbolo* varIteracion = buscarSimboloEnAmbito(nombreVariableIteracion, ambitoASalir);
        if (varIteracion) {
            // Cambiar el ámbito de la variable al ámbito padre
            if (varIteracion->ambito) free(varIteracion->ambito);
            varIteracion->ambito = strdup(ambitoPadre);
            printf("[DEBUG salirAmbitoForLoop] Variable '%s' movida al ámbito '%s'\n", 
                   nombreVariableIteracion, ambitoPadre);
        }
        
        // Limpiar otros símbolos del ámbito (si los hay)
        Simbolo* actual = tabla;
        Simbolo* anterior = NULL;
        
        while (actual) {
            if (strcmp(actual->ambito, ambitoASalir) == 0 && 
                strcmp(actual->nombre, nombreVariableIteracion) != 0) {
                // Este símbolo no es la variable de iteración, eliminarlo
                printf("[DEBUG salirAmbitoForLoop] Eliminando símbolo '%s' del ámbito '%s'\n", 
                       actual->nombre, ambitoASalir);
                
                if (anterior) {
                    anterior->siguiente = actual->siguiente;
                } else {
                    tabla = actual->siguiente;
                }
                
                Simbolo* aEliminar = actual;
                actual = actual->siguiente;
                
                if (aEliminar->nombre) free(aEliminar->nombre);
                if (aEliminar->ambito) free(aEliminar->ambito);
                if (aEliminar->tipo == TIPO_STRING && aEliminar->valor.v.s) {
                    free(aEliminar->valor.v.s);
                }
                free(aEliminar);
            } else {
                anterior = actual;
                actual = actual->siguiente;
            }
        }
        
        // Liberar nombre del ámbito y decrementar tope
        free(pilaAmbitos[--topeAmbitos]);
    }
}

static const char* nombreTipoDato(TipoDato t){
    switch(t){
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_DOUBLE: return "double";
        case TIPO_CHAR: return "char";
        case TIPO_STRING: return "string";
        case TIPO_BOOLEAN: return "boolean";
        case TIPO_VOID: return "void";
        case TIPO_FUNCION: return "funcion";
        default: return "desconocido";
    }
}

static const char* nombreClase(TipoSimbolo c){
    switch(c){
        case SIMBOLO_VARIABLE: return "varriable";
        case SIMBOLO_FUNCION: return "funcion";
        case SIMBOLO_VECTOR: return "vector";
        case SIMBOLO_MATRIZ: return "matriz";
        case SIMBOLO_ARRAY_MULTIDIMENSIONAL: return "array_multidimensional";
        case SIMBOLO_FUNCION_VOID: return "funcion_void";
        case SIMBOLO_CONST: return "constante";
        default: return "?";
    }
}

void setValorSimbolo(Simbolo* sim, TipoDato tipoValor, ValorSimbolo val) {
    if (!sim) {
        printf("[DEBUG setValorSimbolo] Símbolo es NULL\n");
        return;
    }


    printf("[DEBUG setValorSimbolo] === INICIANDO ASIGNACIÓN ===\n");
    printf("[DEBUG setValorSimbolo] Variable: %s, Ámbito: %s, Nuevo valor: ", sim->nombre, sim->ambito);
    printf("[DEBUG setValorSimbolo] Variable: %s\n", sim->nombre);
    printf("[DEBUG setValorSimbolo] Tipo símbolo: %d, Tipo valor: %d\n", sim->tipo, tipoValor);
    printf("[DEBUG setValorSimbolo] Valor inicializado: %d\n", val.inicializado);

    if (sim->clase == SIMBOLO_VECTOR) {
        printf("[DEBUG setValorSimbolo] ¡OPERACIÓN EN VECTOR '%s'! Tipo: %d->%d\n", 
               sim->nombre, sim->tipo, tipoValor);
    }

    if (sim->clase == SIMBOLO_MATRIZ) {
        printf("[DEBUG setValorSimbolo] ¡OPERACIÓN EN MATRIZ '%s'! Tipo: %d->%d\n", 
               sim->nombre, sim->tipo, tipoValor);
    }

    // Actualizar el ámbito de la asignación
    //if (sim->ambito) free(sim->ambito);
    //sim->ambito = strdup(ambitoActual());

    // PRIMERO: Verificar widening casting (char -> int -> float -> double)
    if (esWidening(tipoValor, sim->tipo)) {
        printf("[DEBUG setValorSimbolo] Aplicando widening casting de %d a %d\n", tipoValor, sim->tipo);
        double base = 0;
        switch (tipoValor) {
            case TIPO_CHAR:   base = (unsigned char)val.v.c; break;
            case TIPO_INT:    base = val.v.i; break;
            case TIPO_FLOAT:  base = val.v.f; break;
            case TIPO_DOUBLE: base = val.v.d; break;
            default: break;
        }
        printf("[DEBUG setValorSimbolo] Valor base calculado: %lf\n", base);
        
        // Asignar según el tipo del símbolo
        switch (sim->tipo) {
            case TIPO_DOUBLE: 
                sim->valor.v.d = base; 
                printf("[DEBUG setValorSimbolo] Double asignado: %lf\n", sim->valor.v.d);
                break;
            case TIPO_FLOAT:  
                sim->valor.v.f = (float)base; 
                printf("[DEBUG setValorSimbolo] Float asignado: %f\n", sim->valor.v.f);
                break;
            case TIPO_INT:    
                sim->valor.v.i = (int)base; 
                printf("[DEBUG setValorSimbolo] Int asignado: %d\n", sim->valor.v.i);
                break;
            case TIPO_CHAR:   
                sim->valor.v.c = (char)base; 
                printf("[DEBUG setValorSimbolo] Char asignado: %c\n", sim->valor.v.c);
                break;
            default: break;
        }
        
        sim->valor.tipo = sim->tipo;
        sim->valor.inicializado = 1;
        printf("[DEBUG setValorSimbolo] Widening completado exitosamente\n");
        return;
    }

    // SEGUNDO: Asignación directa para tipos iguales
    if (sim->tipo == tipoValor) {
        printf("[DEBUG setValorSimbolo] Asignación directa (tipos iguales)\n");
        
        // Asignar según el tipo específico
        switch (sim->tipo) {
            case TIPO_INT:
                sim->valor.v.i = val.v.i;
                printf("[DEBUG setValorSimbolo] Int directo: %d\n", sim->valor.v.i);
                break;
            case TIPO_FLOAT:
                sim->valor.v.f = val.v.f;
                printf("[DEBUG setValorSimbolo] Float directo: %f\n", sim->valor.v.f);
                break;
            case TIPO_DOUBLE:
                sim->valor.v.d = val.v.d;
                printf("[DEBUG setValorSimbolo] Double directo: %lf\n", sim->valor.v.d);
                break;
            case TIPO_CHAR:
                sim->valor.v.c = val.v.c;
                printf("[DEBUG setValorSimbolo] Char directo: %c\n", sim->valor.v.c);
                break;
            case TIPO_BOOLEAN:
                sim->valor.v.b = val.v.b;
                printf("[DEBUG setValorSimbolo] Boolean directo: %s\n", val.v.b ? "true" : "false");
                break;
            case TIPO_STRING:
                if (sim->valor.v.s) free(sim->valor.v.s);
                sim->valor.v.s = val.v.s ? strdup(val.v.s) : NULL;
                printf("[DEBUG setValorSimbolo] String directo: %s\n", sim->valor.v.s ? sim->valor.v.s : "NULL");
                break;
            default:
                printf("[ERROR setValorSimbolo] Tipo desconocido en asignación directa: %d\n", sim->tipo);
                return;
        }
        
        sim->valor.tipo = sim->tipo;
        sim->valor.inicializado = 1;
        return;
    }

    // TERCERO: Narrowing casting manual (double -> float -> int -> char)
    if (esNarrowing(tipoValor, sim->tipo)) {
        printf("[DEBUG setValorSimbolo] Intentando narrowing casting\n");
        // Solo permitir si el valor proviene de un cast explícito
        if (!val.es_cast) {
            printf("[ERROR] Narrowing casting solo permitido con cast explícito: %s <- %s\n",
                   nombreTipoDato(sim->tipo), nombreTipoDato(tipoValor));
            return;
        }
        double base = 0;
        switch (tipoValor) {
            case TIPO_DOUBLE: base = val.v.d; break;
            case TIPO_FLOAT:  base = val.v.f; break;
            case TIPO_INT:    base = val.v.i; break;
            case TIPO_CHAR:   base = (unsigned char)val.v.c; break;
            default: break;
        }
        switch (sim->tipo) {
            case TIPO_DOUBLE: sim->valor.v.d = base; break;
            case TIPO_FLOAT:  sim->valor.v.f = (float)base; break;
            case TIPO_INT:    sim->valor.v.i = (int)base; break;
            case TIPO_CHAR:   sim->valor.v.c = (char)base; break;
            default: break;
        }
        sim->valor.tipo = sim->tipo;
        sim->valor.inicializado = 1;
        printf("[DEBUG setValorSimbolo] Narrowing realizado\n");
        return;
    }

    // Si no es compatible, reporta error
    printf("[ERROR] Asignación incompatible: %s <- %s\n",
           nombreTipoDato(sim->tipo), nombreTipoDato(tipoValor));
}

ValorSimbolo getValorSimbolo(Simbolo* sim) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    printf("[DEBUG getValorSimbolo] Variable: %s, Ámbito: %s, Valor actual: ", sim->nombre, sim->ambito);
    if (!sim) {
        r.tipo = TIPO_DESCONOCIDO;
        r.inicializado = 0;
        return r;
    }
    
    // Copiar tipo e inicialización
    r.tipo = sim->valor.tipo;
    r.inicializado = sim->valor.inicializado;
    
    if (!r.inicializado) {
        printf("[DEBUG] Variable '%s' no está inicializada\n", sim->nombre);
        return r;
    }
    
    // Copiar valor según el tipo
    switch (r.tipo) {
        case TIPO_INT:
            r.v.i = sim->valor.v.i;
            printf("[DEBUG] getValorSimbolo: '%s' int = %d\n", sim->nombre, r.v.i);
            break;
        case TIPO_FLOAT:
            r.v.f = sim->valor.v.f;
            printf("[DEBUG] getValorSimbolo: '%s' float = %f\n", sim->nombre, r.v.f);
            break;
        case TIPO_DOUBLE:
            r.v.d = sim->valor.v.d;
            printf("[DEBUG] getValorSimbolo: '%s' double = %lf\n", sim->nombre, r.v.d);
            break;
        case TIPO_CHAR:
            r.v.c = sim->valor.v.c;
            printf("[DEBUG] getValorSimbolo: '%s' char = '%c'\n", sim->nombre, r.v.c);
            break;
        case TIPO_BOOLEAN:
            r.v.b = sim->valor.v.b;
            printf("[DEBUG] getValorSimbolo: '%s' boolean = %s\n", sim->nombre, r.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
            if (sim->valor.v.s) {
                r.v.s = strdup(sim->valor.v.s); // Crear una copia
                printf("[DEBUG] getValorSimbolo: '%s' String = '%s'\n", sim->nombre, r.v.s);
            } else {
                r.v.s = NULL;
                printf("[DEBUG] getValorSimbolo: '%s' String = NULL\n", sim->nombre);
            }
            break;
        default:
            printf("[DEBUG] getValorSimbolo: '%s' tipo desconocido %d\n", sim->nombre, r.tipo);
            r.inicializado = 0;
            break;
    }
    
    return r;
}

void mostrarTablaSimbolos() {
    printf("\n=== Tabla de Símbolos ===\n");
    printf("%-4s %-23s %-28s %-10s %-70s %-28s %-6s %-6s\n",
           "ID", "Nombre", "Clase", "TipoDato", "Valor", "Ambito", "Linea", "Col");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    Simbolo* actual = tabla;

    while (actual) {
        char valor[100] = "(sin valor)";
        
        // Verificar si el símbolo está inicializado
        if (actual->valor.inicializado) {
            // Manejar vectores, matrices y arrays multidimensionales de forma especial
            if (actual->clase == SIMBOLO_VECTOR || actual->clase == SIMBOLO_MATRIZ || actual->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
                if (actual->valor.v.s != NULL) {
                    // Para vectores, matrices y arrays multidimensionales, mostrar el contenido del string
                    if (strlen(actual->valor.v.s) > 40) {
                        char temp[65];
                        strncpy(temp, actual->valor.v.s, 65);
                        temp[65] = '\0';
                        snprintf(valor, sizeof(valor), "%s...", temp);
                    } else {
                        snprintf(valor, sizeof(valor), "%s", actual->valor.v.s);
                    }
                } else {
                    snprintf(valor, sizeof(valor), "[]");
                }
            } else {
                // Para variables normales, manejar según el tipo
                switch (actual->tipo) {
                    case TIPO_INT:
                        snprintf(valor, sizeof(valor), "%d", actual->valor.v.i);
                        break;
                    case TIPO_FLOAT:
                        snprintf(valor, sizeof(valor), "%.2ff", actual->valor.v.f);
                        break;
                    case TIPO_STRING:
                        if (actual->valor.v.s) {
                            snprintf(valor, sizeof(valor), "\"%s\"", actual->valor.v.s);
                        } else {
                            snprintf(valor, sizeof(valor), "null");
                        }
                        break;
                    case TIPO_DOUBLE:
                        snprintf(valor, sizeof(valor), "%.2lf", actual->valor.v.d);
                        break;
                    case TIPO_CHAR:
                        snprintf(valor, sizeof(valor), "'%c'", actual->valor.v.c);
                        break;
                    case TIPO_BOOLEAN:
                        snprintf(valor, sizeof(valor), "%s", actual->valor.v.b ? "true" : "false");
                        break;
                    default:
                        snprintf(valor, sizeof(valor), "(tipo desconocido)");
                        break;
                }
            }
        }

        printf("%-4d %-23s %-28s %-10s %-70s %-28s %-6d %-6d\n",
               actual->id,
               actual->nombre,
               nombreClase(actual->clase),
               nombreTipoDato(actual->tipo),
               valor,
               actual->ambito,
               actual->linea,
               actual->columna);
        actual = actual->siguiente;
    }
    printf("=========================================================================================================================================================\n");
}

// Agregar a tabla_simbolo.c o crear un archivo nuevo
void asignarValor(Simbolo* simbolo, ValorSimbolo valor) {
    if (!simbolo || !valor.inicializado) return;
    
    // Liberar memoria si ya había un valor string
    if (simbolo->valor.inicializado && simbolo->valor.tipo == TIPO_STRING && simbolo->valor.v.s) {
        free(simbolo->valor.v.s);
        simbolo->valor.v.s = NULL;
    }
    
    // Asignar según el tipo
    switch (simbolo->tipo) {
        case TIPO_INT:
            switch (valor.tipo) {
                case TIPO_INT:    simbolo->valor.v.i = valor.v.i; break;
                case TIPO_FLOAT:  simbolo->valor.v.i = (int)valor.v.f; break;
                case TIPO_DOUBLE: simbolo->valor.v.i = (int)valor.v.d; break;
                case TIPO_CHAR:   simbolo->valor.v.i = (int)valor.v.c; break;
                default: return; // Error de tipo
            }
            break;
        case TIPO_FLOAT:
            switch (valor.tipo) {
                case TIPO_INT:    simbolo->valor.v.f = (float)valor.v.i; break;
                case TIPO_FLOAT:  simbolo->valor.v.f = valor.v.f; break;
                case TIPO_DOUBLE: simbolo->valor.v.f = (float)valor.v.d; break;
                case TIPO_CHAR:   simbolo->valor.v.f = (float)valor.v.c; break;
                default: return; // Error de tipo
            }
            break;
        case TIPO_DOUBLE:
            switch (valor.tipo) {
                case TIPO_INT:    simbolo->valor.v.d = (double)valor.v.i; break;
                case TIPO_FLOAT:  simbolo->valor.v.d = (double)valor.v.f; break;
                case TIPO_DOUBLE: simbolo->valor.v.d = valor.v.d; break;
                case TIPO_CHAR:   simbolo->valor.v.d = (double)valor.v.c; break;
                default: return; // Error de tipo
            }
            break;
        case TIPO_CHAR:
            switch (valor.tipo) {
                case TIPO_CHAR: simbolo->valor.v.c = valor.v.c; break;
                case TIPO_INT:  simbolo->valor.v.c = (char)valor.v.i; break;
                default: return; // Error de tipo
            }
            break;
        case TIPO_BOOLEAN:
            if (valor.tipo == TIPO_BOOLEAN) {
                simbolo->valor.v.b = valor.v.b;
            } else {
                return; // Error de tipo
            }
            break;
        case TIPO_STRING:
            if (valor.tipo == TIPO_STRING && valor.v.s) {
                simbolo->valor.v.s = strdup(valor.v.s);
            } else {
                return; // Error de tipo
            }
            break;
        default:
            return; // Tipo desconocido
    }
    
    simbolo->valor.inicializado = 1;
    simbolo->valor.tipo = simbolo->tipo;
}


void modificarClaseSimbolo(const char* nombre, TipoSimbolo nuevaClase) {
    Simbolo* simbolo = buscarSimbolo(nombre);
    if (simbolo) {
        simbolo->clase = nuevaClase;
        printf("[DEBUG] Modificada clase de símbolo '%s' a %d\n", nombre, nuevaClase);
    }
}


typedef struct {
    char nombreLocal[64];
    char nombreOriginal[64];
    char ambitoOriginal[64];
} Referencia;

static Referencia referencias[100];
static int numReferencias = 0;

int esTipoPorReferencia(TipoDato tipo, TipoSimbolo clase) {
    // Por referencia: String y arreglos (vectores, matrices, arrays)
    return (tipo == TIPO_STRING || 
            clase == SIMBOLO_VECTOR || 
            clase == SIMBOLO_MATRIZ || 
            clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL);
}

void crearReferencia(const char* nombreLocal, Simbolo* simboloOriginal) {
    if (!nombreLocal || !simboloOriginal || numReferencias >= 100) return;
    
    // Registrar la referencia
    strncpy(referencias[numReferencias].nombreLocal, nombreLocal, 63);
    strncpy(referencias[numReferencias].nombreOriginal, simboloOriginal->nombre, 63);
    strncpy(referencias[numReferencias].ambitoOriginal, simboloOriginal->ambito, 63);
    numReferencias++;
    
    printf("[DEBUG REFERENCIA] Creada referencia: '%s' -> '%s' (ámbito: %s)\n", 
           nombreLocal, simboloOriginal->nombre, simboloOriginal->ambito);
}

void sincronizarReferencia(const char* nombreLocal) {
    if (!nombreLocal) return;
    
    // Buscar la referencia
    for (int i = 0; i < numReferencias; i++) {
        if (strcmp(referencias[i].nombreLocal, nombreLocal) == 0) {
            // Encontrar símbolos local y original
            Simbolo* simboloLocal = buscarSimboloEnAmbito(nombreLocal, ambitoActual());
            Simbolo* simboloOriginal = buscarSimboloEnAmbito(referencias[i].nombreOriginal, 
                                                           referencias[i].ambitoOriginal);
            
            if (simboloLocal && simboloOriginal) {
                // Sincronizar valor del local al original
                if (simboloOriginal->valor.inicializado && simboloOriginal->valor.tipo == TIPO_STRING && simboloOriginal->valor.v.s) {
                    free(simboloOriginal->valor.v.s);
                }
                
                simboloOriginal->valor = simboloLocal->valor;
                
                // Para strings, duplicar el puntero
                if (simboloLocal->valor.tipo == TIPO_STRING && simboloLocal->valor.v.s) {
                    simboloOriginal->valor.v.s = strdup(simboloLocal->valor.v.s);
                }
                
                printf("[DEBUG REFERENCIA] Sincronizada referencia '%s' -> '%s'\n", 
                       nombreLocal, referencias[i].nombreOriginal);
            }
            break;
        }
    }
}

void limpiarReferenciasDeAmbito(const char* ambito) {
    if (!ambito) return;
    
    // Sincronizar todas las referencias de este ámbito antes de limpiar
    for (int i = 0; i < numReferencias; i++) {
        Simbolo* simboloLocal = buscarSimboloEnAmbito(referencias[i].nombreLocal, ambito);
        if (simboloLocal) {
            sincronizarReferencia(referencias[i].nombreLocal);
        }
    }
    
    // Remover referencias del ámbito
    int j = 0;
    for (int i = 0; i < numReferencias; i++) {
        Simbolo* simboloLocal = buscarSimboloEnAmbito(referencias[i].nombreLocal, ambito);
        if (!simboloLocal) {
            // Esta referencia no es de este ámbito, mantenerla
            referencias[j] = referencias[i];
            j++;
        } else {
            printf("[DEBUG REFERENCIA] Removida referencia '%s' del ámbito '%s'\n", 
                   referencias[i].nombreLocal, ambito);
        }
    }
    numReferencias = j;
}


void salirAmbitoConLimpieza(void) {
    if (topeAmbitos > 0) {
        const char* ambitoASalir = pilaAmbitos[topeAmbitos - 1];
        printf("[DEBUG] Saliendo del ámbito: %s\n", ambitoASalir);
        
        // Limpiar referencias antes de limpiar símbolos
        limpiarReferenciasDeAmbito(ambitoASalir);
        
        // Limpiar símbolos de este ámbito
        limpiarSimbolosDeAmbito(ambitoASalir);
        
        // Liberar nombre del ámbito y decrementar tope
        free(pilaAmbitos[--topeAmbitos]);
    }
}