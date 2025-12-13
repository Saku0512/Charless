#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ope72.h"
#include "../../../tokenizer.h"
#include "../../../vm.h"

void opecode_72(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after 701\n");
        exit(1);
    }
    if (strncmp(ip, "990", 3) == 0) {
        ip += 3;
    }
    long address = get_number_literal();
    fprintf(out_file, "    # JZ to %ld\n", address);
    fprintf(out_file, "    pop rax\n");
    fprintf(out_file, "    cmp rax, 0\n");
    fprintf(out_file, "    je L%ld\n", address);
}
