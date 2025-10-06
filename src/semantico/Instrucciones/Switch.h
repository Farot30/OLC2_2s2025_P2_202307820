#ifndef SWITCH_H
#define SWITCH_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h"

typedef struct {
    NodoAST* expresion;
    NodoAST** casos; // Array of case nodes
    int num_casos;
    NodoAST* casoDefault; // Default case node
} SentenciaSwitch;

SentenciaSwitch* CrearSetenciaSwitch(NodoAST* expresion, NodoAST* casos[], int num_casos, NodoAST* casoDefault);
void LiberarSetenciaSwitch(SentenciaSwitch* setencia);

ResultadoEjecucion EjecturarNodoSwitch(NodoAST* nodo);


#endif