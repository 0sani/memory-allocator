#include "mem_alloc.h"
#include <stdio.h>

int main(void) {
    char* str = (char*) malloc(sizeof(char) * 12);
    str[0] = 'H';
    str[1] = '\0';
    printf("%s\n", str);
    printf("Str pointer: %p\n", (void*) str);
    free(str);
    return 0;
}
