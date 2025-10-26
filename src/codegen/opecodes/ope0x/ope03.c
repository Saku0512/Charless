#include<stdio.h>

#include "../../../tokenizer.h"
#include "ope03.h"

void opecode_03(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 03\n");
        exit(1);
    }
    fprintf(out_file, "    # PRINT_MEM_STRING\n");
    fprintf(out_file, "    pop rsi\n");
    fprintf(out_file, "    lea rdi, string_output_format[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call printf\n");
}