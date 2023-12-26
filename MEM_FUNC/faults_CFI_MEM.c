/**
 * @file Memory access functions fault injection scenario
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef CFICOT
#include "CFICOT.h"
#include "MEM_FUNC/faults_CFI_MEM.cfi.h"
#endif

/**
 * @brief global variables used in memory functions scenarios
 * @param a input array
 * @param b input array
 * @param c constant used to erase data with memset
 * @param x output array
 * @param ref1 reference array
 * @param ref2 reference array
 * @param CFI_F_Integrity array for binary integrity checking (see Edit_CFI_F_Integrity_elf.py)
 */
typedef uint16_t ret_t;
#define LENGTH 10
#define c 0x55
volatile uint16_t __attribute__((section(".CFI_F_Integrity"))) CFI_F_Integrity[5] = {0,0,0,0,0};
const uint8_t ref1[3 * LENGTH] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, c, c, c, c, c, c, c, c, c, c, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
const uint8_t ref2[3 * LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x20, 0x21, 0x22, 0x23, 0x24, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t a[3 * LENGTH] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0x10, 0x11, 0x12, 0x13, 0x14, 0x20, 0x21, 0x22, 0x23, 0x24, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
uint8_t b[3 * LENGTH] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0x10, 0x11, 0x12, 0x13, 0x18, 0x20, 0x21, 0x22, 0x23, 0x24, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
uint8_t x[3 * LENGTH];
volatile int OPcmp;

/**
 * @brief array_descriptor: structure to describe an array
 * @param arr array address
 * @param arrsize array size
 * @param integrity array integrity
 */
#ifdef CFICOT
typedef struct {
	void *arr;
	size_t arrsize;
	uint16_t integrity;
} array_descriptor;

/**
 * @brief mem_set_struct: structure to describe a mem_set input
 * @param arr array to be set
 * @param cst constant used to set data
 * @param integrity structure integrity
 */
typedef struct {
	array_descriptor *arr;
	int cst;
	uint16_t integrity;
} mem_set_struct;

/**
 * @brief mem_cpy_struct: structure to describe a mem_cpy input
 * @param arrdest destination array 
 * @param arrsource source array 
 * @param integrity structure integrity
 */
typedef struct {
	array_descriptor *arrdest;
	array_descriptor *arrsource;
	uint16_t integrity;
} mem_cpy_struct;

/**
 * @brief mem_cmp_struct: structure to describe a mem_cmp input
 * @param arrdest first array 
 * @param arrsource second array 
 * @param result resulting comparison
 * @param integrity structure integrity
 */
typedef struct {
	array_descriptor *arr1;
	array_descriptor *arr2;
	volatile int *result;
	uint16_t integrity;
} mem_cmp_struct;

typedef uint16_t (*memset_f)(mem_set_struct*);
typedef uint16_t (*memcpy_f)(mem_cpy_struct*);
typedef uint16_t (*memcmp_f)(mem_cmp_struct*);

/**
 * @brief compute array integrity
 * @param[out] desc structure array descriptor
 * @return uint16_t the computed integrity
 * @details compute and initialize the integrity of the given input array_descriptor
 */
uint16_t __attribute__((noinline, noipa, noclone)) compute_integrity(array_descriptor * desc) {
	return (uint16_t)desc->arrsize + (uint16_t)(uintptr_t)desc->arr;
}

/**
 * @brief check array integrity
 * @param[out] desc structure array descriptor
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details verify the integrity of the given input array_descriptor
 */
uint16_t __attribute__((noinline, noipa, noclone)) check_integrity(array_descriptor * desc) {
	if (desc->integrity == (desc->arrsize + (uint16_t)(uintptr_t)desc->arr)) {
		return CFI_TRUE;
	} else {
		return CFI_FALSE;
	}
}

/**
 * @brief compute mem_set input integrity
 * @param[out] desc mem_set input structure
 * @return uint16_t the computed integrity
 * @details computes the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) compute_integrity_set(mem_set_struct * desc) {
	return desc->arr->integrity + (uint16_t)desc->cst;
}

/**
 * @brief compute mem_cpy input integrity
 * @param[out] desc mem_cpy input structure
 * @return uint16_t the computed integrity
 * @details computes the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) compute_integrity_cpy(mem_cpy_struct * desc) {
	return desc->arrsource->integrity + desc->arrdest->integrity;
}

/**
 * @brief compute mem_cmpinput integrity
 * @param[out] desc mem_cmp input structure
 * @return uint16_t the computed integrity
 * @details computes the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) compute_integrity_cmp(mem_cmp_struct * desc) {
	return desc->arr1->integrity + desc->arr2->integrity + (uint16_t)(uintptr_t)desc->result;
}

/**
 * @brief check mem_set input integrity
 * @param[out] desc mem_set input structure
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details verify the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) check_integrity_set(mem_set_struct * desc) {
	if ((desc->integrity == desc->arr->integrity + desc->cst) && (check_integrity(desc->arr) == CFI_TRUE)){
		return CFI_TRUE;
	} else {
		return CFI_FALSE;
	}
}

/**
 * @brief check mem_cpy input integrity
 * @param[out] desc mem_cpy input structure
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details verify the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) check_integrity_cpy(mem_cpy_struct * desc) {
	if ((desc->integrity == desc->arrsource->integrity + desc->arrdest->integrity) 
	&& (check_integrity(desc->arrsource) == CFI_TRUE) 
	&& (check_integrity(desc->arrdest) == CFI_TRUE)) {
		return CFI_TRUE;
	} else {
		return CFI_FALSE;
	}
}

/**
 * @brief check mem_cmp input integrity
 * @param[out] desc mem_cmp input structure
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details verify the integrity of the given input structure
 */
uint16_t __attribute__((noinline, noipa, noclone)) check_integrity_cmp(mem_cmp_struct * desc) {
	if ((desc->integrity == desc->arr1->integrity + desc->arr2->integrity + (uint16_t)(uintptr_t)desc->result) 
	&& (check_integrity(desc->arr1) == CFI_TRUE) 
	&& (check_integrity(desc->arr2) == CFI_TRUE)){
		return CFI_TRUE;
	} else {
		return CFI_FALSE;
	}
}

#endif

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
		ret = (uint32_t)x;
		break;

	case 1:
		ret = sizeof(x);
		break;

	case 2:
		ret = (uint32_t)b;
		break;

	case 3:
		ret = sizeof(b);
		break;

	default:
		ret = 0;
		break;
	}

	fault_end((uint32_t)ret);

	return ret;
}

