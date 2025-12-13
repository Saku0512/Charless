#include<stdio.h>

#include "../../../tokenizer.h"
#include "ope00.h"

void opecode_00(FILE *out_file) {
    // HALT命令の後のセパレータを消費（存在する場合）
    consume_separator();
    fprintf(out_file, "    # HALT\n");
    fprintf(out_file, "    mov rax, 0\n");
    fprintf(out_file, "    mov rsp, rbp\n");
    fprintf(out_file, "    pop rbp\n");
    fprintf(out_file, "    ret\n");
}