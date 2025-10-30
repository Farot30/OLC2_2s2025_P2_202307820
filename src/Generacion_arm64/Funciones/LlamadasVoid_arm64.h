#ifndef LLAMADAS_VOID_ARM64_H
#define LLAMADAS_VOID_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "FuncionesVoid_arm64.h"

// ==================== ESTRUCTURAS PARA ARGUMENTOS ====================

typedef struct {
    char nombreVariable[MAX_NOMBRE];  // Nombre de la variable pasada
    TipoDato tipo;                    // Tipo del argumento
    bool esVector;                    // Si es un vector
    bool esMatriz;                    // Si es una matriz
    bool esPorReferencia;             // Si pasa por referencia
    bool esLiteral;                   // Si es un literal (número, string, etc)
    char valorLiteral[256];           // Valor si es literal
} ArgumentoLlamada;

// ==================== BÚSQUEDA DE FUNCIONES EN EL AST ====================

// Busca la declaración de una función void en el AST
NodoAST* buscarFuncionVoidEnAST(NodoAST* raiz, const char* nombreFuncion);

// Verifica que los argumentos coincidan con los parámetros de la función
bool verificarArgumentosConParametros(ArgumentoLlamada args[], int numArgs, 
                                     ParametroInfo params[], int numParams);

// ==================== EXTRACCIÓN DE ARGUMENTOS ====================

// Extrae los argumentos de una llamada desde el nodo AST
void extraerArgumentosDesdeAST(NodoAST* argumentosNodo, ArgumentoLlamada args[], 
                              int* numArgs, NodoAST* raizAST);

// Determina el tipo de un argumento (variable, literal, expresión)
TipoDato determinarTipoArgumento(NodoAST* argNodo, NodoAST* raizAST, 
                                bool* esVector, bool* esMatriz);

// ==================== GENERACIÓN DE CÓDIGO ====================

// Genera código para cargar un argumento en un registro
void generarCargaArgumento(GeneradorDeclaraciones* gen, ArgumentoLlamada* arg, 
                          int registroDestino, PilaAmbitos* pilaAmbitos);

// Genera código para pasar argumentos a la función
void generarPasoDeArgumentos(GeneradorDeclaraciones* gen, ArgumentoLlamada args[], 
                            int numArgs, PilaAmbitos* pilaAmbitos);

// Genera código para la llamada a la función void
void generarLlamadaFuncionVoid(GeneradorDeclaraciones* gen, NodoAST* nodo, 
                              PilaAmbitos* pilaAmbitos, NodoAST* raizAST);

// ==================== MANEJO DE PARÁMETROS POR REFERENCIA ====================

// Genera código para restaurar variables pasadas por referencia
void generarRestauracionPorReferencia(GeneradorDeclaraciones* gen, 
                                     ArgumentoLlamada args[], int numArgs,
                                     ParametroInfo params[], int numParams);

#endif // LLAMADAS_VOID_ARM64_H