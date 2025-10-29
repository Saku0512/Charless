#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/tokenizer.h"
#include "../src/vm.h"

// Define global variables declared in vm.h for the test
long stack[STACK_SIZE];
int sp = 0;
long memory[MEMORY_SIZE];
char *ip;

// --- Test Utilities ---
void setup_ip(const char* source) {
    ip = (char*)source;
}

// --- Test Cases ---

void test_get_opcode_simple() {
    printf("Running test: test_get_opcode_simple... ");
    setup_ip("1020");
    assert(get_opcode() == 10);
    printf("OK\n");
}

void test_get_opcode_with_whitespace() {
    printf("Running test: test_get_opcode_with_whitespace... ");
    setup_ip("  15  20");
    assert(get_opcode() == 15);
    printf("OK\n");
}

void test_get_opcode_with_single_line_comment() {
    printf("Running test: test_get_opcode_with_single_line_comment... ");
    setup_ip("9720 this is a comment\n2520");
    assert(get_opcode() == 25);
    printf("OK\n");
}

void test_get_opcode_with_multi_line_comment() {
    printf("Running test: test_get_opcode_with_multi_line_comment... ");
    setup_ip("9820 multi\nline\ncomment 9820 30 20");
    assert(get_opcode() == 30);
    printf("OK\n");
}

void test_get_number_literal() {
    printf("Running test: test_get_number_literal... ");
    setup_ip("1234520");
    assert(get_number_literal() == 12345);
    printf("OK\n");
}

void test_consume_separator() {
    printf("Running test: test_consume_separator... ");
    setup_ip("2010");
    assert(consume_separator() == 1);
    assert(*ip == '1');
    setup_ip("1020");
    assert(consume_separator() == 0);
    assert(*ip == '1');
    printf("OK\n");
}

// --- Test Runner ---

int main() {
    test_get_opcode_simple();
    test_get_opcode_with_whitespace();
    test_get_opcode_with_single_line_comment();
    test_get_opcode_with_multi_line_comment();
    test_get_number_literal();
    test_consume_separator();

    printf("\nAll tokenizer tests passed!\n");
    return 0;
}
