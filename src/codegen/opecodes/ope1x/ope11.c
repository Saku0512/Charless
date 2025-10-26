#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../tokenizer.h"
#include "ope11.h"

void opecode_11(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 11\n");
        exit(1);
    }
    fprintf(out_file, "    # INPUT_STRING_AND_PARSE\n");
    fprintf(out_file, "    # Step 1: Call scanf to get user input\n");
    fprintf(out_file, "    lea rsi, input_buffer[rip]\n");
    fprintf(out_file, "    lea rdi, input_format_str[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call scanf\n");
    fprintf(out_file, "    # Step 2: Parse the string from the buffer\n");
    fprintf(out_file, "    lea rdi, input_buffer[rip]\n");
    fprintf(out_file, "    xor rax, rax\n");
    fprintf(out_file, "    xor rbx, rbx\n");
    fprintf(out_file, "    mov bl, [rdi]\n");
    fprintf(out_file, "    sub bl, '0'\n");
    fprintf(out_file, "    inc rdi\n");
    fprintf(out_file, "    mov rcx, rbx\n");
    fprintf(out_file, "    cmp rcx, 0\n");
    fprintf(out_file, "    je .parse_loop_11_end\n");
    fprintf(out_file, ".parse_loop_11:\n");
    fprintf(out_file, "    movzx rdx, byte ptr [rdi]\n");
    fprintf(out_file, "    sub rdx, '0'\n");
    fprintf(out_file, "    imul rax, 10\n");
    fprintf(out_file, "    add rax, rdx\n");
    fprintf(out_file, "    loop .parse_loop_11\n");
    fprintf(out_file, ".parse_loop_11_end:\n");
    fprintf(out_file, "    push rax\n");
}