#ifdef NOPROTECTION
/**
 * @brief unprotected mem_set function
 * @param[in] tab array to be set
 * @param[in] cst set constant
 * @param[in] size array size
 * @details The mem_set function  copies  the  value  of cst (converted  to  an unsigned char) into
 * each of the first size characters of the object pointed to by tab.
 */
void* __attribute__((noipa, noinline, noclone, section(".noprot"))) mem_set(void *tab, int cst, size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; i++)
	{
		((unsigned char *)tab)[i] = (unsigned char)cst;
	}
	return tab;
}

/**
 * @brief unprotected mem_cpy function
 * @param[in] tabsource source array
 * @param[in] tabdest destination array
 * @param[in] size array size
 * @details The mem_cpy function  copies size characters  from  the  object  pointed  to  by tabsource into  the
 * object pointed to by tabdest. If copying takes place between objects that overlap, the behavior
 * is undefined.
 */
void* __attribute__((noipa, noinline, noclone, section(".noprot"))) mem_cpy(void *tabdest, void *tabsource, size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; i++)
	{
		((unsigned char *)tabdest)[i] = ((unsigned char *)tabsource)[i];
	}
	return tabdest;
}

/**
 * @brief unprotected mem_cmp function
 * @param[in] tab1 first array
 * @param[in] tab2 second array
 * @param[in] size array size
 * @return The mem_cmp function  returns  an  integer  greater  than,  equal  to,  or  less  than  zero,
 * accordingly as the object pointed to by tab1 is greater than, equal to, or less than the object
 * pointed to by tab2.
 * @details The mem_cmp function  compares  the  first size characters  of  the  object  pointed  to  by tab1 to
 * the first size characters of the object pointed to by tab2
 */
int __attribute__((noipa, noinline, noclone, section(".noprot"))) mem_cmp( void *tab1, void *tab2, size_t size)
{
	unsigned char *tab11 = (unsigned char *)tab1;
	unsigned char *tab22 = (unsigned char *)tab2;
	size_t i = 0;
	if (size == 0) {
		return 0;
	}
	while ((*tab11 == *tab22) && (i < size)) {
		tab11++;
		tab22++;
		i++;
	}
	if(i < size)
		return *tab11 - *tab22;
	else
		return 0;
}
#endif

