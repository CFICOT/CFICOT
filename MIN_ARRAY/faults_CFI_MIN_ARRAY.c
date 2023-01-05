#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h> 

#ifdef YACCA
#include "YACCA.h"
#endif
#ifdef CFICOT
#include "CFICOT.h"
#include "MIN_ARRAY/faults_CFI_MIN_ARRAY.cfi.h"
#endif

typedef uint16_t ret_t;
#define LENGTH 10
const uint8_t ref[LENGTH] = {0x10,0x11,0x12,0x13,0x14,0x10,0x11,0x12,0x13,0x14};
const uint8_t b[3*LENGTH]={3,3,3,3,3,3,3,3,3,3,0x20,0x21,0x22,0x23,0x24,0x10,0x11,0x12,0x13,0x14,4,4,4,4,4,4,4,4,4,4};
const uint8_t a[3*LENGTH]={1,1,1,1,1,1,1,1,1,1,0x10,0x11,0x12,0x13,0x14,0x20,0x21,0x22,0x23,0x24,2,2,2,2,2,2,2,2,2,2};
uint8_t x[3*LENGTH];

uint32_t __attribute__ ((noinline,noclone)) fault_dump(int Element)
{
	uint32_t ret;

	switch (Element) {
	case 0:
		ret = (uint32_t)x;
		break;

	case 1:
		ret = sizeof(x);
		break;

	default:
		ret = 0;
		break;
	}

	fault_end((uint32_t)ret);

	return ret;
}

#ifdef NOPROTECTION
uint32_t __attribute__ ((noipa,noinline,noclone,section (".noprot"))) min_array_call(const uint8_t* a, const uint8_t* b, uint8_t* x, int n)
{

	asm("SPUN_min_array_call_start:");

	int i = 0;

	asm("SPUN_min_array_start:");
	while(i < n) {
		if(a[i] < b[i])
		{
		x[i] = a[i];
		}
		else
		{
		x[i] = b[i];
		}
		i ++;
	}
	asm("SPUN_min_array_end:");

	asm("SPUN_min_array_call_end:");


	if (memcmp(x,ref,sizeof(ref))==0)
		return SPUN_EXEC_OK;
	else
		return SPUN_FAULT_INJECTED;
}
#endif


#ifdef YACCA
static uint32_t __attribute__ ((noipa,noinline,noclone,section (".yacca"))) min_array(const uint8_t* a, const uint8_t* b, uint8_t* x, int n)
{
	volatile bool ERR_CODE;
	volatile uint32_t code;
	volatile int i = 0;
	volatile int i1;
	volatile int error = 0;

	code = B0;
	ERR_CODE = 0;

	asm("SPUN_min_array_start:");
	ERR_CODE |= (code != B0);
	code = code ^ (B0 ^B1);
	while (i < n) {
		ERR_CODE |= ((code != B1) && (code != B9)) || (i>=n);
		code = (code & M1_1_9) ^ M2_1_9_2;
		if (a[i] < b[i]) {
			ERR_CODE |= (code != B2) || (a[i] >= b[i]);
			code = code ^ (B2 ^ B3);
			x[i] = a[i];
			ERR_CODE |= (code != B3);
			code = code ^ (B3 ^ B4);
		} else {
			ERR_CODE |= (code != B2) || (a[i] < b[i]);
			code = code ^ (B2 ^ B5);
			x[i] = b[i];
			ERR_CODE |= (code != B5);
			code = code ^ (B5 ^ B6);
		}
		ERR_CODE |= (code != B4) && (code != B6);
		code = (code & M1_4_6) ^ M2_4_6_7;
		i1=i; i ++; i1++;
		code = code ^ (B7 ^ B8);
		ERR_CODE |= (code != B8)||(i!=i1);
		code = code ^ (B8 ^ B9);
	}
	ERR_CODE |= ((code != B1) && (code != B9)) || (i<n);
	if (ERR_CODE) {
		error=1;
	}
	asm("SPUN_min_array_end:");

	return error;
}

uint32_t __attribute__ ((noipa,noinline,noclone,section (".yacca"))) min_array_call(const uint8_t* a, const uint8_t* b, uint8_t* x, int n) {
	
	volatile int error = 0;

	asm("SPUN_min_array_call_start:");

	error = min_array(a,b,x,n);

	asm("SPUN_min_array_call_end:");

	if (error)
		return SPUN_FAULT_DETECTED;
	else if (memcmp(x,ref,sizeof(ref)) == 0)
		return SPUN_EXEC_OK;
	else
		return SPUN_FAULT_INJECTED;
}
#endif

#ifdef CFICOT
#undef CFI_FUNC
#define CFI_FUNC min_array
uint16_t __attribute__ ((noipa,noinline,noclone,section (".cficot"))) min_array(const uint8_t* a, const uint8_t* b, uint8_t* x, int n) 
{
	volatile int i = 0;
	volatile uint16_t status;
	volatile uint16_t sum = CFI_CST;

	asm("SPUN_min_array_start:");
	status = CFI_SET_SEED(status);

	while(i < n) {
		if(a[i] < b[i]) {
			x[i] = a[i];
			sum = sum + (CFI_access(a[i]) > CFI_access(b[i]));
			sum = sum + (uint16_t)CFI_access(a[i]);
		} else {
			x[i] = b[i];
			sum = sum + (CFI_access(a[i]) < CFI_access(b[i]));
			sum = sum + (uint16_t)CFI_access(b[i]);
		}
		sum = sum - (uint16_t)CFI_access(x[i]);
		i++;
	}
	status = CFI_FEED(status, 16, sum + n - i);
	status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_CST);
	status = CFI_FINAL_STEP(status, 1);
	status = CFI_CHECK_FINAL(status);
	asm("SPUN_min_array_end:");
	return status;
}

#undef CFI_FUNC
#define CFI_FUNC min_array_call
uint32_t __attribute__ ((noipa,noinline,noclone,section (".cficot"))) min_array_call(const uint8_t* a, const uint8_t* b, uint8_t* x, int n) {

	volatile uint16_t status;

	asm("SPUN_min_array_call_start:");

	status = CFI_SET_SEED(status);

	status = CFI_FEED(status, 16, min_array(a,b,x,n));
    
	status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_FINAL(min_array));
	status = CFI_FINAL_STEP(status, 1);
	status = CFI_CHECK_FINAL(status);

	asm("SPUN_min_array_call_end:");

    	if (status == CFI_ERROR)
        	return SPUN_FAULT_DETECTED;
    	else if (memcmp(x,ref,sizeof(ref))==0)
        	return SPUN_EXEC_OK;
	else
		return SPUN_FAULT_INJECTED;
}
#endif


/**
 * @brief Executes the fault testing scenario for the min_array function
 *
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
int main(void)
{
	memset(x,0xFF,3*LENGTH);

	fault_end(min_array_call(a+10,b+10,x+10,LENGTH));

	fault_end(SPUN_SETUP_PROBLEM);
	exit(0);
}