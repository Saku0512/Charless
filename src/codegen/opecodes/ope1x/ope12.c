#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope12.h"

void opecode_12(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 12\n");
        exit(1);
    }
    fprintf(out_file, "    # INPUT_NUM\n");
    fprintf(out_file, "    lea rsi, input_buffer[rip]\n");
    fprintf(out_file, "    lea rdi, input_format_num[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call scanf\n");
    fprintf(out_file, "    lea rbx, input_buffer[rip]\n");
    fprintf(out_file, "    mov rax, [rbx]\n");
    fprintf(out_file, "    push rax\n");
}