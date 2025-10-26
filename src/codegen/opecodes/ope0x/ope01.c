#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../../vm.h"
#include "../../../tokenizer.h"
#include "ope01.h"

void opecode_01(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after 01\n");
        exit(1);
    }
    fprintf(out_file, "    # PRINT_STRING\n");

    while (strncmp(ip, "20", 2) != 0 && *ip != '\0') {
        char len_str[2] = {*ip, '\0'};
        ip++;
        int len = atoi(len_str);

        if (len <= 0 || len > 10) { // Basic validation
            fprintf(stderr, "Error: Invalid length for character code\n");
            exit(1);
        }

        char* code_str = malloc(len + 1);
        strncpy(code_str, ip, len);
        code_str[len] = '\0';
        ip += len;

        long val = atol(code_str);
        free(code_str);

        fprintf(out_file, "    mov rsi, %ld\n", val);
        fprintf(out_file, "    lea rdi, char_format[rip]\n");
        fprintf(out_file, "    mov al, 0\n");
        fprintf(out_file, "    call printf\n");
    }

    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after string data\n");
        exit(1);
    }
}