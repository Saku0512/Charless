#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "vm.h"
#include "tokenizer.h"
#include "codegen/codegen.h"
#include "linker.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *output_filename = NULL;
    int output_asm = 0; // Flag to output assembly instead of object file

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_filename = argv[i + 1];
                i++; // Skip the next argument
            } else {
                fprintf(stderr, "Error: -o requires an output filename\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            output_asm = 1;
        } else {
            if (input_filename != NULL) {
                fprintf(stderr, "Error: Only one input file is allowed\n");
                exit(1);
            }
            input_filename = argv[i];
        }
    }

    if (input_filename == NULL) {
        fprintf(stderr, "Usage: %s <filename.cless | filename.s> [-o <output_filename>]\n", argv[0]);
        exit(1);
    }

    // If input is an assembly file, link it
    if (ends_with(input_filename, ".s")) {
        link_assembly(input_filename, output_filename);
        return 0;
    }

    // If input is a .cless file, compile it
    if (ends_with(input_filename, ".cless")) {
        FILE *file = fopen(input_filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", input_filename);
            exit(1);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *code = malloc(file_size + 1);
        if (code == NULL) {
            fprintf(stderr, "Error: Could not allocate memory to read file\n");
            fclose(file);
            exit(1);
        }

        fread(code, 1, file_size, file);
        code[file_size] = '\0';
        fclose(file);

        // Determine output filename
        char asm_filename[256];
        char *base = basename(input_filename);
        char *dot = strrchr(base, '.');
        if (dot != NULL) {
            size_t len = dot - base;
            strncpy(asm_filename, base, len);
            asm_filename[len] = '\0';
        } else {
            strncpy(asm_filename, base, sizeof(asm_filename) - 1);
        }
        strcat(asm_filename, ".s");

        FILE *out_file = fopen(asm_filename, "w");
        if (out_file == NULL) {
            fprintf(stderr, "Error: Could not open output file %s\n", asm_filename);
            free(code);
            exit(1);
        }

        generate_code(out_file, code);

        fclose(out_file);
        free(code);

        assemble_and_link(asm_filename, output_filename, output_asm);

        return 0;
    }

    // If we reach here, the input file is not supported
    fprintf(stderr, "Error: Unsupported input file type. Please provide a .cless or .s file.\n");
    exit(1);
}
