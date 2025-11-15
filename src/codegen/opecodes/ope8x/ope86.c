#include <stdio.h>
#include <stdlib.h>
#include "ope86.h"
#include "../../../tokenizer.h"

void opecode_86(FILE *out_file) {
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 86\n");
        exit(1);
    }
    fprintf(out_file, "    # LTE (Less Than or Equal)\n");
    fprintf(out_file, "    pop rax      # Pop right operand\n");
    fprintf(out_file, "    pop rbx      # Pop left operand\n");
    fprintf(out_file, "    cmp rbx, rax\n");
    fprintf(out_file, "    mov rax, 0\n");
    fprintf(out_file, "    setle al     # Set al to 1 if left <= right\n");
    fprintf(out_file, "    push rax\n");
}
