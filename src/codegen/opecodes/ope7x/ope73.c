#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ope73.h"
#include "../../../tokenizer.h"
#include "../../../vm.h"

void opecode_73(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after 702\n");
        exit(1);
    }
    if (strncmp(ip, "990", 3) == 0) {
        ip += 3;
    }
    long address = get_number_literal();
    fprintf(out_file, "    # JNZ to %ld\n", address);
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    cmp rax, 0\n");
    fprintf(out_file, "    jne L%ld\n", address);
}

