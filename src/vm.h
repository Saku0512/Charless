#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h> // for exit()

// --- 仮想マシン(VM)の定義 ---
#define STACK_SIZE 256
#define MEMORY_SIZE 256

extern long stack[STACK_SIZE];
extern int sp;
extern long memory[MEMORY_SIZE];
extern char *ip;

// --- スタック操作 ---
void push(long value);
long pop();

#endif // VM_H