#ifdef CFICOT
/**
 * @brief protected mem_set function
 * @param[inout] DMEM mem_set input structure
 * @return The sec_mem_set function returns a status value ensuring security requirements.
 * @details -The sec_mem_set function  copies  the  value  of cst (converted  to  an unsigned char) into
 * each of the first size characters of the object pointed to by tab.
 * security features:
 * -We ensure that "size" iterations have been made
 * -At each iteration step, we ensure that the assignment to cst is made
 */
#undef CFI_FUNC
#define CFI_FUNC sec_mem_set
uint16_t __attribute__((noipa, noinline, noclone, section(".sec_mem_set"))) sec_mem_set(mem_set_struct *DMEM)
{
	volatile size_t i = 0;
	volatile size_t j;
	volatile uint16_t status;
	volatile uint16_t sum = CFI_CST;

	CFI_SET_SEED(status);

	//check the integrity of the structure input
	CFI_FEED(status, 16, check_integrity_set(DMEM));
	CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_TRUE);
	CFI_CHECK_STEP(status,1);

	//additionnal counter
	j = CFI_access(DMEM->arr->arrsize);
	sum ^= CFI_access(DMEM->arr->arrsize);

	for (i = 0; i < DMEM->arr->arrsize; i++)
	{	
		sum += i;
		if((i+j) != CFI_access(DMEM->arr->arrsize)) {
			return CFI_ERROR;
		}
		j--;
		((unsigned char *)DMEM->arr->arr)[i] = (unsigned char)CFI_access(DMEM->cst);
		sum = sum + (uint16_t)((unsigned char *)DMEM->arr->arr)[i];
		sum = sum - (uint16_t)CFI_access(DMEM->cst);
	}
	sum -= CFI_access(DMEM->arr->arrsize)*(CFI_access(DMEM->arr->arrsize)-1)/2;
	sum += j;
	sum ^= i;
	//check the validity of the sum invariant (it should be equal to CFI_CST)
	CFI_FEED(status, 16, sum);
	CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_CST);
	CFI_CHECK_STEP(status,2);
	CFI_FINAL_STEP(status, 2);
	CFI_CHECK_FINAL(status);

	return status;
}

/**
 * @brief protected mem_cpy function
 * @param[inout] DMEM mem_cpy input structure
 * @return The sec_mem_cpy function returns a status value ensuring security requirements.
 * @details The sec_mem_cpy function  copies size characters  from  the  object  pointed  to  by tabsource into  the
 * object pointed to by tabdest. If copying takes place between objects that overlap, the behavior
 * is undefined.
 * security features:
 * -We ensure that "size" iterations have been made
 * -At each iteration step, we ensure that the copy of the current element is made
 */
#undef CFI_FUNC
#define CFI_FUNC sec_mem_cpy
uint16_t __attribute__((noipa, noinline, noclone, section(".sec_mem_cpy"))) sec_mem_cpy(mem_cpy_struct *DMEM)
{
	volatile size_t i = 0;
	volatile size_t j;
	volatile uint16_t status;
	volatile uint16_t sum = CFI_CST;

	CFI_SET_SEED(status);

	//check the integrity of the structure input
	CFI_FEED(status, 16, check_integrity_cpy(DMEM));
	CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_TRUE);
	CFI_CHECK_STEP(status,1);

	if ((DMEM->arrsource->arrsize != DMEM->arrdest->arrsize) || (DMEM->arrsource == DMEM->arrdest)) {
		return CFI_ERROR;
	}

	//additionnal counter
	j = CFI_access(DMEM->arrdest->arrsize);
	sum ^= CFI_access(DMEM->arrdest->arrsize);

	for (i = 0; i < DMEM->arrdest->arrsize; i++) {
		sum += i;
		if((i+j) != CFI_access(DMEM->arrdest->arrsize)) {
			return CFI_ERROR;
		}
		j--;
		((uint8_t *)(DMEM->arrdest->arr))[i] = ((uint8_t *)(DMEM->arrsource->arr))[i];
		sum ^= ((uint8_t *)(DMEM->arrdest->arr))[i];
		sum ^= ((uint8_t *)(DMEM->arrsource->arr))[i];
	}
	sum -= CFI_access(DMEM->arrdest->arrsize)*(CFI_access(DMEM->arrdest->arrsize)-1)/2;
	sum += j;
	sum ^= i;
	//check the validity of the sum invariant (it should be equal to CFI_CST)
	CFI_FEED(status, 16, sum);
	CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_CST);
	CFI_CHECK_STEP(status,2);
	CFI_FINAL_STEP(status, 2);
	CFI_CHECK_FINAL(status);
	return status;
}

