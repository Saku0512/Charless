#include <stdio.h>
#include <stdlib.h> // for atol(), exit()
#include <string.h> // for strncmp()
#include <ctype.h>  // for isdigit()

#include "vm.h"
#include "tokenizer.h"


// --- メインの実行ループ ---
int main(int argc, char *argv[]) { // <-- (1) main のシグネチャ変更

    // --- (2) 引数チェック ---
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.cless>\n", argv[0]);
        exit(1);
    }

    // --- (3) ファイル読み込み処理 ---
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        exit(1);
    }

    fseek(file, 0, SEEK_END); // ファイルの末尾に移動
    long file_size = ftell(file); // ファイルサイズを取得
    rewind(file); // ファイルの先頭に戻る

    char *code = malloc(file_size + 1); // ファイルサイズ + 終端ヌル文字('\0')
    if (code == NULL) {
        fprintf(stderr, "Error: Could not allocate memory to read file\n");
        fclose(file);
        exit(1);
    }

    size_t read_size = fread(code, 1, file_size, file); // ファイル内容を 'code' に読み込む
    if (read_size != file_size) {
        fprintf(stderr, "Error: Could not read file\n");
        fclose(file);
        free(code);
        exit(1);
    }
    code[file_size] = '\0'; // 終端ヌル文字をセット
    fclose(file);
    // --- (3) ファイル読み込み終了 ---


    ip = code; // 命令ポインタをコードの先頭にセット

    printf("Charless VM Start...\n");
    printf("Executing code from: %s\n\n", argv[1]); // (4) 実行元ファイル名を表示

    while (1) {
        long opcode = get_opcode();
        if (opcode == -1) break; // コードの終端

        // printf("DEBUG: Opcode %ld\n", opcode); // デバッグ用

        switch (opcode) {
            case 00: // 終了
                printf("\n...VM Halt (00).\n");
                free(code); // (5) メモリ解放
                return 0;

            case 02: // 数値出力
                {
                    long val = pop();
                    printf("Output: %ld\n", val);
                    if (!consume_separator()) { // 命令後の '20' を消費
                        fprintf(stderr, "Error: Expected '20' after 02\n");
                        free(code); // (5) メモリ解放
                        exit(1);
                    }
                }
                break;

            case 51: // PUSH
                // 51 の直後には '20' が必須
                if (!consume_separator()) {
                    fprintf(stderr, "Error: Expected '20' after 51\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }

                if (strncmp(ip, "99", 2) == 0) {
                    ip += 2; // '99' をスキップ
                    long val = get_number_literal();
                    push(val);
                    // printf("DEBUG: Pushed %ld\n", val);
                } else {
                    fprintf(stderr, "Error: PUSH (51) requires '99' prefix\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }
                break;

            case 53: // STORE
                // 53 の直後には '20' が必須
                if (!consume_separator()) {
                    fprintf(stderr, "Error: Expected '20' after 53\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }

                if (strncmp(ip, "99", 2) == 0) {
                    ip += 2; // '99' をスキップ
                    long var_id = get_number_literal();
                    if (var_id < 0 || var_id >= MEMORY_SIZE) {
                        fprintf(stderr, "Error: Invalid memory ID %ld\n", var_id);
                        free(code); // (5) メモリ解放
                        exit(1);
                    }
                    long val_to_store = pop();
                    memory[var_id] = val_to_store;
                    // printf("DEBUG: Stored %ld into mem[%ld]\n", val_to_store, var_id);
                } else {
                    fprintf(stderr, "Error: STORE (53) requires '99' prefix\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }
                break;

            case 54: // LOAD
                // 54 の直後には '20' が必須
                if (!consume_separator()) {
                    fprintf(stderr, "Error: Expected '20' after 54\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }
                
                if (strncmp(ip, "99", 2) == 0) {
                    ip += 2; // '99' をスキップ
                    long var_id = get_number_literal();
                    if (var_id < 0 || var_id >= MEMORY_SIZE) {
                        fprintf(stderr, "Error: Invalid memory ID %ld\n", var_id);
                        free(code); // (5) メモリ解放
                        exit(1);
                    }
                    long val_loaded = memory[var_id];
                    push(val_loaded);
                    // printf("DEBUG: Loaded %ld from mem[%ld]\n", val_loaded, var_id);
                } else {
                    fprintf(stderr, "Error: LOAD (54) requires '99' prefix\n");
                    free(code); // (5) メモリ解放
                    exit(1);
                }
                break;

            default:
                fprintf(stderr, "Error: Unknown opcode %ld\n", opcode);
                free(code); // (5) メモリ解放
                exit(1);
        }
    }

    printf("...VM End (no HALT).\n");
    free(code); // (5) メモリ解放
    return 0;
}
