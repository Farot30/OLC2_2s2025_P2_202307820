#include "salida.h"
#include <stdlib.h>
#include <string.h>

static char* program_output = NULL;
static size_t program_output_len = 0;

void capture_print_output(const char* text) {
    if (!text) return;
    size_t text_len = strlen(text);
    size_t new_len = program_output_len + text_len + 1;
    
    program_output = realloc(program_output, new_len);
    if (program_output) {
        if (program_output_len == 0) {
            program_output[0] = '\0';
        }
        strcat(program_output, text);
        program_output_len = new_len - 1;
    }
}

void clear_print_output(void) {
    if (program_output) {
        free(program_output);
        program_output = NULL;
        program_output_len = 0;
    }
}

char* get_captured_output(void) {
    return program_output ? strdup(program_output) : strdup("");
}