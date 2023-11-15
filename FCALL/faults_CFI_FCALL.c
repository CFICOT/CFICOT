/**
 * @file FCall fault injection scenario
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef CFICOT
#include "CFICOT.h"
#include "FCALL/faults_CFI_FCALL.cfi.h"
#endif

typedef uint16_t ret_t;
volatile uint16_t status_F, status_G;

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
	default:
		ret = 0;
		break;
	}

	fault_end((uint32_t)ret);

	return ret;
}

#ifdef NOPROTECTION
/**
 * @brief Dummy unprotected function G
 * @return 1
 */
uint16_t __attribute__((noinline,noclone)) Function_G(void) {
	return 1;
}

/**
 * @brief Dummy unprotected function F
 * @return 1 if G is executed 0 if G's execution is skipped
 * @details call function G
 */
uint16_t __attribute__((noipa, noinline, noclone, section(".noprot"))) Function_F(void){
	ret_t ret = 0;

	ret = Function_G();

	return ret;
}

/**
 * @brief Call function for unprotected scenario
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details call function F and return the resulting fault scenario
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".noprot"))) Func_call(void) {
	ret_t ret =-1;

	asm("SPUN_Function_F_start:");
	ret = Function_F();
	asm("SPUN_Function_F_end:");
	if(ret == 1) {
		return SPUN_EXEC_OK;
	} else {
		return SPUN_FAULT_INJECTED;
	}
}
#endif

/**
 * @brief Dummy protected function G
 * @param[in] token execution token for conditionnal execution functionnality
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details check the token, compute the final status and return it
 */
#ifdef CFICOT
#undef CFI_FUNC
#define CFI_FUNC Function_G
uint16_t __attribute__((noinline,noclone, section(".COT"))) Function_G(uint16_t token) {
	volatile uint16_t status = CFI_ERROR;

	status = CFI_GET_SEED(status,token);

	status = CFI_NEXT_STEP(status,1);
	status = CFI_CHECK_STEP(status,1);
	status = CFI_FINAL_STEP(status,1);
	status = CFI_CHECK_FINAL(status);

	return status;
}

/**
 * @brief Dummy protected function F
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details compute the token, call function G and return the CFI status
 */
#undef CFI_FUNC
#define CFI_FUNC Function_F
uint16_t __attribute__((noipa, noinline, noclone, section(".COT"))) Function_F(void){
	volatile uint16_t status, token;

	status = CFI_SET_SEED(status);

	token = status ^ _CFI_SEED(Function_G) ^ _CFI_SEED(Function_F);

	status_G = Function_G(token);

	status = CFI_FEED(status, 16, status_G);
	status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_FINAL(Function_G));
	status = CFI_CHECK_STEP(status,1);

	status = CFI_NEXT_STEP(status,2);
	status = CFI_CHECK_STEP(status,2);

	status = CFI_FINAL_STEP(status,2);
	status = CFI_CHECK_FINAL(status);

	return status;
}

/**
 * @brief Call function for protected scenario
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details call function F and return the resulting fault scenario
 */
#undef CFI_FUNC
#define CFI_FUNC Func_call
uint32_t __attribute__((noipa, noinline, noclone, section(".COT"))) Func_call(void) {

	asm("SPUN_Function_F_start:");
	status_F = Function_F();

	asm("SPUN_Function_F_end:");
	if(status_F == CFI_FINAL(Function_F)) {
		if (status_G == CFI_FINAL(Function_G)) {
			return SPUN_EXEC_OK;
		} else {
			return SPUN_FAULT_INJECTED;
		}
	} else {
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
	fault_end(Func_call());

	fault_end(SPUN_SETUP_PROBLEM);
	exit(0);
}