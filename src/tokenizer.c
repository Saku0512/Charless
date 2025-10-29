#include "tokenizer.h"
#include "vm.h"
#include <ctype.h>

// --- 字句解析 (トークナイザ) ---

// '20' セパレータを消費する
// セパレータがあれば 1 を、なければ 0 を返す
int consume_separator() {
    if (strncmp(ip, "20", 2) == 0) {
        ip += 2; // ポインタを2文字進める
        return 1;
    }
    return 0;
}

// '99' の後の数値リテラルを読み込む
// (仕様 4. に基づく)
long get_number_literal() {
    char buffer[64]; // 数値を一時的に格納
    int i = 0;

    // '20' が来るまで数字を読み込む
    while (*ip != '\0' && strncmp(ip, "20", 2) != 0) {
        if (!isdigit(*ip)) {
            fprintf(stderr, "Error: Expected digit in number literal, but got '%c'\n", *ip);
            exit(1);
        }
        buffer[i++] = *ip;
        ip++;
    }
    buffer[i] = '\0';

    // 末尾の '20' を消費する
    if (!consume_separator()) {
        fprintf(stderr, "Error: Expected '20' after number literal\n");
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

        // Single-line comment (9720)
        if (strncmp(ip, "9720", 4) == 0) {
            ip += 4;
            while (*ip != '\0' && *ip != '\n') {
                ip++;
            }
            continue; // Restart the loop to handle more whitespace or comments
        }

        // Multi-line comment (9820 ... 9820)
        if (strncmp(ip, "9820", 4) == 0) {
            ip += 4; // Consume the opening "9820"
            while (*ip != '\0' && strncmp(ip, "9820", 4) != 0) {
                ip++;
            }
            if (strncmp(ip, "9820", 4) == 0) {
                ip += 4; // Consume the closing "9820"
            }
            continue; // Restart the loop
        }
        
        break; // No more whitespace or comments
    }

    // オペコードの数字部分を読み込む
    while (*ip != '\0') {
        // 先読みして、コメントの開始シーケンスかをチェック
        if (strncmp(ip, "9720", 4) == 0 || strncmp(ip, "9820", 4) == 0) {
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
        if (strncmp(ip, "20", 2) == 0 || strncmp(ip, "99", 2) == 0) {
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
    
    return atol(buffer);
}