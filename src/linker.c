#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linker.h"

void link_assembly(char *input_filename, char *output_filename) {
    char command[1024];
    char *final_output_name = output_filename ? output_filename : "cless.out";

    snprintf(command, sizeof(command), "gcc %s -o %s", input_filename, final_output_name);

    printf("Linking: %s\n", command);
    int ret = system(command);

    if (ret == 0) {
        printf("Linking successful. Executable created: %s\n", final_output_name);
    } else {
        fprintf(stderr, "Error: Linking failed\n");
        exit(1);
    }
}

void assemble_and_link(char *asm_filename, char *final_output_name, int output_asm) {
    if (output_asm) {
        // If -s is used, rename the assembly file if -o is specified
        if (final_output_name) {
            if (rename(asm_filename, final_output_name) != 0) {
                perror("Error renaming file");
                exit(1);
            }
            printf("Compilation successful. Assembly code written to %s\n", final_output_name);
        } else {
            printf("Compilation successful. Assembly code written to %s\n", asm_filename);
        }
    } else {
        // Assemble and link the .s file
        char command[1024];
        char *output_name = final_output_name ? final_output_name : "cless.out";
        snprintf(command, sizeof(command), "gcc %s -o %s", asm_filename, output_name);
        
        int ret = system(command);
        if (ret != 0) {
            fprintf(stderr, "Error: Assembler/Linker failed\n");
            exit(1);
        }
        
        // Remove the temporary assembly file
        remove(asm_filename);
        printf("Compilation successful. Executable created: %s\n", output_name);
    }
}