/**
 * @brief protected mem_cmp function
 * @param[inout] DMEM
 * @return The sec_mem_cmp function returns a status value ensuring security requirements.
 * @details The sec_mem_cmp function  compares  the  first size characters  of  the  object  pointed  to  by tab1 to
 * the first size characters of the object pointed to by tab2
 * The sec_mem_cmp function store the comparison result as an  integer  greater  than,  equal  to,  or  less  than  zero,
 * accordingly as the object pointed to by tab1 is equal to the object pointed to by tab2 or not
 * The sec_mem_cmp function returns a status value ensuring security requirements.
 * security features:
 * -We ensure that "size" iterations have been made
 * -We ensure that comparison isn't trivial, ie tab1 and tab2 points towards distincts objects of non null length
 * -At each iteration step, we ensure that the comparison is correctly made beetween the 2 objects
 */
#undef CFI_FUNC
#define CFI_FUNC sec_mem_cmp
uint16_t __attribute__((noipa, noinline, noclone, section(".sec_mem_cmp"))) sec_mem_cmp(mem_cmp_struct *DMEM)
{
	volatile int tmp;
	volatile uint16_t status;
	volatile uint16_t sum;
	volatile unsigned char *tab11 = (unsigned char *)DMEM->arr1->arr;
	volatile unsigned char *tab22 = (unsigned char *)DMEM->arr2->arr;
	volatile size_t i = 0;
	volatile size_t j;
	sum = CFI_MASK(CFI_CST,sum,16);
	

	if(check_integrity_cmp(DMEM) != CFI_TRUE)
		return CFI_ERROR;
	if((DMEM->arr1->arrsize == 0) || (DMEM->arr1->arrsize != DMEM->arr2->arrsize) || (*tab11 == *tab22))
		return CFI_ERROR;
	
	*(DMEM->result) = CFI_CST;

	CFI_SET_SEED(status);
	CFI_NEXT_STEP(status,1);
	CFI_CHECK_STEP(status,1);

	if(DMEM->arr1 == DMEM->arr2)
		return CFI_ERROR;

	sum = CFI_UNMASK(sum,status,16,1);
	if(sum != CFI_CST) {
		return CFI_ERROR;
	}

	//additionnal counter
	j = CFI_access(DMEM->arr1->arrsize);
	sum ^= CFI_access(DMEM->arr1->arrsize);

	while (i < DMEM->arr1->arrsize) {
		sum += i;
		if((i+j) != CFI_access(DMEM->arr1->arrsize)) {
			return CFI_ERROR;
		}
		tmp = (*tab11) - (*tab22);
		*(DMEM->result) += tmp + i;
		sum += tmp -(*tab11 - *tab22);
		tab11++;
		tab22++;
		j--;
		i++;
	}
	*(DMEM->result) = *(DMEM->result) - (CFI_CST + ((DMEM->arr1->arrsize * (DMEM->arr1->arrsize-1))/2));
	sum -= CFI_access(DMEM->arr1->arrsize)*(CFI_access(DMEM->arr1->arrsize)-1)/2;
	sum += j;
	sum ^= i;
	CFI_FEED(status, 16, sum);
	CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_CST);
	CFI_CHECK_STEP(status,2);
	CFI_FINAL_STEP(status, 2);
	CFI_CHECK_FINAL(status);
	return status;
}
#endif

/**
 * @brief Call function for unprotected scenario
 * @param[in] tab1 first array
 * @param[in] tab2 second array
 * @param[in] tab3 third array
 * @param[in] cst constant for mem_set
 * @param[in] size array size
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details call mem_set mem_cpy and mem_cmp then return the resulting fault scenario
 */
#ifdef NOPROTECTION
uint32_t __attribute__((noipa, noinline, noclone, section(".noprot"))) mem_call(uint8_t *tab1, uint8_t *tab2, uint8_t *tab3, uint8_t cst, size_t size)
{

	asm("SPUN_mem_call_start:");

	int OPcmp;
	OPcmp = 0;

	asm("SPUN_mem_start:");
	asm("SPUN_mem_set_start:");
	tab2 = mem_set(tab2,cst,size);
	asm("SPUN_mem_set_end:");
	asm("SPUN_mem_cpy_start:");
	tab3 = mem_cpy(tab3,tab1,size);
	asm("SPUN_mem_cpy_end:");
	asm("SPUN_mem_cmp_start:");
	OPcmp = mem_cmp(tab3,tab2,size);
	asm("SPUN_mem_cmp_end:");
	asm("SPUN_mem_end:");

	asm("SPUN_mem_call_end:");

	if ((memcmp(x, ref2, sizeof(ref2)) == 0) && (memcmp(b, ref1, sizeof(ref1)) == 0) && (OPcmp != 0))
		return SPUN_EXEC_OK;
	else
		return SPUN_FAULT_INJECTED;
}
#endif

