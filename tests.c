#include <stdlib.h>
#include "mem_alloc.h"
#include <stdio.h>
#include <time.h>

#define NUM_ALLOCS 1000
#define MAX_ALLOC_SIZE 2048
#define EXIT_FAILURE 1

static int run = 0;

void random_alloc_free(void) {
    void *pointers[NUM_ALLOCS] = {0};
    
    for (int i = 0; i < NUM_ALLOCS; i++) {

        

        size_t size = rand() % MAX_ALLOC_SIZE + 1; // Allocate between 1 and MAX_ALLOC_SIZE bytes

        pointers[i] = malloc(size);
        if (pointers[i] == NULL) {
            printf("Allocation failed at iteration %d\n", i);
            exit(EXIT_FAILURE);
        }

        // Optionally, write to the allocated memory to test for accessibility
        char *ptr = (char *)pointers[i];
        for (size_t j = 0; j < size; j++) {
            ptr[j] = (char)(j % 256);
        }

        // Randomly free some pointers
        if (rand() % 5 == 0) {
            free(pointers[i]);
            pointers[i] = NULL;
        }
    }

    // Free any remaining allocated pointers
    for (int i = 0; i < NUM_ALLOCS; i++) {
        if (pointers[i] != NULL) {
            free(pointers[i]);
        }
    }
}

int main(void) {
    // srand((unsigned int) time(NULL));
    srand(0);

    for (; run < 10; run++) { // Repeat the test multiple times
        printf("Run %d\n", run + 1);
        random_alloc_free();
        heap_info();
    }

    heap_info();

    printf("All tests completed successfully.\n");
    return 0;
}
