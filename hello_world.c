#include "mem_alloc.h"
#include <stdio.h>

int main(void) {
    print_freep();
    char* str = (char*) malloc(sizeof(char) * 12);
    str = "Hello World!";
    print_freep();
    printf("%s\n", str);
    printf("Str pointer: %p\n", (void*) str);
    free(str);
    return 0;
}
