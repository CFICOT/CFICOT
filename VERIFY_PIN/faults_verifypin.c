#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h>
#include <stddef.h>

#include "share/interface.h"
#include "share/types.h"

#include "CFICOT.h"

#ifdef VP1
#include "VerifyPIN_1_HB/include/commons.h"
BOOL verifyPIN_1(void);
#endif

#ifdef VP11
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_11(volatile BOOL *result);
#endif

#ifdef VP31
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_31(volatile BOOL *result);
#endif

#ifdef VP32
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_32(volatile BOOL *result);
#endif

typedef uint16_t ret_t;
uint16_t status;

uint16_t __attribute__((noinline, noclone)) fault_dump(int Element)
{
	uint16_t ret;

	switch (Element)
	{
	default:
		ret = 0;
		break;
	}

	fault_end((uint16_t)ret);

	return ret;
}

#ifdef VP1
uint32_t __attribute__((noipa, noinline, noclone, section(".noprot"))) verifypin_call(void)
{
	initialize();

	asm("SPUN_verifypin_call_start:");
	verifyPIN_1();
	asm("SPUN_verifypin_call_end:");
	if (oracle()) {
		return SPUN_FAULT_INJECTED;
    	}
	return SPUN_EXEC_OK;
}
#endif

#ifdef VP11
uint32_t __attribute__((noipa, noinline, noclone, section(".cficustomlvl1"))) verifypin_call(void)
{
	initialize();
	volatile uint16_t status;
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status = verifyPIN_11(&result);
	asm("SPUN_verifypin_call_end:");
	if (oracle() && (status != CFI_ERROR)) {
		return SPUN_FAULT_INJECTED;
    	}
	else if(oracle() && (status == CFI_ERROR))
		return SPUN_FAULT_DETECTED;
	return SPUN_EXEC_OK;
}
#endif

#ifdef VP31
uint32_t __attribute__((noipa, noinline, noclone, section(".cficustomlvl1"))) verifypin_call(void)
{
	initialize();
	volatile uint16_t status;
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status = verifyPIN_31(&result);
	asm("SPUN_verifypin_call_end:");
	if (status == CFI_WEXEC) {
		return SPUN_FAULT_INJECTED;
	}
	else if (oracle() && (status != CFI_ERROR)) {
		return SPUN_FAULT_INJECTED;
    	}
	else if(oracle() && (status == CFI_ERROR))
		return SPUN_FAULT_DETECTED;
	return SPUN_EXEC_OK;	
}
#endif

#ifdef VP32
uint32_t __attribute__((noipa, noinline, noclone, section(".cficustomlvl1"))) verifypin_call(void)
{
	initialize();
	volatile uint16_t status;
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status = verifyPIN_32(&result);
	asm("SPUN_verifypin_call_end:");
	if (status == CFI_WEXEC) {
		return SPUN_FAULT_INJECTED;
	}
	else if (oracle() && (status != CFI_ERROR)) {
		return SPUN_FAULT_INJECTED;
    	}
	else if(oracle() && (status == CFI_ERROR))
		return SPUN_FAULT_DETECTED;
	return SPUN_EXEC_OK;	
}
#endif

/**
 * @brief Executes the fault testing scenario for the mem functions
 *
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
int main(void)
{
	fault_end(verifypin_call());

	fault_end(SPUN_SETUP_PROBLEM);
	exit(0);
}