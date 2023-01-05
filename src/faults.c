#include <stdint.h>
#include "faults.h"

/* The noclone attribute is necessary to garanty there is only one instance of this function in the code */
void __attribute__ ((noinline,noclone)) fault_end(uint32_t ret)
{
	(void)ret;
	asm("nop");
	asm("nop");
	asm("nop");
}
