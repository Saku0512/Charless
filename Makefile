CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRCS_COMPILER = src/compiler.c src/tokenizer.c src/vm.c

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
