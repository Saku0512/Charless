#include<stdio.h>

#include "ope00.h"

void opecode_00(FILE *out_file) {
    fprintf(out_file, "    # HALT\n");
    fprintf(out_file, "    mov rsp, rbp\n");
    fprintf(out_file, "    pop rbp\n");
    fprintf(out_file, "    ret\n");
}