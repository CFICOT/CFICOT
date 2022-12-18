#ifndef CFICOT_INC_H
#define CFICOT_INC_H
#include <stdint.h>

//Volatile access to a variable
#define CFI_access(variable)		*((volatile typeof(variable) *)&variable)

//Transition step values
#define TOSTEP_1 0x1ebd
#define TOSTEP_2 0x9367
#define TOSTEP_3 0xf67a
#define TOSTEP_4 0xaa9f
#define TOSTEP_5 0x0cda
#define TOSTEP_6 0x1d5f
#define TOSTEP_7 0xc7d9
#define TOSTEP_8 0x9c9b
#define TOSTEP_9 0x734a
#define TOSTEP_10 0x7cbf
#define TOSTEP_11 0xcd6f
#define TOSTEP_12 0xb093
#define TOSTEP_13 0x6e3a
#define TOSTEP_14 0xb754
#define TOSTEP_15 0xd840
#define TOSTEP_16 0x4a3d
#define TOSTEP_17 0xdd09
#define TOSTEP_18 0x8a3e

//Important CFI values
#define CFI_KEY 0x8eb6
#define CFI_ERROR 0x5b34
#define CFI_WEXEC 0x148b
#define CFI_TRUE 0xab54
#define CFI_FALSE 0xc60a
#define CFI_CST 0x176f

//Seeds
#define __CFI_SEED_min_array 0x7ede
#define __CFI_SEED_min_array_call 0x4dfa
#define __CFI_SEED_mem 0xa032
#define __CFI_SEED_mem_call 0xf115
#define __CFI_SEED_sec_mem_set 0x6a09
#define __CFI_SEED_sec_mem_cpy 0xbb67
#define __CFI_SEED_sec_mem_cmp 0x3c6e
#define __CFI_SEED_SIGN 0xcfb5
#define __CFI_SEED_VERIFY 0x8bfd
#define __CFI_SEED_PONCURVE 0x719a

//Merge macros
#define _CFI_MRG(a,b) __CFI_ ## a ## _ ## b
#define CFI_MRG(a,b) _CFI_MRG(a,b)
#define _CFI_SMRG(a,b) __ ## a ## _ ## b
#define CFI_SMRG(a,b) _CFI_SMRG(a,b)

//CFI values macros
#define CFI_FUNC 0
#define _CFI_SEED(A) CFI_SMRG(CFI_SEED,A)
#define CFI_SEED() _CFI_SEED(CFI_FUNC)
#define CFI_STEP(n) _CFI_STEP(n)
#define _CFI_STEP(n) CFI_MRG(CFI_SEED(),STEP_##n)
#define CFI_TOFINAL(n) _CFI_TOFINAL(n)
#define _CFI_TOFINAL(n) CFI_MRG(CFI_SEED(),TOFINAL_##n)
#define CFI_FINAL(func) __CFI_FINAL(func)
#define __CFI_FINAL(func) CFI_MRG(_CFI_SEED(func),FINAL)
#define CFI_TOCOMPENSATE1(n,m,S1,V1) _CFI_TOCOMPENSATE1(n,m,S1,V1)
#define _CFI_TOCOMPENSATE1(n,m,S1,V1) CFI_MRG(CFI_SEED(),TOCOMPENSATE1_##n##_##m##_##S1##_##V1)
//#define CFI_TOCOMPENSATE2(n,m,V1,V2) CFI_##CFI_MRG(CFI_SEED(),TOCOMPENSATE_##n##_##m##_##V1##_##V2)
//#define CFI_TOCOMPENSATE3(n,m,V1,V2,V3) CFI_##CFI_MRG(CFI_SEED(),TOCOMPENSATE_##n##_##m##_##V1##_##V2##_##V3)
//#define CFI_TOCOMPENSATE4(n,m,V1,V2,V3,V4) CFI_##CFI_MRG(CFI_SEED(),TOCOMPENSATE_##n##_##m##_##V1##_##V2##_##V3##_##V4)
#define CFI_MASK16(seed,n) _CFI_MASK16(seed,n)
#define _CFI_MASK16(seed,n) CFI_MRG(seed,MASK16_##n)
#define CFI_MASK32(seed,n) _CFI_MASK32(seed,n)
#define _CFI_MASK32(seed,n) CFI_MRG(seed,MASK32_##n)

//CFI functions macros
#define CFI_GET_SEED(status,TOKEN_SEED) status; if(TOKEN_SEED==CFI_SEED()) \
status = TOKEN_SEED; else status = CFI_ERROR
#define CFI_SET_SEED(status) CFI_SEED()
#define CFI_NEXT_STEP(status,n) CFI_TFunc16(status,TOSTEP_##n)
#define CFI_FEED(status,S,V) CFI_TFunc##S(status,V)
//CFI_TOFINAL(n) such as CFI_TFunc16(status,CFI_TOFINAL(n)) = CFI_TFunc16(status,CFI_TFunc16(SEED(),CFI_KEY))
#define CFI_FINAL_STEP(status,n) CFI_TFunc16(status,CFI_TOFINAL(n))
//CFI_COMPENSATE(n,m,V...) such as CFI_TFunc16(status,CFI_COMPENSATE(n,m,V...)) = CFI_STEP(m)
#define CFI_COMPENSATE_STEP1(status,n,m,S1,V1) CFI_TFunc16(status,CFI_TOCOMPENSATE1(n,m,S1,V1))
#define CFI_COMPENSATE_STEP2(status,n,m,S1,V1,S2,V2) CFI_TFunc16(status,CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2))
#define CFI_COMPENSATE_STEP3(status,n,m,S1,V1,S2,V2,S3,V3) CFI_TFunc16(status,CFI_TOCOMPENSATE3(n,m,S1,V1,S2,V2,S3,V3))
#define CFI_COMPENSATE_STEP4(status,n,m,S1,V1,S2,V2,S3,V3,S4,V4) CFI_TFunc16(status,CFI_TOCOMPENSATE4(n,m,S1,V1,S2,V2,S3,V3,S4,V4))
#define CFI_CHECK_STEP(status,n) status; if(status != CFI_STEP(n)) \
status = CFI_ERROR
#define CFI_CHECK_FINAL(status) status; if(status != CFI_TFunc16(CFI_SEED(),CFI_KEY)) \
status = CFI_ERROR
#define CFI_MASK_DATA16(data,seed,n) (CFI_MASK16(seed,n)) ^ data
#define CFI_UNMASK_DATA16(data,status) data ^ status
#define CFI_MASK_DATA32(data,seed,n) (CFI_MASK32(seed,n)) ^ data
#define CFI_UNMASK_DATA32(data,status) data ^ ((uint32_t)status ^ ((uint32_t)(status >> 16)))

extern uint16_t CFI_TFunc16(uint16_t status, uint16_t Value);

extern uint16_t crc16(uint16_t crc, uint8_t *buffer, uint16_t len);

extern void FAULT_OFF (void);

extern void FAULT_ON (void);

#endif