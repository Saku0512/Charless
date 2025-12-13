#include "tokenizer.h"
#include "vm.h"
#include <ctype.h>

// --- 字句解析 (トークナイザ) ---

// '200' セパレータを消費する (v3)
// セパレータがあれば 1 を、なければ 0 を返す
int consume_separator() {
    if (strncmp(ip, "200", 3) == 0) {
        ip += 3; // ポインタを3文字進める
        return 1;
    }
    return 0;
}

// '990' の後の数値リテラルを読み込む (v3)
// (仕様 4. に基づく)
long get_number_literal() {
    char buffer[64]; // 数値を一時的に格納
    int i = 0;

    // '200' が来るまで数字を読み込む
    while (*ip != '\0' && strncmp(ip, "200", 3) != 0) {
        if (!isdigit(*ip)) {
            fprintf(stderr, "Error: Expected digit in number literal, but got '%c'\n", *ip);
            exit(1);
        }
        buffer[i++] = *ip;
        ip++;
    }
    buffer[i] = '\0';

    // 末尾の '200' を消費する
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '200' after number literal\n");
        exit(1);
    }

    return atol(buffer); // C言語の long 型に変換
}

// 命令トークンを読み込む
// (仕様 3. に基づく)
long get_opcode() {
    char buffer[64];
    int i = 0;

    // Skip whitespace and comments
    while (1) {
        while (isspace(*ip)) {
            ip++;
        }

        // Single-line comment (90020) - v3
        if (strncmp(ip, "90020", 5) == 0) {
            ip += 5;
            while (*ip != '\0' && *ip != '\n') {
                ip++;
            }
            continue; // Restart the loop to handle more whitespace or comments
        }

        // Multi-line comment (90120 ... 90120) - v3
        if (strncmp(ip, "90120", 5) == 0) {
            ip += 5; // Consume the opening "90120"
            while (*ip != '\0' && strncmp(ip, "90120", 5) != 0) {
                ip++;
            }
            if (strncmp(ip, "90120", 5) == 0) {
                ip += 5; // Consume the closing "90120"
            }
            continue; // Restart the loop
        }
        
        // Legacy 2-digit comments (deprecated, show warning)
        if (strncmp(ip, "9720", 4) == 0) {
            fprintf(stderr, "Warning: 2-digit comment (97) is deprecated. Use 90020 instead.\n");
            ip += 4;
            while (*ip != '\0' && *ip != '\n') {
                ip++;
            }
            continue;
        }
        if (strncmp(ip, "9820", 4) == 0) {
            fprintf(stderr, "Warning: 2-digit comment (98) is deprecated. Use 90120 instead.\n");
            ip += 4;
            while (*ip != '\0' && strncmp(ip, "9820", 4) != 0) {
                ip++;
            }
            if (strncmp(ip, "9820", 4) == 0) {
                ip += 4;
            }
            continue;
        }
        
        break; // No more whitespace or comments
    }

    // オペコードの数字部分を読み込む（3桁固定）
    // v3では命令は必ず3桁
    while (*ip != '\0' && i < 3) {
        // 先読みして、コメントの開始シーケンスかをチェック
        if (strncmp(ip, "90020", 5) == 0 || strncmp(ip, "90120", 5) == 0) {
            if (i > 0) {
                // バッファに既に数字が入っているのにコメントが始まったらエラー
                fprintf(stderr, "Error: Comments are not allowed within an instruction. Please remove the comment here.\n");
                exit(1);
            }
            // i == 0 の場合は、get_opcode冒頭のスキップループで処理されるべき。
            // ここに到達するのは想定外だが、ループを抜けて後続に任せる。
            break;
        }

        // セパレータか数値リテラルの開始ならオペコードの終わり
        if (strncmp(ip, "200", 3) == 0 || strncmp(ip, "990", 3) == 0) {
            break;
        }

        if (!isdigit(*ip)) {
            // 数字でない文字（空白など）ならオペコードの終わり
            break;
        }

        buffer[i++] = *ip;
        ip++;
    }
    buffer[i] = '\0';

    if (i == 0) return -1; // コードの終端
    
    // v3: 命令は必ず3桁でなければならない
    if (i < 3) {
        fprintf(stderr, "Error: Opcode must be 3 digits (v3). Found %d-digit opcode: %s\n", i, buffer);
        fprintf(stderr, "Please use 3-digit opcodes (e.g., 000, 010, 020, etc.)\n");
        exit(1);
    }
    
    if (i > 3) {
        fprintf(stderr, "Error: Opcode must be exactly 3 digits. Found %d-digit opcode: %s\n", i, buffer);
        exit(1);
    }
    
    return atol(buffer);
}