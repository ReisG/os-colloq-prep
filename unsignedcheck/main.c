#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	unsigned char a = 1;
	unsigned b = a << 8;
	printf("%u\n", b);
}
