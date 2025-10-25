#include "vm.h"

// --- 仮想マシン(VM)の定義 ---
long stack[STACK_SIZE];
int sp = -1; // スタックポインタ (-1は空っぽ)
long memory[MEMORY_SIZE] = {0}; // 変数ストレージ
char *ip; // 命令ポインタ (Charlessコードを指す)

// --- スタック操作 ---
void push(long value) {
    if (sp >= STACK_SIZE - 1) {
        fprintf(stderr, "Error: Stack overflow\n");
        exit(1);
    }
    stack[++sp] = value;
}

long pop() {
    if (sp < 0) {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(1);
    }
    return stack[sp--];
}
