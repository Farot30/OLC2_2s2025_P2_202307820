CC = gcc
BISON = bison
FLEX = flex

SRC_DIR = src
PARSER_DIR = $(SRC_DIR)/parser
AST_DIR = $(SRC_DIR)/AST

PKG_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
PKG_LIBS   := $(shell pkg-config --libs gtk+-3.0)

CFLAGS = -Wall -Wextra -std=c11 -Isrc -D_POSIX_C_SOURCE=200809L
LEXLIB = -lfl
SYSTEM_LDFLAGS = -Wl,-rpath,/lib/x86_64-linux-gnu -Wl,-rpath,/usr/lib/x86_64-linux-gnu

# Fuentes backend (parser consola)
BACKEND_SRC = \
	$(SRC_DIR)/main.c \
	$(PARSER_DIR)/parser.tab.c \
	$(PARSER_DIR)/lexer.c \
	$(AST_DIR)/ast.c \
	$(SRC_DIR)/AST/ejecutar_ast.c \
	$(SRC_DIR)/AST/err.c \
	$(SRC_DIR)/AST/errorTipos.c \
	$(SRC_DIR)/semantico/semantico.c \
	$(SRC_DIR)/semantico/tipos.c \
	$(SRC_DIR)/semantico/tabla_simbolo.c \
	$(SRC_DIR)/semantico/Expresiones/aritmeticas.c \
	$(SRC_DIR)/semantico/Instrucciones/declaracion.c \
	$(SRC_DIR)/semantico/Instrucciones/asignacion.c \
	$(SRC_DIR)/semantico/Instrucciones/Print.c \
	$(SRC_DIR)/semantico/Instrucciones/SentIF.c \
	$(SRC_DIR)/semantico/Instrucciones/Switch.c \
	$(SRC_DIR)/semantico/Instrucciones/While.c \
	$(SRC_DIR)/semantico/Instrucciones/break.c \
	$(SRC_DIR)/semantico/Instrucciones/continue.c \
	$(SRC_DIR)/semantico/Instrucciones/for.c \
	$(SRC_DIR)/semantico/Instrucciones/for_vector.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_vector.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_matriz.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_array.c \
	$(SRC_DIR)/semantico/Vectores/acceso_vector.c \
	$(SRC_DIR)/semantico/Vectores/acceso_matriz.c \
	$(SRC_DIR)/semantico/Vectores/acceso_arrays.c \
	$(SRC_DIR)/simbolos/Evaluador.c \
	$(SRC_DIR)/simbolos/EvaluadorArray.c \
	$(SRC_DIR)/semantico/Embebidas/EvaluadoresArrays.c \
	$(SRC_DIR)/semantico/Embebidas/evaluarString.c \
	$(SRC_DIR)/semantico/Embebidas/EvaluadorNumerico.c \
	$(SRC_DIR)/semantico/Funciones/LlamadasVoid.c \
	$(SRC_DIR)/semantico/Funciones/FuncionVoid.c \
	$(SRC_DIR)/semantico/Funciones/Llamadas.c \
	$(SRC_DIR)/semantico/Funciones/Funcion.c \
	$(SRC_DIR)/semantico/Instrucciones/return.c \
	$(SRC_DIR)/simbolos/salida.c \
	$(SRC_DIR)/simbolos/Operador.c \
	$(SRC_DIR)/Generacion_arm64/generador_arm64.c \
	$(SRC_DIR)/Generacion_arm64/registros_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/declaraciones_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/declaracionConst_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Operadores_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Asignacion_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/StringBuilder.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Evaluadores_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/declaracionesVec_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Impresion_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/If_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Switch_arm64.c \
	$(SRC_DIR)/Generacion_arm64/EscapeSecuencia_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/for_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/while_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Break_continue_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/String_valueOf.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/EvaluadorExpr_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/Embebidas_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/AccesoVector_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/EscrituraVector_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/ForEach_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Do_While_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/Parseos_arm64.c \
	

