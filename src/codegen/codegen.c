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
#include "opecodes/ope7x/ope71.h"
#include "opecodes/ope7x/ope72.h"
#include "opecodes/ope7x/ope73.h"
#include "opecodes/ope8x/ope82.h"
#include "opecodes/ope8x/ope83.h"
#include "opecodes/ope8x/ope84.h"
#include "opecodes/ope8x/ope85.h"
#include "opecodes/ope8x/ope86.h"

void generate_code(FILE *out_file, char *code) {
    // --- Pass 1: Find all jump targets ---
    long *jump_targets = NULL;
    int targets_count = 0;
    char *scanner = code;
    while (*scanner != '\0') {
        if ((strncmp(scanner, "7002099", 7) == 0) ||
            (strncmp(scanner, "7012099", 7) == 0) ||
            (strncmp(scanner, "7022099", 7) == 0))
        {
            char *num_start = scanner + 7; // Move past "XXX2099"
            char *num_end = strstr(num_start, "20");
            if (num_end) {
                char addr_buf[20];
                long len = num_end - num_start;
                if (len > 0 && len < 20) {
                    strncpy(addr_buf, num_start, len);
                    addr_buf[len] = '\0';
                    long addr = atol(addr_buf);

                    // Add to jump_targets, avoiding duplicates
                    int found = 0;
                    for (int i = 0; i < targets_count; i++) {
                        if (jump_targets[i] == addr) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        jump_targets = realloc(jump_targets, (targets_count + 1) * sizeof(long));
                        jump_targets[targets_count++] = addr;
                    }
                }
            }
        }
        scanner++;
    }

    // --- Pass 2: Generate Code ---
    ip = code;
    asm_header(out_file);

    while (1) {
        long current_pos = ip - code;
        for (int i = 0; i < targets_count; i++) {
            if (jump_targets[i] == current_pos) {
                fprintf(out_file, "L%ld:\n", current_pos);
            }
        }

        long opcode = get_opcode();
        if (opcode == -1) break;

        switch (opcode) {
            case 0: // HALT (000 in v3, but parsed as 0)
                opecode_00(out_file);
                break;
            
            case 10: // PRINT_STRING (010 in v3, but parsed as 10)
                opecode_01(out_file);
                break;
            
            case 20: // PRINT (020 in v3, but parsed as 20)
                opecode_02(out_file);
                break;

            case 30: // PRINT_MEM_STRING (030 in v3, but parsed as 30)
                opecode_03(out_file);
                break;

            case 40: // PRINT_CHAR (040 in v3, but parsed as 40)
                opecode_04(out_file);
                break;

            case 100: // INPUT_CHAR (v3, 旧11)
                opecode_11(out_file);
                break;

            case 101: // INPUT_NUM (v3, 旧12)
                opecode_12(out_file);
                break;

            case 102: // INPUT_STRING (v3, 旧13)
                opecode_13(out_file);
                break;

            case 210: // PRINT_NEWLINE (v3, 旧21)
                opecode_21(out_file);
                break;

            case 500: // PUSH (v3, 旧51)
                opecode_51(out_file);
                break;

            case 510: // STORE (v3, 旧53)
                opecode_53(out_file);
                break;

            case 511: // LOAD (v3, 旧54)
                opecode_54(out_file);
                break;

            case 600: // ADD (v3, 旧61)
                opecode_61(out_file);
                break;

            case 601: // SUB (v3, 旧62)
                opecode_62(out_file);
                break;

            case 602: // MUL (v3, 旧63)
                opecode_63(out_file);
                break;

            case 603: // DIV (v3, 旧64)
                opecode_64(out_file);
                break;

            case 604: // MOD (v3, 旧65)
                opecode_65(out_file);
                break;

            case 700: // JUMP (v3, 旧71)
                opecode_71(out_file);
                break;
            case 701: // JZ (v3, 旧72)
                opecode_72(out_file);
                break;
            case 702: // JNZ (v3, 旧73)
                opecode_73(out_file);
                break;
            
            case 800: // EQS (v3, 旧81) - 未実装の可能性あり
                // opecode_81(out_file); // 実装されていない場合はコメントアウト
                fprintf(stderr, "Error: EQS (800) is not yet implemented\n");
                exit(1);
                break;
            case 801: // EQN (v3, 旧82)
                opecode_82(out_file);
                break;
            case 802: // GT (v3, 旧83)
                opecode_83(out_file);
                break;
            case 803: // LT (v3, 旧84)
                opecode_84(out_file);
                break;
            case 804: // GTE (v3, 旧85)
                opecode_85(out_file);
                break;
            case 805: // LTE (v3, 旧86)
                opecode_86(out_file);
                break;

            default:
                fprintf(stderr, "Error: Unknown opcode %ld\n", opcode);
                exit(1);
        }
        fprintf(out_file, "\n");
    }

    // --- Assembly Footer ---
    fprintf(out_file, "    # Implicit HALT\n");
    fprintf(out_file, "    mov rax, 0\n");
    fprintf(out_file, "    mov rsp, rbp\n");
    fprintf(out_file, "    pop rbp\n");
    fprintf(out_file, "    ret\n");
    
    free(jump_targets);
}
