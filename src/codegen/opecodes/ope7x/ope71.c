#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ope71.h"
#include "../../../tokenizer.h"
#include "../../../vm.h"

void opecode_71(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after 700\n");
        exit(1);
    }
    if (strncmp(ip, "990", 3) == 0) {
        ip += 3;
    }
    long address = get_number_literal();
    fprintf(out_file, "    # JUMP to %ld\n", address);
    fprintf(out_file, "    jmp L%ld\n", address);
}

