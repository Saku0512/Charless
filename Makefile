CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRCS_COMPILER = src/compiler.c src/tokenizer.c src/vm.c src/linker.c src/utils.c src/codegen/codegen.c src/codegen/asm_header.c $(wildcard src/codegen/opecodes/ope*x/*.c)

OBJS_COMPILER = $(SRCS_COMPILER:.c=.o)

TARGET_COMPILER = clessc

.PHONY: all clean

all: $(TARGET_COMPILER)

$(TARGET_COMPILER): $(OBJS_COMPILER)
	$(CC) $(CFLAGS) -o $(TARGET_COMPILER) $(OBJS_COMPILER)

# Pattern rule to build .o from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

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
