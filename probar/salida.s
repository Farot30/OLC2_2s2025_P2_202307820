.global _start
.extern itoa
.extern printf
.extern sprintf

.data
buffer: .space 64
fmt:    .asciz "Número: %d\n"

.text
_start:
    ldr x0, =buffer     // Destino
    ldr x1, =fmt        // Formato  
    mov w2, #123        // Número
    bl sprintf          // Convierte
    ldr x0, =buffer     // Carga resultado
    bl printf           // Imprime

    mov w0, #0          // Código de salida
    mov x8, #93         // Número de syscall para exit
    svc #0              // Llama al sistema para salir