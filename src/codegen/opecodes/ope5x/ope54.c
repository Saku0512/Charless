#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../tokenizer.h"
#include "../../../vm.h"
#include "ope54.h"

void opecode_54(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 54\n");
        exit(1);
    }
    if (strncmp(ip, "99", 2) == 0) {
        ip += 2;
        long var_id = get_number_literal();
        fprintf(out_file, "    # LOAD %ld\n", var_id);
        fprintf(out_file, "    lea rbx, memory[rip]\n");
        fprintf(out_file, "    mov rax, [rbx + %ld]\n", var_id * 8);
        fprintf(out_file, "    push rax\n");
    } else {
        fprintf(stderr, "Error: LOAD (54) requires '99' prefix\n");
        exit(1);
    }
}