#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../tokenizer.h"
#include "../../../vm.h"
#include "ope53.h"

void opecode_53(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after 510\n");
        exit(1);
    }
    if (strncmp(ip, "990", 3) == 0) {
        ip += 3;
        long var_id = get_number_literal();
        fprintf(out_file, "    # STORE %ld\n", var_id);
        fprintf(out_file, "    pop rax\n");
        fprintf(out_file, "    lea rbx, memory[rip]\n");
        fprintf(out_file, "    mov [rbx + %ld], rax\n", var_id * 8);
    } else {
        fprintf(stderr, "Error: STORE (510) requires '990' prefix\n");
        exit(1);
    }
}