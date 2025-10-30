#ifndef FUNCIONES_VOID_ARM64_H
#define FUNCIONES_VOID_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include <stdbool.h>

#define MAX_AMBITOS 100
#define MAX_PARAMS 20
#define MAX_NOMBRE 64

// ==================== ESTRUCTURAS PARA ÁMBITOS ====================

// Información de un parámetro
typedef struct {
    char nombre[MAX_NOMBRE];
    TipoDato tipo;
    bool esVector;      // true si es tipo[]
    bool esMatriz;      // true si es tipo[][]
    bool esPorReferencia; // true para String y arrays, false para tipos primitivos
} ParametroInfo;

// Información de una variable local en un ámbito
typedef struct {
    char nombre[MAX_NOMBRE];
    TipoDato tipo;
    bool esVector;
    bool esMatriz;
    int offset_stack;  // Desplazamiento en el stack frame
} VariableLocal;

// Ámbito (scope) de una función
typedef struct {
    char nombreFuncion[MAX_NOMBRE];
    ParametroInfo parametros[MAX_PARAMS];
    int numParametros;
    VariableLocal variables[50]; // Variables locales declaradas
    int numVariables;
    int tamanioStackFrame; // Tamaño total del stack frame
    bool esVoid; // true para void, false para funciones con retorno
} AmbitoFuncion;

// Pila de ámbitos
typedef struct {
    AmbitoFuncion ambitos[MAX_AMBITOS];
    int tope; // Índice del ámbito actual
} PilaAmbitos;

// ==================== GESTIÓN DE ÁMBITOS ====================

void inicializarPilaAmbitos(PilaAmbitos* pila);
void pushAmbito(PilaAmbitos* pila, const char* nombreFuncion, bool esVoid);
AmbitoFuncion* popAmbito(PilaAmbitos* pila);
AmbitoFuncion* getAmbitoActual(PilaAmbitos* pila);
void agregarParametroAmbito(PilaAmbitos* pila, const char* nombre, TipoDato tipo, 
                           bool esVector, bool esMatriz);
void agregarVariableLocalAmbito(PilaAmbitos* pila, const char* nombre, TipoDato tipo,
                               bool esVector, bool esMatriz);
VariableLocal* buscarVariableEnAmbito(PilaAmbitos* pila, const char* nombre);
ParametroInfo* buscarParametroEnAmbito(PilaAmbitos* pila, const char* nombre);

// ==================== ANÁLISIS DEL AST ====================

// Extrae información de parámetros desde el nodo AST
void extraerParametrosDesdeAST(NodoAST* parametrosNodo, ParametroInfo params[], int* numParams);

// Determina si un tipo debe pasar por referencia
bool esTipoPorReferencias(TipoDato tipo, bool esVector, bool esMatriz);

// Calcula el tamaño del stack frame necesario
int calcularTamanioStackFrame(AmbitoFuncion* ambito);

// ==================== GENERACIÓN DE CÓDIGO ARM64 ====================

// Genera el prólogo de la función (guarda registros, crea stack frame)
void generarPrologoFuncion(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito);

// Genera el epílogo de la función (restaura registros, retorna)
void generarEpilogoFuncion(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito);

// Genera código para cargar parámetros desde registros al stack
void generarCargaParametros(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito);

// Genera la declaración completa de una función void
void generarDeclaracionFuncionVoid(GeneradorDeclaraciones* gen, NodoAST* nodo, 
                                  PilaAmbitos* pilaAmbitos, NodoAST* raizAST);

// Procesa el cuerpo de la función (declaraciones, sentencias, etc.)
void procesarCuerpoFuncion(GeneradorDeclaraciones* gen, NodoAST* cuerpoNodo, 
                          PilaAmbitos* pilaAmbitos, NodoAST* raizAST);

// ==================== UTILIDADES ====================

// Genera un nombre único para la etiqueta de la función
void generarNombreFuncion(const char* nombre, char* buffer, size_t bufferSize);

// Obtiene el offset en el stack para una variable
int obtenerOffsetVariable(AmbitoFuncion* ambito, const char* nombre);

// Obtiene el tamaño en bytes de un tipo
int obtenerTamanioTipo(TipoDato tipo);

#endif // FUNCIONES_VOID_ARM64_H