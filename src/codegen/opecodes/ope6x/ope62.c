#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope62.h"

void opecode_62(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 62\n");
        exit(1);
    }
    fprintf(out_file, "    # SUB\n");
    // pop right into rbx, pop left into rax, compute rax = rax - rbx
    fprintf(out_file, "    pop rbx\n");
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    sub rax, rbx\n");
    fprintf(out_file, "    push rax\n");
}