#ifdef CFICOT
/**
 * @brief Array for pointer editing (see EDIT_CFI_POINTER.py)
 */
volatile const uint32_t __attribute__((section(".CFI_F_Pointer"))) CFI_F_Pointer[5] = {(uintptr_t)sec_mem_set,(uintptr_t)sec_mem_cpy,(uintptr_t)sec_mem_cmp,0,0};

/**
 * @brief Call function for unprotected scenario
 * @param[in] mems mem_set input structure
 * @param[in] memcp mem_cpy input structure
 * @param[in] memcm mem_cmp input structure
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details call mem_set mem_cpy and mem_cmp then return the CFI code
 */
#undef CFI_FUNC
#define CFI_FUNC mem
uint16_t __attribute__((noipa, noinline, noclone, section(".mem"))) mem(mem_set_struct *mems, mem_cpy_struct *memcp, mem_cmp_struct *memcm)
{
	volatile uint16_t status;
#ifdef MPOINTER
	volatile uintptr_t pmset;
	volatile uintptr_t pmcpy;
	volatile uintptr_t pmcmp;
#endif

	if(check_integrity_set(mems) != CFI_TRUE)
		return CFI_ERROR;
	if(check_integrity_cpy(memcp) != CFI_TRUE)
		return CFI_ERROR;
	if(check_integrity_cmp(memcm) != CFI_TRUE)
		return CFI_ERROR;


	CFI_SET_SEED(status);

	asm("SPUN_mem_set_start:");
#ifdef MPOINTER
	pmset = CFI_UNMASK_POINTER(pmset, 0, status, 32, 0);
	CFI_FEED(status, 16, ((memset_f)(pmset))(mems));
#else
	CFI_FEED(status, 16, sec_mem_set(mems));
#endif
	asm("SPUN_mem_set_end:");
	CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_FINAL(sec_mem_set));
	CFI_CHECK_STEP(status,1);
	asm("SPUN_mem_cpy_start:");
#ifdef MPOINTER
	pmcpy = CFI_UNMASK_POINTER(pmcpy, 1, status, 32, 1);
	CFI_FEED(status, 16, ((memcpy_f)(pmcpy))(memcp));
#else
	CFI_FEED(status, 16, sec_mem_cpy(memcp));
#endif
	asm("SPUN_mem_cpy_end:");
	CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_FINAL(sec_mem_cpy));
	CFI_CHECK_STEP(status,2);
	asm("SPUN_mem_cmp_start:");
#ifdef MPOINTER
	pmcmp = CFI_UNMASK_POINTER(pmcmp, 2, status, 32, 2);
	CFI_FEED(status, 16, ((memcmp_f)(pmcmp))(memcm));
#else
	CFI_FEED(status, 16, sec_mem_cmp(memcm));
#endif
	asm("SPUN_mem_cmp_end:");
	CFI_COMPENSATE_STEP1(status, 2, 3, 16, CFI_FINAL(sec_mem_cmp));
	CFI_CHECK_STEP(status,3);

	CFI_FINAL_STEP(status, 3);
	CFI_CHECK_FINAL(status);

	return status;
}

/**
 * @brief Call function for unprotected scenario
 * @param[in] tab1 first array
 * @param[in] tab2 second array
 * @param[in] tab3 third array
 * @param[in] cst constant for mem_set
 * @param[in] size array size
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details call mem_set mem_cpy and mem_cmp then return the resulting fault scenario
 */
