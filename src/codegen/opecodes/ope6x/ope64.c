#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope64.h"

void opecode_64(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 64\n");
        exit(1);
    }
    fprintf(out_file, "    # DIV\n");
    // pop divisor into rbx, pop dividend into rax, perform signed divide
    fprintf(out_file, "    pop rbx\n");
    fprintf(out_file, "    pop rax\n");
    // sign-extend rax into rdx:rax, then idiv rbx -> quotient in rax
    fprintf(out_file, "    cqo\n");
    fprintf(out_file, "    idiv rbx\n");
    fprintf(out_file, "    push rax\n");
}