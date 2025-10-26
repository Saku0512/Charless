#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "../vm.h"
#include "../tokenizer.h"
#include "asm_header.h"
#include "opecodes/ope0x/ope00.h"
#include "opecodes/ope0x/ope01.h"
#include "opecodes/ope0x/ope02.h"
#include "opecodes/ope0x/ope03.h"
#include "opecodes/ope0x/ope04.h"
#include "opecodes/ope1x/ope11.h"
#include "opecodes/ope1x/ope12.h"
#include "opecodes/ope1x/ope13.h"
#include "opecodes/ope2x/ope21.h"
#include "opecodes/ope5x/ope51.h"
#include "opecodes/ope5x/ope53.h"
#include "opecodes/ope5x/ope54.h"
#include "opecodes/ope6x/ope61.h"
#include "opecodes/ope6x/ope62.h"
#include "opecodes/ope6x/ope63.h"
#include "opecodes/ope6x/ope64.h"
#include "opecodes/ope6x/ope65.h"

void generate_code(FILE *out_file, char *code) {
    ip = code;

    asm_header(out_file);

    while (1) {
        long opcode = get_opcode();
        if (opcode == -1) break;

        switch (opcode) {
            case 00: // HALT
                opecode_00(out_file);
                break;
            
            case 01: // PRINT_STRING
                opecode_01(out_file);
                break;
            
            case 02: // PRINT
                opecode_02(out_file);
                break;

            case 3: // PRINT_MEM_STRING
                opecode_03(out_file);
                break;

            case 4: // PRINT_CHAR
                opecode_04(out_file);
                break;

            case 11: // INPUT_STRING_AND_PARSE
                opecode_11(out_file);
                break;

            case 12: // INPUT_NUM
                opecode_12(out_file);
                break;

            case 13: // INPUT_STRING
                opecode_13(out_file);
                break;

            case 21: // PRINT_NEWLINE
                opecode_21(out_file);
                break;

            case 51: // PUSH
                opecode_51(out_file);
                break;

            case 53: // STORE
                opecode_53(out_file);
                break;

            case 54: // LOAD
                opecode_54(out_file);
                break;

            case 61: // ADD
                opecode_61(out_file);
                break;

            case 62: // SUB
                opecode_62(out_file);
                break;

            case 63: // MUL
                opecode_63(out_file);
                break;

            case 64: // DIV
                opecode_64(out_file);
                break;

            case 65: // MOD
                opecode_65(out_file);
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
}
