#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope21.h"

void opecode_21(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 21\n");
        exit(1);
    }
    fprintf(out_file, "    # PRINT_NEWLINE\n");
    fprintf(out_file, "    lea rdi, newline[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call printf\n");
}

