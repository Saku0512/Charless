CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRCS_COMPILER = src/compiler.c src/tokenizer.c src/vm.c src/linker.c src/utils.c src/codegen/codegen.c src/codegen/asm_header.c $(wildcard src/codegen/opecodes/ope*x/*.c)

OBJS_COMPILER = $(SRCS_COMPILER:.c=.o)

TARGET_COMPILER = clessc

.PHONY: all clean test

all: $(TARGET_COMPILER)

$(TARGET_COMPILER): $(OBJS_COMPILER)
	$(CC) $(CFLAGS) -o $(TARGET_COMPILER) $(OBJS_COMPILER)

# Pattern rule to build .o from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Test Target ---
TEST_SRCS = tests/test_tokenizer.c src/tokenizer.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_TARGET = test_tokenizer

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

clean:
	rm -f src/*.o $(TARGET_COMPILER) *.s *.out
	rm -f src/codegen/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope0x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope1x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope2x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope3x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope4x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope5x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope6x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope7x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope8x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f src/codegen/opecodes/ope9x/*.o ${TARGET_COMPILER} *.s *.out
	rm -f tests/*.o $(TEST_TARGET)
