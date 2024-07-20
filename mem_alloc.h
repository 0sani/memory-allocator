#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

// copying only the most necessary parts of stdlib
#if defined(__SIZE_TYPE__)
typedef __SIZE_TYPE__           __darwin_size_t;        /* sizeof() */
#else
typedef unsigned long           __darwin_size_t;        /* sizeof() */
#endif

#if __has_builtin(__builtin___memset_chk) || defined(__GNUC__)
#undef memset
/* void *memset(void *b, int c, size_t len) */
#define memset(dest, ...) \
		__builtin___memset_chk (dest, __VA_ARGS__, __darwin_obsz0 (dest))
#endif

typedef __darwin_size_t        size_t;

#define NULL __DARWIN_NULL


// not allowing an input byte size of 0 because that's dumb corner case behavior
void* malloc(size_t size);
void free(void *ptr);
void* calloc(size_t count, size_t size);

#ifdef DEBUG
void heap_info(void);
#else
static inline void heap_info(void) {}
#endif

#endif
