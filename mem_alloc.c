#include "mem_alloc.h"
#include <stdio.h>


typedef long Align;


union header {
    struct {
        union header *ptr; // next block
        unsigned size; // size of this block
    } s;
    Align x; // unused, but forces alignment
};

typedef union header Header;

static Header base; // some base for our heap
static Header *freep = NULL; // free list

#define NALLOC 1024 // tiniest amount of memory we're allowed to request from the OS


static Header *more_mem_pls(unsigned nu)
{
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
    printf("UP Pointer: %p\n", (void*) up);
    free((void*) (up + 1));
    return freep; // freep static, edited during free.
}

void* malloc(size_t size)
{
    // some checks to make sure the user can't do absolutely whatever they want because i'm :sparkle: professional :sparkles

    if (size == 0) {
        printf("this edge case is dumb and you should feel bad for requesting zero bytes of memory even though normally it returns a valid pointer for some reason\n");
        return NULL;
    }
    if (size < 0) {
        printf("Silly goose, trying to get negative memory\n");
        return NULL;
    }


    // el plan
    // degenerate free list at first
    // other wise look for the first block that fits
    // ask for more memory if we need it :pleading_face: :points_right: :points_left: 

    Header *p, *prevp;
    Header *more_mem_pls(unsigned);
    unsigned nunits;
    printf("Sizeof Header: %lu\n", sizeof(Header));
    printf("Sizeof Long: %lu\n", sizeof(long));
    // skeleton code
    // if freep empty (first malloc call), then make it a degenerate free list Base pointing to itself
    // loop through the free list, looking for the first space that fits, not the best fit
    // we reach the start of the free list, then we're out of memory and need to ask for more

    // space needed for header + storage
    nunits = (size + sizeof(Header) -1) / sizeof(Header) + 1;

    // base case
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    // iterates through the linked list
    for (p = prevp->s.ptr; ;prevp = p, p = p->s.ptr) 
    {
        // if sufficient size
        if (p->s.size >= nunits)
        {
            // exact fit
            if (p->s.size == nunits) {
                // removes p from the free list
                prevp->s.ptr = p->s.ptr;
            } else {
                // Image of the situation
                // | used | free space | more free |
                // | used | free | ret | more free |
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freep = prevp;
            return (void*) (p + 1); // pointer to the memory, plus offset for the header
        }

        // if we've wrapped around to the start
        if (p == freep) {
            // request 
            printf("Requesting more memory :pleading_face:\n");
            if ((p = more_mem_pls(nunits)) == NULL) {
                return NULL; // we failed to get more memory
            }
        }
    }

}


void free(void* ptr)
{
    // loops through free list and tries to place 
    
    Header *bp, *p;

    // casts back to Header* and subtracts one
    bp = ((Header *)ptr) - 1;
    printf("BP pointer: %p\n", (void*) bp);
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
            printf("Gap at start or end: %p --- %p\n", (void*) p, (void*) p->s.ptr);
            break; // found our gap yippee :3
        }
    }

    // attempts to coalesse 
    printf("Trying to coalesse with next one\n");
    if (bp + bp->s.size == p->s.ptr) { // found a match, going to merge with next block
        printf("Can merge with next\n");
        bp->s.size += p->s.ptr->s.size; // adds the next block size
        bp->s.ptr = p->s.ptr->s.ptr;
        printf("Successfully merged with next\n");
    } else {
        printf("Cannot merge with next\n");
        
        bp->s.ptr = p->s.ptr;
    }
    // check for merge with previous block
    printf("Trying to coalesse with previous one\n");
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
        printf("Successfully merged with prev\n");
    } else {
        p->s.ptr = bp;
    }

    freep = p;
}

void* calloc(size_t count, size_t size) 
{
    void* ptr = malloc(count * size);
    memset(ptr, 0, count*size);
    return ptr;
}

void print_freep(void)
{
    printf("Base address: %p\n", (void*) &base);
    if (!freep)
    {
        printf("Empty freep (no malloc has happened yet)\n");
        return;
    }

    for (Header* p = freep; p->s.ptr != freep; p = p->s.ptr) {
        printf("%p -->", (void*) p);
    }
    printf("\n");
}
