CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# --- Main Compiler ---
SRCS_COMPILER = src/compiler.c src/tokenizer.c src/vm.c src/linker.c src/utils.c src/codegen/codegen.c src/codegen/asm_header.c $(wildcard src/codegen/opecodes/ope*x/*.c)
OBJS_COMPILER = $(SRCS_COMPILER:.c=.o)
TARGET_COMPILER = clessc

# --- Test Configuration ---
# Add new test runners here
TEST_RUNNERS = tests/test_tokenizer_runner tests/test_codegen_runner

.PHONY: all clean test

all: $(TARGET_COMPILER)

$(TARGET_COMPILER): $(OBJS_COMPILER)
	$(CC) $(CFLAGS) -o $@ $(OBJS_COMPILER)

# Generic rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Test Targets ---
test: all $(TEST_RUNNERS)
	@echo "\n--- Running All Tests ---"
	@./tests/test_tokenizer_runner
	@echo ""
	@./tests/test_codegen_runner
	@echo "--- All Tests Finished ---\n"

# Rule for the tokenizer unit test runner
tests/test_tokenizer_runner: tests/test_tokenizer.c src/tokenizer.c src/vm.c
	$(CC) $(CFLAGS) -o $@ $^

# Rule for the codegen E2E test runner
tests/test_codegen_runner: tests/test_codegen.c
	$(CC) $(CFLAGS) -o $@ $<

# --- Clean Target ---
clean:
	rm -f $(OBJS_COMPILER) $(TARGET_COMPILER) $(TEST_RUNNERS) *.s *.out *.cless *.txt
