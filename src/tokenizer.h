#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <stdlib.h> // for atol()
#include <string.h> // for strncmp()
#include <ctype.h>  // for isdigit()

// --- 字句解析 (トークナイザ) ---

int consume_separator();
long get_number_literal();
long get_opcode();

#endif // TOKENIZER_H
