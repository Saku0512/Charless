#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../../tokenizer.h"
#include "../../../vm.h"
#include "ope51.h"

void opecode_51(FILE *out_file){
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after 500\n");
        exit(1);
    }
    if (strncmp(ip, "990", 3) == 0) {
        ip += 3;
        long val = get_number_literal();
        fprintf(out_file, "    # PUSH %ld\n", val);
        fprintf(out_file, "    mov rax, %ld\n", val);
        fprintf(out_file, "    push rax\n");
    } else {
        fprintf(stderr, "Error: PUSH (500) requires '990' prefix\n");
        exit(1);
    }
}