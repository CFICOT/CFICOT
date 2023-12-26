/**
 * @file VerifyPIN fault injection scenario
 */
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

/**
 * @brief VerifyPIN without CFI protection
 */
#ifdef VP1
#include "VerifyPIN_1_HB/include/commons.h"
BOOL verifyPIN_1(void);
#endif

/**
 * @brief VerifyPIN with CFI protection
 */
#ifdef VP11
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_11(volatile BOOL *result);
#endif

/**
 * @brief VerifyPIN with CFI protection
 * byteArrayCompare is considered critical
 * Conditionnal execution is not enabled
 */
#ifdef VP31
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_31(volatile BOOL *result);
#endif

/**
 * @brief VerifyPIN with CFI protection
 * byteArrayCompare is considered critical
 * Conditionnal execution is enabled
 */
#ifdef VP32
#include "VerifyPIN_1_HB/include/commons.h"
uint16_t verifyPIN_32(volatile BOOL *result);
#endif

typedef uint16_t ret_t;
volatile uint16_t status[5] = {0,0,0,0,0};

/**
 * @brief Dump usefull results to analyze fault scenarios
 * @param[in] Element variable to be dumped
 * @return returns Element value or its size
 * @details Dump chosen variables and their size
 */
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
/**
 * @brief VerifyPIN call without CFI protection and fault return code production
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".noprot"))) verifypin_call(void)
{
	initialize();

	asm("SPUN_verifypin_call_start:");
	verifyPIN_1();
	asm("SPUN_verifypin_call_end:");
	if (!oracle()) {
		return SPUN_EXEC_OK;
    	} else {
		return SPUN_FAULT_INJECTED;
	}
	
}
#endif

#ifdef VP11
/**
 * @brief VerifyPIN call with CFI protection and fault return code production
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".cfiCOT"))) verifypin_call(void)
{
	initialize();
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status[0] = verifyPIN_11(&result);
	asm("SPUN_verifypin_call_end:");
	if (!oracle()) {
		return SPUN_EXEC_OK;
	} else if (status[0] == CFI_FINAL(verifyPIN_11)) {
		return SPUN_FAULT_INJECTED;
	} else {
		return SPUN_FAULT_DETECTED;
	}
}
#endif

#ifdef VP31
/**
 * @brief VerifyPIN call with CFI protection and fault return code production
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details byteArrayCompare is considered critical
 * Conditionnal execution is not enabled
 */
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
	else if (!oracle()) {
		return SPUN_EXEC_OK;
	}
	else if (status[0] == CFI_FINAL(verifyPIN_31)) {
		return SPUN_FAULT_INJECTED;
    	}
	else {
		return SPUN_FAULT_DETECTED;
	}
		
}
#endif

#ifdef VP32
/**
 * @brief VerifyPIN call with CFI protection and fault return code production
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details byteArrayCompare is considered critical
 * Conditionnal execution is not enabled
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".cfiCOT"))) verifypin_call(void)
{
	initialize();
	volatile BOOL result;

	asm("SPUN_verifypin_call_start:");
	status[0] = verifyPIN_32(&result);
	asm("SPUN_verifypin_call_end:");
	if (status[0] == CFI_WEXEC) {
		return SPUN_FAULT_INJECTED;
	} else if (!oracle()) {
		return SPUN_EXEC_OK;
	}
	else if (status[0] == CFI_FINAL(verifyPIN_32)) {
		return SPUN_FAULT_INJECTED;
    	}
	else {
		return SPUN_FAULT_DETECTED;
	}
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