#include "parserApi.h"
#include "../semantico/semantico.h"
#include "../semantico/tabla_simbolo.h"
#include "../simbolos/salida.h"
#include "../AST/err.h"
#include "../AST/errorTIpos.h"
#include "../AST/ejecutar_ast.h"
#include <unistd.h>      
#include <sys/types.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern NodoAST* astGlobal;
// Declaraciones externas del lexer/parser
int yyparse(void);
void yyrestart(FILE*);
extern int yylineno;
extern int yycolumn;
extern int error_semantico;
extern struct yy_buffer_state* yy_scan_string(const char*);
extern void yy_delete_buffer(struct yy_buffer_state*);

static char* duptext(const char* s){
    if(!s) return NULL;
    size_t n=strlen(s);
    char* p=malloc(n+1);
    if(p) memcpy(p,s,n+1);
    return p;
}

// Captura de stdout temporal
static char* capture_symbols(){
    fflush(stdout);
    int pipefd[2];
    if(pipe(pipefd)!=0) return duptext("");
    int saved = dup(1);
    dup2(pipefd[1],1);
    mostrarTablaSimbolos();
    fflush(stdout);
    close(pipefd[1]);
    dup2(saved,1);
    close(saved);
    
    char buf[4096];
    size_t total=0;
    char* out=NULL;
    ssize_t r;
    while((r=read(pipefd[0], buf, sizeof(buf)))>0){
        char* tmp=realloc(out,total+r+1);
        if(!tmp){ free(out); close(pipefd[0]); return duptext(""); }
        out=tmp;
        memcpy(out+total, buf, r);
        total+=r;
        out[total]=0;
    }
    close(pipefd[0]);
    if(!out) return duptext("");
    return out;
}

static char* capture_errors(void) __attribute__((unused));

static char* capture_errors(){
    fflush(stdout);
    int pipefd[2];
    if(pipe(pipefd)!=0) return duptext("");
    int saved = dup(1);
    dup2(pipefd[1],1);
    mostrarErrores();
    fflush(stdout);
    close(pipefd[1]);
    dup2(saved,1);
    close(saved);
    char buf[4096];
    size_t total=0;
    char* out=NULL;
    ssize_t r;
    while((r=read(pipefd[0], buf, sizeof(buf)))>0){
        char* tmp=realloc(out,total+r+1);
        if(!tmp){ free(out); close(pipefd[0]); return duptext(""); }
        out=tmp;
        memcpy(out+total, buf, r);
        total+=r;
        out[total]=0;
    }
    close(pipefd[0]);
    if(!out) return duptext("");
    return out;
}

static char* capture_unified_errors(){
    fflush(stdout);
    int pipefd[2];
    if(pipe(pipefd)!=0) return duptext("");
    int saved = dup(1);
    dup2(pipefd[1],1);
    mostrarTablaUnificadaErrores();  // Función de errorTipos.c
    fflush(stdout);
    close(pipefd[1]);
    dup2(saved,1);
    close(saved);
    
    char buf[4096];
    size_t total=0;
    char* out=NULL;
    ssize_t r;
    while((r=read(pipefd[0], buf, sizeof(buf)))>0){
        char* tmp=realloc(out,total+r+1);
        if(!tmp){ free(out); close(pipefd[0]); return duptext(""); }
        out=tmp;
        memcpy(out+total, buf, r);
        total+=r;
        out[total]=0;
    }
    close(pipefd[0]);
    if(!out) return duptext("");
    return out;
}



ParseResult parse_source(const char* code){
    ParseResult res = {0};
    initErrores();
    initErroresSintacticos(); 
    inicializarTabla();
    error_semantico = 0;
    yylineno = 1;
    yycolumn = 1;

    clear_print_output();

    struct yy_buffer_state* buf = yy_scan_string(code);
    yyparse();
    yy_delete_buffer(buf);

    if (astGlobal) {
        printf("\n=== ESTRUCTURA DEL AST ===\n");
        imprimirAST(astGlobal, 0);
        interpretarPrograma(astGlobal);
        printf("ejecutando exportarAST\n");
        exportarAST(astGlobal, "ast.dot", "ast.png");
        printf("\n=== LIBERANDO MEMORIA ===\n");
        liberarAST(astGlobal);
        printf("[DEBUG] Procesamiento completado.\n");
    }

    // Siempre leer el AST generado, tenga errores o no
    FILE* f = fopen("ast.dot", "r");
    if(f){
        fseek(f, 0, SEEK_END);
        long n = ftell(f);
        fseek(f, 0, SEEK_SET);
        res.astDot = malloc(n+1);
        if(res.astDot){
            fread(res.astDot, 1, n, f);
            res.astDot[n] = 0;
        }
        fclose(f);
    } else {
        res.astDot = duptext("digraph AST { }");
    }

    res.simbolos = capture_symbols();
    res.errores = capture_unified_errors();
    res.salida = get_captured_output();
    
    // Verificar si hay errores solo para el campo ok
    int hayErrores = error_semantico || hayErroresSintacticos() || hayErroresSemanticos();
    res.ok = !hayErrores;

    liberarTabla();
    liberarErrores();
    liberarErroresSintacticos();
    return res;
}

void free_parse_result(ParseResult* r){
    if(!r) return;
    free(r->astDot);
    free(r->simbolos);
    free(r->errores);
    free(r->salida);
    r->astDot = r->simbolos = r->errores = r->salida = NULL;
}