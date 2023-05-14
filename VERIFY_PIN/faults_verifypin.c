#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h>
#include <stddef.h>

#include "share/interface.h"
#include "share/types.h"

#ifdef CFICOT
#include "CFICOT.h"
#include "VERIFY_PIN/faults_verifypin.cfi.h"
#endif

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
volatile uint16_t status[5] = {0,0,0,0,0};

uint32_t __attribute__((noinline, noclone)) fault_dump(int Element)
{
	uint32_t ret;

	switch (Element)
	{
	case 0:
		ret = (uint32_t)status;
		break;
	case 1:
		ret = sizeof(status);
		break;
	default:
		ret = 0;
		break;
	}

	fault_end((uint32_t)ret);

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
uint32_t __attribute__((noipa, noinline, noclone, section(".cfiCOT"))) verifypin_call(void)
{
	initialize();
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status[0] = verifyPIN_11(&result);
	asm("SPUN_verifypin_call_end:");
	if (oracle() && (status[0] != CFI_FINAL(verifyPIN_11))) {
		return SPUN_FAULT_DETECTED;
	} else if (oracle() && (status[0] == CFI_FINAL(verifyPIN_11))) {
		return SPUN_FAULT_INJECTED;
	}
	return SPUN_EXEC_OK;
}
#endif

#ifdef VP31
uint32_t __attribute__((noipa, noinline, noclone, section(".cfiCOT"))) verifypin_call(void)
{
	initialize();
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status[0] = verifyPIN_31(&result);
	asm("SPUN_verifypin_call_end:");
	if (status[0] == CFI_WEXEC) {
		return SPUN_FAULT_INJECTED;
	}
	else if (oracle() && (status[0] == CFI_FINAL(verifyPIN_31))) {
		return SPUN_FAULT_INJECTED;
    	}
	else if(oracle() && (status[0] != CFI_FINAL(verifyPIN_31))) {
		return SPUN_FAULT_DETECTED;
	}
	return SPUN_EXEC_OK;	
}
#endif

#ifdef VP32
uint32_t __attribute__((noipa, noinline, noclone, section(".cfiCOT"))) verifypin_call(void)
{
	initialize();
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status[0] = verifyPIN_32(&result);
	asm("SPUN_verifypin_call_end:");
	if (status[0] == CFI_WEXEC) {
		return SPUN_FAULT_INJECTED;
	}
	else if (oracle() && (status[0] == CFI_FINAL(verifyPIN_32))) {
		return SPUN_FAULT_INJECTED;
    	}
	else if(oracle() && (status[0] != CFI_FINAL(verifyPIN_32))) {
		return SPUN_FAULT_DETECTED;
	}
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