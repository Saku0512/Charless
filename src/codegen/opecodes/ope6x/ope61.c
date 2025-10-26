#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../tokenizer.h"
#include "ope61.h"

void opecode_61(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 61\n");
        exit(1);
    }
    fprintf(out_file, "    # ADD\n");
    // pop right into rbx, pop left into rax, compute rax = rax + rbx
    fprintf(out_file, "    pop rbx\n");
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    add rax, rbx\n");
    fprintf(out_file, "    push rax\n");
}