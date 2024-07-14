# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g
OBJ_HELLO = hello_world.o mem_alloc.o
TARGET_HELLO = hello

# Default target
all: $(TARGET_HELLO)

# Link object files to create the final  executable
$(TARGET_HELLO): $(OBJ_HELLO)
	$(CC) $(CFLAGS) -o $(TARGET_HELLO) $(OBJ_HELLO)


# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_HELLO) $(TARGET_HELLO)

# Phony targets (these are not files)
.PHONY: all clean
