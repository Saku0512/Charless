#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope63.h"

void opecode_63(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 63\n");
        exit(1);
    }
    fprintf(out_file, "    # MUL\n");
    // pop right into rbx, pop left into rax, compute rax = rax * rbx
    fprintf(out_file, "    pop rbx\n");
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    imul rax, rbx\n");
    fprintf(out_file, "    push rax\n");
}