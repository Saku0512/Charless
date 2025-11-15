#include <stdio.h>
#include <stdlib.h>
#include "ope71.h"
#include "../../../tokenizer.h"

void opecode_71(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 71\n");
        exit(1);
    }
    long address = get_number_literal();
    fprintf(out_file, "    # JUMP to %ld\n", address);
    fprintf(out_file, "    jmp L%ld\n", address);
}

