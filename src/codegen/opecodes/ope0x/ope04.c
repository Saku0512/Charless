#include<stdio.h>

#include "../../../tokenizer.h"
#include "ope04.h"

void opecode_04(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 04\n");
        exit(1);
    }
    fprintf(out_file, "    # PRINT_CHAR\n");
    fprintf(out_file, "    pop rsi\n");
    fprintf(out_file, "    lea rdi, char_format[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call printf\n");
}