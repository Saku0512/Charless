#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>

#include "vm.h"
#include "tokenizer.h"

// Function to check if a filename ends with a specific extension
int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix > len_str) return 0;
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

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
        return 0;
    }

    // If input is a .cless file, compile it
    if (ends_with(input_filename, ".cless")) {
        // --- This is the original compiler logic ---
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

        ip = code;

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

        char final_output_name[256];
        if (output_filename) {
            strncpy(final_output_name, output_filename, sizeof(final_output_name) - 1);
        } else {
            if (output_asm) {
                // If -s is specified without -o, output is based on input filename
                char *base_out = basename(input_filename);
                char *dot_out = strrchr(base_out, '.');
                if (dot_out != NULL) {
                    size_t len = dot_out - base_out;
                    strncpy(final_output_name, base_out, len);
                    final_output_name[len] = '\0';
                } else {
                    strncpy(final_output_name, base_out, sizeof(final_output_name) - 1);
                }
                strcat(final_output_name, ".s");
            } else {
                // Default output is cless.out if not an assembly output
                strcpy(final_output_name, "cless.out");
            }
        }

        FILE *out_file = fopen(asm_filename, "w");
        if (out_file == NULL) {
            fprintf(stderr, "Error: Could not open output file %s\n", asm_filename);
            free(code);
            exit(1);
        }

        // --- Assembly Header ---
        fprintf(out_file, ".intel_syntax noprefix\n");
        fprintf(out_file, ".globl main\n");
        fprintf(out_file, "\n");
        fprintf(out_file, ".data\n");
        fprintf(out_file, "memory: .zero %d\n", MEMORY_SIZE * 8); // long = 8 bytes
        fprintf(out_file, "output_format: .asciz \"%%ld\\n\"\n");
        fprintf(out_file, "\n");
        fprintf(out_file, ".text\n");
        fprintf(out_file, "main:\n");
        fprintf(out_file, "    push rbp\n");
        fprintf(out_file, "    mov rbp, rsp\n");
        fprintf(out_file, "\n"
);


        while (1) {
            long opcode = get_opcode();
            if (opcode == -1) break;

            switch (opcode) {
                case 00: // HALT
                    fprintf(out_file, "    # HALT\n");
                    fprintf(out_file, "    mov rsp, rbp\n");
                    fprintf(out_file, "    pop rbp\n");
                    fprintf(out_file, "    ret\n");
                    break;

                case 02: // PRINT
                    if (!consume_separator()) {
                        fprintf(stderr, "Error: Expected '20' after 02\n");
                        exit(1);
                    }
                    fprintf(out_file, "    # PRINT\n");
                    fprintf(out_file, "    pop rsi\n");
                    fprintf(out_file, "    lea rdi, output_format[rip]\n");
                    fprintf(out_file, "    mov al, 0\n");
                    fprintf(out_file, "    call printf\n");
                    break;

                case 51: // PUSH
                    if (!consume_separator()) {
                        fprintf(stderr, "Error: Expected '20' after 51\n");
                        exit(1);
                    }
                    if (strncmp(ip, "99", 2) == 0) {
                        ip += 2;
                        long val = get_number_literal();
                        fprintf(out_file, "    # PUSH %ld\n", val);
                        fprintf(out_file, "    mov rax, %ld\n", val);
                        fprintf(out_file, "    push rax\n");
                    } else {
                        fprintf(stderr, "Error: PUSH (51) requires '99' prefix\n");
                        exit(1);
                    }
                    break;

                case 53: // STORE
                    if (!consume_separator()) {
                        fprintf(stderr, "Error: Expected '20' after 53\n");
                        exit(1);
                    }
                    if (strncmp(ip, "99", 2) == 0) {
                        ip += 2;
                        long var_id = get_number_literal();
                        fprintf(out_file, "    # STORE %ld\n", var_id);
                        fprintf(out_file, "    pop rax\n");
                        fprintf(out_file, "    lea rbx, memory[rip]\n");
                        fprintf(out_file, "    mov [rbx + %ld], rax\n", var_id * 8);
                    } else {
                        fprintf(stderr, "Error: STORE (53) requires '99' prefix\n");
                        exit(1);
                    }
                    break;

                case 54: // LOAD
                    if (!consume_separator()) {
                        fprintf(stderr, "Error: Expected '20' after 54\n");
                        exit(1);
                    }
                    if (strncmp(ip, "99", 2) == 0) {
                        ip += 2;
                        long var_id = get_number_literal();
                        fprintf(out_file, "    # LOAD %ld\n", var_id);
                        fprintf(out_file, "    lea rbx, memory[rip]\n");
                        fprintf(out_file, "    mov rax, [rbx + %ld]\n", var_id * 8);
                        fprintf(out_file, "    push rax\n");
                    } else {
                        fprintf(stderr, "Error: LOAD (54) requires '99' prefix\n");
                        exit(1);
                    }
                    break;

                default:
                    fprintf(stderr, "Error: Unknown opcode %ld\n", opcode);
                    exit(1);
            }
            fprintf(out_file, "\n");
        }

        // --- Assembly Footer ---
        fprintf(out_file, "    # Implicit HALT\n");
        fprintf(out_file, "    mov rsp, rbp\n");
        fprintf(out_file, "    pop rbp\n");
        fprintf(out_file, "    ret\n");

        fclose(out_file);
        free(code);

        if (output_asm) {
            // If -s is used, rename the assembly file if -o is specified
            if (output_filename) {
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
            snprintf(command, sizeof(command), "gcc %s -o %s", asm_filename, final_output_name);
            
            int ret = system(command);
            if (ret != 0) {
                fprintf(stderr, "Error: Assembler/Linker failed\n");
                exit(1);
            }
            
            // Remove the temporary assembly file
            remove(asm_filename);
            printf("Compilation successful. Executable created: %s\n", final_output_name);
        }

        return 0;
    }

    // If we reach here, the input file is not supported
    fprintf(stderr, "Error: Unsupported input file type. Please provide a .cless or .s file.\n");
    exit(1);
}