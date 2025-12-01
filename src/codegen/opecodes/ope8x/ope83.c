#include <stdio.h>
#include <stdlib.h>
#include "ope83.h"
#include "../../../tokenizer.h"

void opecode_83(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 83\n");
        exit(1);
    }
    fprintf(out_file, "    # GT (Greater Than)\n");
    fprintf(out_file, "    pop rax      # Pop right operand\n");
    fprintf(out_file, "    pop rbx      # Pop left operand\n");
    fprintf(out_file, "    cmp rbx, rax\n");
    fprintf(out_file, "    mov rax, 0\n");
    fprintf(out_file, "    setg al      # Set al to 1 if left > right\n");
    fprintf(out_file, "    push rax\n");
}

