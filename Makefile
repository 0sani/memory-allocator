# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
DEBUG_FLAGS = -g -DDEBUG

OBJ_HELLO = hello_world.o mem_alloc.o
OBJ_TESTS = tests.o mem_alloc.o
TARGET_HELLO = hello
TARGET_TESTS = tests

# Default target
all: build

# Build configuration
build: $(TARGET_HELLO) $(TARGET_TESTS)

# Debug configuration
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET_HELLO) $(TARGET_TESTS)

# Link object files to create the final executables
$(TARGET_HELLO): $(OBJ_HELLO)
	$(CC) $(CFLAGS) -o $(TARGET_HELLO) $(OBJ_HELLO)

$(TARGET_TESTS): $(OBJ_TESTS)
	$(CC) $(CFLAGS) -o $(TARGET_TESTS) $(OBJ_TESTS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_HELLO) $(TARGET_HELLO) $(OBJ_TESTS) $(TARGET_TESTS)

# Phony targets (these are not files)
.PHONY: all build debug clean