#undef CFI_FUNC
#define CFI_FUNC mem_call
extern unsigned char __start_section_mem_call;
extern unsigned char __stop_section_mem_call;
uint32_t __attribute__((noipa, noinline, noclone, section("section_mem_call"))) mem_call(uint8_t *tab1, uint8_t *tab2, uint8_t *tab3, uint8_t cst, size_t size)
{
	OPcmp = 1;
	volatile uint16_t status;
#ifdef CHECK_BINARY_INTEGRITY
	volatile uint16_t CRC_integrity = 0, check_integrity_CRC = 1;
#endif

	asm("SPUN_mem_call_start:");

	array_descriptor tab11, tab22, tab33;
	mem_set_struct mems;
	mem_cpy_struct memcp;
	mem_cmp_struct memcm;

	CFI_SET_SEED(status);

	tab11.arr = tab1;
	tab11.arrsize = size;
	tab11.integrity = compute_integrity(&tab11);
	if ((tab11.arr != tab1) || 
			(tab11.arrsize != size)) {
		status = CFI_ERROR;
		goto mem_call_end;
	}

	CFI_FEED(status, 16, check_integrity(&tab11));
	CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_TRUE);

	tab22.arr = tab2;
	tab22.arrsize = size;
	tab22.integrity = compute_integrity(&tab22);
	if ((tab22.arr != tab2) || 
			(tab22.arrsize != size)) {
		status = CFI_ERROR;
		goto mem_call_end;
	}

	CFI_FEED(status, 16, check_integrity(&tab22));
	CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_TRUE);

	tab33.arr = tab3;
	tab33.arrsize = size;
	tab33.integrity = compute_integrity(&tab33);
	if ((tab33.arr != tab3) || 
			(tab33.arrsize != size)) {
		status = CFI_ERROR;
		goto mem_call_end;
	}

	CFI_FEED(status, 16, check_integrity(&tab33));
	CFI_COMPENSATE_STEP1(status, 2, 3, 16, CFI_TRUE);

	mems.arr = &tab22;
	mems.cst = cst;
	mems.integrity = compute_integrity_set(&mems);
	if ((mems.arr != &tab22) || (CFI_access(mems.cst) != CFI_access(cst))) {
		status = CFI_ERROR;
		goto mem_call_end;
	}

	CFI_FEED(status, 16, check_integrity_set(&mems));
	CFI_COMPENSATE_STEP1(status, 3, 4, 16, CFI_TRUE);

	memcp.arrdest = &tab33;
	memcp.arrsource = &tab11;
	
	if ((CFI_access(memcp.arrdest) != &tab33) || 
			(CFI_access(memcp.arrsource) != &tab11)) {
		status = CFI_ERROR;
		goto mem_call_end;
	} else {
		memcp.integrity = compute_integrity_cpy(&memcp);
	}

	CFI_FEED(status, 16, check_integrity_cpy(&memcp));
	CFI_COMPENSATE_STEP1(status, 4, 5, 16, CFI_TRUE);

	memcm.arr1 = &tab33;
	memcm.arr2 = &tab22;
	memcm.result = &OPcmp;
	memcm.integrity = compute_integrity_cmp(&memcm);
	if ((memcm.arr1 != &tab33) || 
			(memcm.arr2 != &tab22) || 
			(memcm.result != &OPcmp)) {
		status = CFI_ERROR;
		goto mem_call_end;
	}

	CFI_FEED(status, 16, check_integrity_cmp(&memcm));
	CFI_COMPENSATE_STEP1(status, 5, 6, 16, CFI_TRUE);

	asm("SPUN_mem_start:");
	CFI_FEED(status, 16, mem(&mems, &memcp, &memcm));
	asm("SPUN_mem_end:");

	CFI_COMPENSATE_STEP1(status, 6, 7, 16, CFI_FINAL(mem));

#ifdef CHECK_BINARY_INTEGRITY
	CRC_integrity = crc16(CRC_integrity, &__start_section_mem_call, &__stop_section_mem_call - &__start_section_mem_call);
	check_integrity_CRC = CRC_integrity ^ CFI_F_Integrity[0] ^ CFI_CST;

	CFI_FEED(status, 16, check_integrity_CRC);
	CFI_COMPENSATE_STEP1(status, 7, 8, 16, CFI_CST);
#else
	CFI_NEXT_STEP(status,8);
#endif

	CFI_FINAL_STEP(status, 8);
mem_call_end:

	asm("SPUN_mem_call_end:");

	if (status != CFI_TFunc16(CFI_SEED(),CFI_KEY))
		return SPUN_FAULT_DETECTED;
	else if ((memcmp(x, ref2, sizeof(ref2)) == 0) && (memcmp(b, ref1, sizeof(ref1)) == 0) && (OPcmp != 0))
		return SPUN_EXEC_OK;
	else
		return SPUN_FAULT_INJECTED;
}
#endif

/**
 * @brief Executes the fault testing scenario for the mem functions
 *
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
int main(void)
{
	memset(x, 0xFF, 3 * LENGTH);

	fault_end(mem_call(a + LENGTH, b + LENGTH, x + LENGTH, c, LENGTH));

	fault_end(SPUN_SETUP_PROBLEM);
	exit(0);
}