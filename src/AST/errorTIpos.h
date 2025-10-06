#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

typedef enum {
    // Errores sintácticos
    ERROR_SINTACTICO_GENERAL,
    ERROR_SINTACTICO_DECLARACION,
    ERROR_SINTACTICO_ASIGNACION,
    ERROR_SINTACTICO_PRINT,
    ERROR_SINTACTICO_CAST,
    ERROR_SINTACTICO_EXPRESION,
    ERROR_SINTACTICO_IDENTIFICADOR,
    ERROR_SINTACTICO_EOF,
    
    // Errores semánticos
    ERROR_SEMANTICO_VARIABLE_NO_DECLARADA,
    ERROR_SEMANTICO_VARIABLE_YA_DECLARADA,
    ERROR_SEMANTICO_TIPO_INCOMPATIBLE,
    ERROR_SEMANTICO_CAST_INVALIDO,
    ERROR_SEMANTICO_OPERACION_INVALIDA,
    ERROR_SEMANTICO_ASIGNACION_SIN_DECLARACION 
} TipoErrorSintactico; 

typedef struct ErrorSintactico {
    int id;
    TipoErrorSintactico tipo;
    char* mensaje;
    char* contexto;
    int linea;
    int columna;
    struct ErrorSintactico* sig;
} ErrorSintactico;


void initErroresSintacticos(void);
void addErrorSintactico(const char* mensaje, 
                        const char* contexto, 
                        int linea, int columna);
void mostrarErroresSintacticos(void);
void mostrarErroresPorCategoria(void);
int hayErroresSintacticos(void);
int hayErroresSemanticos(void);
int hayErroresDeTipo(TipoErrorSintactico tipo);
void liberarErroresSintacticos(void);
const char* nombreTipoErrorSintactico(TipoErrorSintactico tipo);

void mostrarTablaUnificadaErrores(void);

#endif