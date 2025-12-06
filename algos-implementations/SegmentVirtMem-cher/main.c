#include <stdio.h>
#include <limits.h>
#include <signal.h>

typedef unsigned addr_t;

struct Segment
{
    unsigned base;
    unsigned limit;
};

enum { ADDR_BITS = sizeof(addr_t) * CHAR_BITS };
enum { SEG_BITS = 2};

unsigned va2phys(unsigned va, struct Segment *segs)
{
    unsigned sn = va >> (ADDR_BITS - SEG_BITS);
    unsigned offset = va & ((1U << (ADDR_BITS - SEG_BITS)) - 1);
    
    if (segs[sn].limit <= offset)
    {
        raise(SIGUSR1);
    }
    else
    {
        return segs[sn].base + offset;
    }
}