# Fuentes GUI
GUI_SRC = \
	$(SRC_DIR)/gtks/main_gtk.c \
	$(SRC_DIR)/gtks/Llama.c \
	$(SRC_DIR)/gtks/Gtk.c \
	$(SRC_DIR)/frontend/parserApi.c \
	$(PARSER_DIR)/parser.tab.c \
	$(PARSER_DIR)/lexer.c \
	$(AST_DIR)/ast.c \
	$(SRC_DIR)/AST/ejecutar_ast.c \
	$(SRC_DIR)/AST/err.c \
	$(SRC_DIR)/AST/errorTipos.c \
	$(SRC_DIR)/semantico/semantico.c \
	$(SRC_DIR)/semantico/tipos.c \
	$(SRC_DIR)/semantico/tabla_simbolo.c \
	$(SRC_DIR)/semantico/Expresiones/aritmeticas.c \
	$(SRC_DIR)/semantico/Instrucciones/declaracion.c \
	$(SRC_DIR)/semantico/Instrucciones/asignacion.c \
	$(SRC_DIR)/semantico/Instrucciones/Print.c \
	$(SRC_DIR)/semantico/Instrucciones/SentIF.c \
	$(SRC_DIR)/semantico/Instrucciones/Switch.c \
	$(SRC_DIR)/semantico/Instrucciones/While.c \
	$(SRC_DIR)/semantico/Instrucciones/break.c \
	$(SRC_DIR)/semantico/Instrucciones/continue.c \
	$(SRC_DIR)/semantico/Instrucciones/for.c \
	$(SRC_DIR)/semantico/Instrucciones/for_vector.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_vector.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_matriz.c \
	$(SRC_DIR)/semantico/Vectores/declaracion_array.c \
	$(SRC_DIR)/semantico/Vectores/acceso_vector.c \
	$(SRC_DIR)/semantico/Vectores/acceso_matriz.c \
	$(SRC_DIR)/semantico/Vectores/acceso_arrays.c \
	$(SRC_DIR)/simbolos/Evaluador.c \
	$(SRC_DIR)/simbolos/EvaluadorArray.c \
	$(SRC_DIR)/semantico/Embebidas/EvaluadoresArrays.c \
	$(SRC_DIR)/semantico/Embebidas/evaluarString.c \
	$(SRC_DIR)/semantico/Embebidas/EvaluadorNumerico.c \
	$(SRC_DIR)/semantico/Funciones/LlamadasVoid.c \
	$(SRC_DIR)/semantico/Funciones/FuncionVoid.c \
	$(SRC_DIR)/semantico/Funciones/Llamadas.c \
	$(SRC_DIR)/semantico/Funciones/Funcion.c \
	$(SRC_DIR)/semantico/Instrucciones/return.c \
	$(SRC_DIR)/simbolos/Operador.c \
	$(SRC_DIR)/simbolos/salida.c \
	$(SRC_DIR)/Generacion_arm64/generador_arm64.c \
	$(SRC_DIR)/Generacion_arm64/registros_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/declaraciones_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/declaracionConst_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Operadores_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Asignacion_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/StringBuilder.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesNorm_arm64/Evaluadores_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/declaracionesVec_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Impresion_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/If_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Switch_arm64.c \
	$(SRC_DIR)/Generacion_arm64/EscapeSecuencia_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/for_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/while_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Break_continue_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/String_valueOf.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/EvaluadorExpr_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/Embebidas_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/AccesoVector_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Vectores_arm64/EscrituraVector_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/ForEach_arm64.c \
	$(SRC_DIR)/Generacion_arm64/InstruccionesSent_arm64/Do_While_arm64.c \
	$(SRC_DIR)/Generacion_arm64/Embebidas/Parseos_arm64.c \


all: parser gui

parser: $(BACKEND_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LEXLIB) -lm

gui: $(GUI_SRC)
	$(CC) $(CFLAGS) $(PKG_CFLAGS) $(SYSTEM_LDFLAGS) -o $@ $^ $(PKG_LIBS) $(LEXLIB) -lm

$(PARSER_DIR)/parser.tab.c $(PARSER_DIR)/parser.tab.h: $(PARSER_DIR)/parser.y
	$(BISON) -d -o $(PARSER_DIR)/parser.tab.c $(PARSER_DIR)/parser.y

$(PARSER_DIR)/lexer.c: $(PARSER_DIR)/lexer.l $(PARSER_DIR)/parser.tab.h
	$(FLEX) -o $(PARSER_DIR)/lexer.c $(PARSER_DIR)/lexer.l

clean:
	rm -f parser gui \
	$(PARSER_DIR)/parser.tab.c $(PARSER_DIR)/parser.tab.h \
	$(PARSER_DIR)/lexer.c ast.png ast.dot

.PHONY: all clean