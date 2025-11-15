#include <stdio.h>
#include <stdlib.h>
#include "ope82.h"
#include "../../../tokenizer.h"

void opecode_82(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 82\n");
        exit(1);
    }
    fprintf(out_file, "    # EQN (Not Equal Number)\n");
    fprintf(out_file, "    pop rbx      # Pop second operand\n");
    fprintf(out_file, "    pop rax      # Pop first operand\n");
    fprintf(out_file, "    cmp rax, rbx\n");
    fprintf(out_file, "    mov rax, 0   # Zero out rax before setting flag\n");
    fprintf(out_file, "    setne al     # Set al to 1 if they are not equal\n");
    fprintf(out_file, "    push rax\n");
}

