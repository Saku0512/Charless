#include<stdio.h>

#include "../vm.h"
#include "asm_header.h"

void asm_header(FILE *out_file) {
    fprintf(out_file, ".intel_syntax noprefix\n");
    fprintf(out_file, ".globl main\n");
    fprintf(out_file, "\n");
    fprintf(out_file, ".data\n");
    fprintf(out_file, "memory: .zero %d\n", MEMORY_SIZE * 8); // long = 8 bytes
    fprintf(out_file, "input_buffer: .zero 1024\n");
    fprintf(out_file, "output_format: .asciz \"%%ld\\n\"\n");
    fprintf(out_file, "char_format: .asciz \"%%c\"\n");
    fprintf(out_file, "input_format_num: .asciz \"%%ld\"\n");
    fprintf(out_file, "input_format_str: .asciz \"%%s\"\n");
    fprintf(out_file, "string_output_format: .asciz \"%%s\\n\"\n");
    fprintf(out_file, "\n");
    fprintf(out_file, ".text\n");
    fprintf(out_file, "main:\n");
    fprintf(out_file, "    push rbp\n");
    fprintf(out_file, "    mov rbp, rsp\n");
    fprintf(out_file, "\n\n");
}