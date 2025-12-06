// 2-level paged memory managment
#include <limits.h>
#include <signal.h>

typedef unsigned addr_t;
typedef unsigned phys_t;

enum
{
    LEVEL0 = 10
};
enum
{
    LEVEL1 = 10
};
// 12 bits in left for offset

enum
{
    ADDR_BITS = sizeof(addr_t) * CHAR_BITS
};

enum
{
    OFF_BITS = ADDR_BITS - LEVEL0 - LEVEL1
};

enum
{
    PAGE_MASK = (((phys_t) 1 << OFF_BITS) - 1)
};
enum
{
    LEVEL1_MASK = (((phys_t) 1 << LEVEL1) - 1)
};

enum
{
    PRESENT = 1, // check if page is mapped (The only one really required)
    WRITE = 2,   // can we write to this page
    EXECUTE = 4, // can contents of this page be executed
    // managment on OS level
    ACCESSED = 8, // used to mark unused pages
    DIRTY = 16,   // was something written to this page??
};

addr_t
vatophys(addr_t va, // requested virtual address
         phys_t *t0 // stores physical addresses of pages of level1
)
{
    addr_t i0 = va >> (OFF_BITS + LEVEL1);
    if (!(t0[i0] & PRESENT)) {
        raise(SIGSEGV);
    }

    phys_t *t1 = t0[i0] & ~PAGE_MASK; // ~PAGE_MASK - clearing service information
    addr_t i1 = (va >> OFF_BITS) & LEVEL1_MASK;
    if (!(t1[i1] & PRESENT)) {
        raise(SIGSEGV);
    }

    phys_t pa = t1[i1] & ~PAGE_MASK; // ~PAGE_MASK - clearing service information
    return pa | (va & PAGE_MASK);
}
