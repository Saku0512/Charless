#include <stdio.h>
#include <stdlib.h>

#include "../../../tokenizer.h"
#include "ope13.h"

void opecode_13(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 13\n");
        exit(1);
    }
    fprintf(out_file, "    # INPUT_STRING_AND_PARSE\n");
    fprintf(out_file, "    # Step 1: Call scanf to get user input\n");
    fprintf(out_file, "    lea rdi, input_format_str[rip]\n");
    fprintf(out_file, "    lea rsi, input_buffer[rip]\n");
    fprintf(out_file, "    mov al, 0\n");
    fprintf(out_file, "    call scanf\n\n");

    fprintf(out_file, "    # Step 2: Prepare for parsing\n");
    fprintf(out_file, "    lea rsi, input_buffer[rip]\n");
    fprintf(out_file, "    lea rdi, string_build_buffer[rip]\n");
    fprintf(out_file, "    mov r10, rdi\n\n");

    fprintf(out_file, ".parse_loop_start_13:\n");
    fprintf(out_file, "    movzx rcx, byte ptr [rsi]\n");
    fprintf(out_file, "    cmp rcx, 0\n");
    fprintf(out_file, "    je .parse_loop_end_13\n");
    fprintf(out_file, "    sub rcx, '0'\n");
    fprintf(out_file, "    inc rsi\n");
    fprintf(out_file, "    xor rax, rax\n\n");

    fprintf(out_file, ".parse_char_loop_13:\n");
    fprintf(out_file, "    cmp rcx, 0\n");
    fprintf(out_file, "    je .parse_char_loop_end_13\n");
    fprintf(out_file, "    movzx rbx, byte ptr [rsi]\n");
    fprintf(out_file, "    sub rbx, '0'\n");
    fprintf(out_file, "    imul rax, 10\n");
    fprintf(out_file, "    add rax, rbx\n");
    fprintf(out_file, "    inc rsi\n");
    fprintf(out_file, "    dec rcx\n");
    fprintf(out_file, "    jmp .parse_char_loop_13\n\n");

    fprintf(out_file, ".parse_char_loop_end_13:\n");
    fprintf(out_file, "    mov [rdi], al\n");
    fprintf(out_file, "    inc rdi\n");
    fprintf(out_file, "    jmp .parse_loop_start_13\n\n");

    fprintf(out_file, ".parse_loop_end_13:\n");
    fprintf(out_file, "    mov byte ptr [rdi], 0\n");
    fprintf(out_file, "    push r10\n");
}