#include<stdio.h>
#include<stdlib.h>

#include "../../../tokenizer.h"
#include "ope02.h"

void opecode_02(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 02\n");
        exit(1);
    }
    fprintf(out_file, "    # PRINT\n");
    fprintf(out_file, "    pop rsi\n");
    fprintf(out_file, "    lea rdi, output_format[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call printf\n");
}