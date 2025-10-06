#include <stdio.h>
#include "semantico/semantico.h"
#include "semantico/tabla_simbolo.h"
#include "AST/ast.h"
#include "AST/err.h"

int yyparse(void);

int main(void) {
    initErrores();
    inicializarTabla();
    printf("bienvenido al parser ctrl + D\n");
    if (yyparse() == 0) {
        if (error_semantico || hayErrores()) {
            printf("Hubo errores.\n");
            mostrarErrores();
        } else {
            printf("Cadena válida\n");
        }
        mostrarTablaSimbolos();   
    }
 
    liberarTabla();
    mostrarErrores(); 
    liberarErrores();
    return 0;
}