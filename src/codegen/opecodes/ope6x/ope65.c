#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope65.h"

void opecode_65(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 65\n");
        exit(1);
    }
    fprintf(out_file, "    # MOD\n");
    // pop divisor into rbx, pop dividend into rax, perform idiv -> remainder in rdx
    fprintf(out_file, "    pop rbx\n");
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    cqo\n");
    fprintf(out_file, "    idiv rbx\n");
    fprintf(out_file, "    push rdx\n");
}