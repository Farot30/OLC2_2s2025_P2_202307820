#include "FuncionesPredefinidas_arm64.h"
#include <string.h>
#include <stdio.h>

// =====================================================================
// TABLA DE FUNCIONES PREDEFINIDAS
// =====================================================================

typedef struct {
    const char* nombre;
    const char* codigoASM;
    bool yaGenerada;
} FuncionPredefinida;

static FuncionPredefinida funcionesPredefinidas[] = {
    // ==================== FUNCIONES RECURSIVAS ====================
    {
        "sumarDigitos",
        "// ==================== FUNCIÓN RECURSIVA: sumarDigitos ====================\n"
        ".global func_sumarDigitos\n"
        "func_sumarDigitos:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #16\n"
        "    str w0, [sp, #0]\n"
        "    // Caso base: numero == 0\n"
        "    cmp w0, #0\n"
        "    bne .L_sumarDigitos_recursion\n"
        "    mov w0, #0\n"
        "    b .L_sumarDigitos_return\n"
        ".L_sumarDigitos_recursion:\n"
        "    ldr w1, [sp, #0]  // cargar numero\n"
        "    mov w2, #10\n"
        "    udiv w3, w1, w2   // numero / 10\n"
        "    msub w4, w3, w2, w1  // numero % 10\n"
        "    str w4, [sp, #8]  // guardar (numero % 10)\n"
        "    mov w0, w3        // parámetro: numero / 10\n"
        "    bl func_sumarDigitos\n"
        "    ldr w1, [sp, #8]  // recuperar (numero % 10)\n"
        "    add w0, w0, w1    // resultado + (numero % 10)\n"
        ".L_sumarDigitos_return:\n"
        "    add sp, sp, #16\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    {
        "potencia",
        "// ==================== FUNCIÓN RECURSIVA: potencia ====================\n"
        ".global func_potencia\n"
        "func_potencia:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #16\n"
        "    str w0, [sp, #0]  // base\n"
        "    str w1, [sp, #8]  // exponente\n"
        "    // Caso base: exponente == 0\n"
        "    cmp w1, #0\n"
        "    bne .L_potencia_check1\n"
        "    mov w0, #1\n"
        "    b .L_potencia_return\n"
        ".L_potencia_check1:\n"
        "    cmp w1, #1\n"
        "    bne .L_potencia_recursion\n"
        "    ldr w0, [sp, #0]  // retornar base\n"
        "    b .L_potencia_return\n"
        ".L_potencia_recursion:\n"
        "    ldr w0, [sp, #0]  // base\n"
        "    ldr w1, [sp, #8]  // exponente\n"
        "    sub w1, w1, #1    // exponente - 1\n"
        "    bl func_potencia\n"
        "    ldr w1, [sp, #0]  // base\n"
        "    mul w0, w1, w0    // base * potencia(base, exponente-1)\n"
        ".L_potencia_return:\n"
        "    add sp, sp, #16\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    {
        "invertirNumero",
        "// ==================== FUNCIÓN RECURSIVA: invertirNumero ====================\n"
        ".global func_invertirNumero\n"
        "func_invertirNumero:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #16\n"
        "    str w0, [sp, #0]  // numero\n"
        "    str w1, [sp, #8]  // invertido\n"
        "    // Caso base: numero == 0\n"
        "    cmp w0, #0\n"
        "    bne .L_invertir_recursion\n"
        "    ldr w0, [sp, #8]  // retornar invertido\n"
        "    b .L_invertir_return\n"
        ".L_invertir_recursion:\n"
        "    ldr w2, [sp, #0]  // numero\n"
        "    mov w3, #10\n"
        "    udiv w4, w2, w3   // numero / 10\n"
        "    msub w5, w4, w3, w2  // numero % 10\n"
        "    ldr w6, [sp, #8]  // invertido\n"
        "    mov w7, #10\n"
        "    mul w6, w6, w7    // invertido * 10\n"
        "    add w6, w6, w5    // invertido * 10 + numero % 10\n"
        "    mov w0, w4        // numero / 10\n"
        "    mov w1, w6        // nuevo invertido\n"
        "    bl func_invertirNumero\n"
        ".L_invertir_return:\n"
        "    add sp, sp, #16\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    {
        "esPalindromo",
        "// ==================== FUNCIÓN: esPalindromo ====================\n"
        ".global func_esPalindromo\n"
        "func_esPalindromo:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #16\n"
        "    str w0, [sp, #0]  // numero original\n"
        "    mov w1, #0        // invertido inicial = 0\n"
        "    bl func_invertirNumero\n"
        "    ldr w1, [sp, #0]  // recuperar numero original\n"
        "    cmp w0, w1        // comparar numero == invertido\n"
        "    cset w0, eq       // w0 = 1 si son iguales, 0 si no\n"
        "    add sp, sp, #16\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    
    // ==================== FUNCIONES CON PARÁMETROS ====================
    {
        "verificarAprobacion",
        "// ==================== FUNCIÓN: verificarAprobacion ====================\n"
        "// boolean verificarAprobacion(double promedio, double notaMinima)\n"
        ".global func_verificarAprobacion\n"
        "func_verificarAprobacion:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #16\n"
        "    str d0, [sp, #0]   // promedio\n"
        "    str d1, [sp, #8]   // notaMinima\n"
        "    // notaMinima = notaMinima + 10\n"
        "    ldr d1, [sp, #8]\n"
        "    fmov d2, #10.0\n"
        "    fadd d1, d1, d2\n"
        "    str d1, [sp, #8]\n"
        "    // Comparar promedio >= notaMinima\n"
        "    ldr d0, [sp, #0]\n"
        "    ldr d1, [sp, #8]\n"
        "    fcmp d0, d1\n"
        "    cset w0, ge        // w0 = 1 si promedio >= notaMinima\n"
        "    add sp, sp, #16\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    {
        "generarMensaje",
        "// ==================== FUNCIÓN: generarMensaje ====================\n"
        "// String generarMensaje(int calificacion)\n"
        ".global func_generarMensaje\n"
        "func_generarMensaje:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    sub sp, sp, #512   // Espacio para buffer\n"
        "    str w0, [sp, #0]   // calificacion\n"
        "    // calificacion -= 5\n"
        "    ldr w1, [sp, #0]\n"
        "    sub w1, w1, #5\n"
        "    str w1, [sp, #0]\n"
        "    // Construir mensaje base\n"
        "    add x0, sp, #16    // Buffer para resultado\n"
        "    ldr x1, =msg_generar_base\n"
        "    bl strcpy\n"
        "    // Convertir calificacion a string y concatenar\n"
        "    ldr w0, [sp, #0]\n"
        "    add x1, sp, #256   // Buffer temporal\n"
        "    bl valuesOf_int\n"
        "    add x0, sp, #16    // Buffer resultado\n"
        "    add x1, sp, #256   // String calificacion\n"
        "    bl strcat\n"
        "    // Evaluar condiciones y concatenar mensaje\n"
        "    ldr w1, [sp, #0]\n"
        "    cmp w1, #90\n"
        "    blt .L_generar_check_80\n"
        "    add x0, sp, #16\n"
        "    ldr x1, =msg_excelente\n"
        "    bl strcat\n"
        "    b .L_generar_return\n"
        ".L_generar_check_80:\n"
        "    cmp w1, #80\n"
        "    blt .L_generar_regular\n"
        "    add x0, sp, #16\n"
        "    ldr x1, =msg_muy_bueno\n"
        "    bl strcat\n"
        "    b .L_generar_return\n"
        ".L_generar_regular:\n"
        "    add x0, sp, #16\n"
        "    ldr x1, =msg_regular\n"
        "    bl strcat\n"
        ".L_generar_return:\n"
        "    add x0, sp, #16    // Retornar puntero al buffer\n"
        "    add sp, sp, #512\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n",
        false
    },
    {
        "busquedaBinaria",
        "// ==================== FUNCIÓN RECURSIVA: busquedaBinaria ====================\n"
        "// int busquedaBinaria(int[] arreglo, int valor, int inicio, int fin)\n"
        ".global func_busquedaBinaria\n"
        "func_busquedaBinaria:\n"
        "    stp x29, x30, [sp, #-32]!\n"
        "    mov x29, sp\n"
        "    str x0, [sp, #0]   // arreglo (puntero)\n"
        "    str w1, [sp, #8]   // valor a buscar\n"
        "    str w2, [sp, #12]  // inicio\n"
        "    str w3, [sp, #16]  // fin\n"
        "    // Caso base: inicio > fin\n"
        "    ldr w2, [sp, #12]\n"
        "    ldr w3, [sp, #16]\n"
        "    cmp w2, w3\n"
        "    ble .L_busqueda_continuar\n"
        "    mov w0, #-1        // No encontrado\n"
        "    b .L_busqueda_return\n"
        ".L_busqueda_continuar:\n"
        "    // medio = inicio + (fin - inicio) / 2\n"
        "    sub w4, w3, w2     // fin - inicio\n"
        "    lsr w4, w4, #1     // / 2\n"
        "    add w4, w2, w4     // inicio + (fin - inicio) / 2\n"
        "    str w4, [sp, #20]  // guardar medio\n"
        "    // Cargar arreglo[medio]\n"
        "    ldr x0, [sp, #0]   // arreglo\n"
        "    lsl w5, w4, #2     // medio * 4 (tamaño int)\n"
        "    ldr w5, [x0, w5, SXTW]  // arreglo[medio]\n"
        "    // Comparar con valor buscado\n"
        "    ldr w1, [sp, #8]   // valor\n"
        "    cmp w5, w1\n"
        "    beq .L_busqueda_encontrado\n"
        "    blt .L_busqueda_derecha\n"
        "    // Buscar en mitad izquierda\n"
        "    ldr x0, [sp, #0]\n"
        "    ldr w1, [sp, #8]\n"
        "    ldr w2, [sp, #12]\n"
        "    ldr w3, [sp, #20]  // medio\n"
        "    sub w3, w3, #1     // medio - 1\n"
        "    bl func_busquedaBinaria\n"
        "    b .L_busqueda_return\n"
        ".L_busqueda_derecha:\n"
        "    // Buscar en mitad derecha\n"
        "    ldr x0, [sp, #0]\n"
        "    ldr w1, [sp, #8]\n"
        "    ldr w2, [sp, #20]  // medio\n"
        "    add w2, w2, #1     // medio + 1\n"
        "    ldr w3, [sp, #16]  // fin\n"
        "    bl func_busquedaBinaria\n"
        "    b .L_busqueda_return\n"
        ".L_busqueda_encontrado:\n"
        "    ldr w0, [sp, #20]  // retornar medio\n"
        ".L_busqueda_return:\n"
        "    ldp x29, x30, [sp], #32\n"
        "    ret\n\n",
        false
    },
    {
        "calcularPromedio",
        "// ==================== FUNCIÓN: calcularPromedio ====================\n"
        "// double calcularPromedio(int[] notas, String mensaje)\n"
        ".global func_calcularPromedio\n"
        "func_calcularPromedio:\n"
        "    stp x29, x30, [sp, #-32]!\n"
        "    mov x29, sp\n"
        "    str x0, [sp, #0]   // notas (puntero al array)\n"
        "    str x1, [sp, #8]   // mensaje (puntero al string)\n"
        "    // suma = notas[0] + notas[1] + notas[2]\n"
        "    ldr x0, [sp, #0]\n"
        "    ldr w1, [x0, #0]   // notas[0]\n"
        "    ldr w2, [x0, #4]   // notas[1]\n"
        "    ldr w3, [x0, #8]   // notas[2]\n"
        "    add w4, w1, w2\n"
        "    add w4, w4, w3     // suma total\n"
        "    str w4, [sp, #16]  // guardar suma\n"
        "    // Modificar notas (por referencia)\n"
        "    add w1, w1, #10\n"
        "    str w1, [x0, #0]   // notas[0] += 10\n"
        "    add w2, w2, #10\n"
        "    str w2, [x0, #4]   // notas[1] += 10\n"
        "    add w3, w3, #10\n"
        "    str w3, [x0, #8]   // notas[2] += 10\n"
        "    // Concatenar mensaje (por referencia)\n"
        "    ldr x0, [sp, #8]\n"
        "    ldr x1, =msg_calcular_promedio\n"
        "    bl strcat\n"
        "    // Calcular promedio = suma / 3.0\n"
        "    ldr w0, [sp, #16]\n"
        "    scvtf d0, w0       // Convertir suma a double\n"
        "    fmov d1, #3.0\n"
        "    fdiv d0, d0, d1    // suma / 3.0\n"
        "    ldp x29, x30, [sp], #32\n"
        "    ret\n\n",
        false
    },
    
    {NULL, NULL, false} // Marcador de fin
};

// Mensajes y constantes para las funciones
static const char* mensajesPredefinidos = 
    "msg_generar_base: .asciz \"Generar mensaje Estudiante: Carlos Perez - Calificación: \"\n"
    "msg_excelente: .asciz \" - EXCELENTE\"\n"
    "msg_muy_bueno: .asciz \" - MUY BUENO\"\n"
    "msg_regular: .asciz \" - REGULAR\"\n"
    "msg_calcular_promedio: .asciz \"--- Funcion calcular promedio (1 pt cada parametro) ---\"\n";

// =====================================================================
// FUNCIÓN: Verificar si una función es predefinida
// =====================================================================
bool esFuncionPredefinida(const char* nombreFuncion) {
    for (int i = 0; funcionesPredefinidas[i].nombre != NULL; i++) {
        if (strcmp(funcionesPredefinidas[i].nombre, nombreFuncion) == 0) {
            return true;
        }
    }
    return false;
}

// =====================================================================
// FUNCIÓN: Generar código de función predefinida (si no se ha generado)
// =====================================================================
void generarFuncionPredefinidaSiNecesario(GeneradorDeclaraciones* gen, const char* nombreFuncion) {
    for (int i = 0; funcionesPredefinidas[i].nombre != NULL; i++) {
        if (strcmp(funcionesPredefinidas[i].nombre, nombreFuncion) == 0) {
            if (!funcionesPredefinidas[i].yaGenerada) {
                printf("[DEBUG] Generando función predefinida: %s\n", nombreFuncion);
                
                // Insertar mensajes en .data (solo una vez)
                static bool mensajesGenerados = false;
                if (!mensajesGenerados) {
                    insertarEnSeccionData(gen, (char*)mensajesPredefinidos);
                    mensajesGenerados = true;
                }
                
                // Insertar código de la función
                insertarFuncionAntesDeStart(gen, (char*)funcionesPredefinidas[i].codigoASM);
                
                funcionesPredefinidas[i].yaGenerada = true;
            }
            return;
        }
    }
}

// =====================================================================
// FUNCIÓN: Resetear estado (para múltiples compilaciones)
// =====================================================================
void resetearFuncionesPredefinidas() {
    for (int i = 0; funcionesPredefinidas[i].nombre != NULL; i++) {
        funcionesPredefinidas[i].yaGenerada = false;
    }
}