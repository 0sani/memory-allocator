#include "mem_alloc.h"
#include <stdio.h>

typedef long Align;


union header {
    struct {
        union header *ptr; // next block
        unsigned size; // size of this block
        int free; // 0 if used
    } s;
    Align x; // unused, but forces alignment
};

typedef union header Header;

static Header sentinel; // base
static Header* blocks = NULL;

#ifdef DEBUG
static int sbrk_calls = 0;
static int malloc_calls = 0;
static int free_calls = 0;
#endif

#define NALLOC 1024 // tiniest amount of memory we're allowed to request from the OS


static Header *more_mem_pls(unsigned nu)
{
    #ifdef DEBUG
    sbrk_calls++;
    #endif

    char *cp, *sbrk(int);
    Header *up;

    // minimal amount of mem to get
    if (nu < NALLOC) { 
        nu = NALLOC;
    }

    cp = sbrk(nu * sizeof(Header));
    if (cp == (char*) - 1) {
        return NULL; // failed for some reason, stored in errno
    }

    up = (Header *) cp;
    up->s.size = nu;
    up->s.free = 1;
    return up;
}


void* malloc(size_t size) {
    #ifdef DEBUG
    malloc_calls++;
    #endif


    if (size <= 0) {
        printf("Invalid size");
        return NULL;
    }

    Header *p;

    unsigned nunits = (size + sizeof(Header) - 1) / sizeof(Header) + 1;

    // if uninitialized
    if (blocks == NULL) {
        // points the sentinel to itself
        sentinel.s.ptr = &sentinel;
        sentinel.s.size = 0;
        sentinel.s.free = 0;
        blocks = &sentinel;
    }

    for (p = blocks->s.ptr; ; p = p->s.ptr) {
        
        // if we've reached the start again
        if (p == &sentinel) {
            Header* up;
            if ((up = more_mem_pls(nunits)) == NULL)  {
                return NULL;
            }

            up->s.ptr = sentinel.s.ptr;
            sentinel.s.ptr = up;
        }

        
        // found something that fits
        if (p->s.size >= nunits && p->s.free) {

            if (p->s.size > nunits) {
                Header* new = p + nunits;
                new->s.ptr = p->s.ptr;
                new->s.free = 1;
                new->s.size = p->s.size - nunits;
                p->s.ptr = new;
                p->s.size = nunits;
            }
            p->s.free = 0;
            return (void *) (p + 1);
        }
    }
}

void free(void* ptr) {
    #ifdef DEBUG
    free_calls++;
    #endif

    Header* freed = ((Header *) ptr) - 1;

    Header *p, *prev;

    for (p = blocks->s.ptr, prev = blocks; p->s.ptr != blocks; p = p->s.ptr, prev = prev->s.ptr) {

        // found our block
        if (p == freed) {
            p->s.free = 1;

            // defragmentation
            // merge with next
            if (p->s.ptr->s.free == 1) {
                p->s.size += p->s.ptr->s.size;
                p->s.ptr = p->s.ptr->s.ptr;
            }
            // merge with previous
            if (prev->s.free == 1) {
                prev->s.size += p->s.size;
                prev->s.ptr = p->s.ptr;
            }

            return;
        }
    }
}



void* calloc(size_t count, size_t size) 
{
    void* ptr = malloc(count * size);
    memset(ptr, 0, count*size);
    return ptr;
}

#ifdef DEBUG
unsigned get_num_blocks(void) {
    if (blocks == NULL) {
        return 0;
    }
    Header *p;
    unsigned count = 0;
    for (p = blocks->s.ptr; p != blocks; p = p->s.ptr) {
        count++;
    }
    return count;
}


void heap_info(void) {
    printf("Number of sbrk calls: %d\n", sbrk_calls);
    printf("Number of malloc calls: %d\n", malloc_calls);
    printf("Number of free calls: %d\n", free_calls);
    printf("Number of blocks: %d\n", get_num_blocks());
}
#endif
