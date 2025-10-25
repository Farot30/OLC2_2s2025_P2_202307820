// Implementación de parse_float en ARM64 puro
// Convierte un string a float
// Entrada: x0 = puntero al string
// Salida: s0 = valor float

.global parse_float
.extern parse_double
.text

parse_float:
    // Guardar registros
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    
    // Llamar a parse_double (reutilizar la lógica)
    bl parse_double         // Resultado en d0
    
    // Convertir double a float
    fcvt s0, d0
    
    // Restaurar registros y retornar
    ldp x29, x30, [sp], #16
    ret
