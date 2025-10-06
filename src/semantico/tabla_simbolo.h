#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H
#define MAX_SCOPES 128
#include "tipos.h"
#include "../comun/strutil.h"


typedef enum {
    SIMBOLO_VARIABLE,
    SIMBOLO_FUNCION,
    SIMBOLO_FUNCION_VOID,
    SIMBOLO_VECTOR,
    SIMBOLO_MATRIZ,
    SIMBOLO_ARRAY_MULTIDIMENSIONAL,
    SIMBOLO_CONST,
} TipoSimbolo;
typedef struct {
    TipoDato tipo;
    union {
        int i;
        float f;
        double d;
        char c;
        char* s;
        int b;
    } v;
    int inicializado;
    int es_cast;
} ValorSimbolo;

typedef struct Simbolo {
    int id;
    char* nombre;
    TipoSimbolo clase;
    TipoDato tipo;
    char* ambito;
    int linea;
    int columna;
    ValorSimbolo valor; 
    struct Simbolo* siguiente;
    int inicializado;   
    int longitud; 
    NodoAST* nodo_bloque; 
    NodoAST* nodo_return;
    NodoAST* nodo_params;
    int es_constante;
} Simbolo;

extern char* pilaAmbitos[MAX_SCOPES];
extern int topeAmbitos;

void entrarAmbito(const char* nombre);
void salirAmbito(void);
const char* ambitoActual(void);

void inicializarTabla();
void liberarTabla();
void agregarSimboloDetallado(const char* nombre, TipoSimbolo clase, TipoDato tipo,
                             const char* ambito, int linea, int columna);
Simbolo* buscarSimbolo(const char* nombre);
void mostrarTablaSimbolos();
void setValorSimbolo(Simbolo* sim, TipoDato tipo, ValorSimbolo valor);
ValorSimbolo getValorSimbolo(Simbolo* sim);
void asignarValor(Simbolo* simbolo, ValorSimbolo valor);

Simbolo* buscarSimboloPorId(int id);

void asignarElementoArrayMultidimensional(Simbolo* simbolo, int* indices, int numIndices, ValorSimbolo valor);

void modificarClaseSimbolo(const char* nombre, TipoSimbolo nuevaClase);
void eliminarSimbolo(const char* nombre);

void agregarSimboloSiNoExiste(const char* nombre, TipoSimbolo clase, TipoDato tipo,
                             const char* ambito, int linea, int columna);

Simbolo* buscarSimboloEnAmbito(const char* nombre, const char* ambito);

void limpiarSimbolosDeAmbito(const char* ambito);
void salirAmbitoConLimpieza(void);

void salirAmbitoForLoop(const char* nombreVariableIteracion);

Simbolo* buscarSimboloConAmbito(const char* nombre);
void validarSimbolo(Simbolo* simbolo, const char* operacion);
Simbolo* buscarSimboloConDepuracion(const char* nombre);

#endif