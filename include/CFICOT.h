/**
 * @file main Control Flow Intergity file
 */
#ifndef CFICOT_INC_H
#define CFICOT_INC_H
#include <stdint.h>
#include <CFISEED.h>

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

/**
 * @brief Important CFI values
 * @param CFI_KEY Final CFI value computation
 * @param CFI_ERROR Erase current status in case of error detection
 * @param CFI_WEXEC Test complete execution of a function
 * @param CFI_TRUE 16-bit True
 * @param CFI_FALSE 16-bit False
 * @param CFI_CST Constant to avoid 0 with XOR transition function
 */
#define CFI_KEY 0x8eb6
#define CFI_ERROR 0x5b34
#define CFI_WEXEC 0x148b
#define CFI_TRUE 0xab54
#define CFI_FALSE 0xc60a
#define CFI_CST 0x176f

/**
 * @brief Merge macros
 * @details intermediate macros to build CFI variables names
 */
#define _CFI_MRG(a,b) __CFI_ ## a ## _ ## b
#define CFI_MRG(a,b) _CFI_MRG(a,b)
#define _CFI_SMRG(a,b) __ ## a ## _ ## b
#define CFI_SMRG(a,b) _CFI_SMRG(a,b)

/**
 * @brief CFI values macros
 * @param CFI_FUNC
 * @param CFI_SEED()
 * @param CFI_STEP(n)
 * @param CFI_TOFINAL(n)
 * @param CFI_FINAL(func)
 * @param CFI_TOCOMPENSATE1(n,m,S1,V1)
 * @param CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2)
 * @details Define the CFI values name for precomputation in ".cfi.h" files
 */
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
#define CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2) _CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2)
#define _CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2) CFI_MRG(CFI_SEED(),TOCOMPENSATE2_##n##_##m##_##S1##_##V1##_##S2##_##V2)

/**
 * @brief Initialize status to seed based on token validity
 * @param status CFI tracking variable "CoT"
 * @param TOKEN_SEED execution token
 * @return updated status
 * @details if token is equals to the expected seed, satatus is initialized 
 * with token
 */
#define CFI_GET_SEED(status,TOKEN_SEED) status; if(TOKEN_SEED==CFI_SEED()) \
status = TOKEN_SEED; else return CFI_ERROR

/**
 * @brief Initialize status to seed
 * @param status CFI tracking variable "CoT"
 * @return updated status
 * @details Initialize status to the function corresponding seed
 */
#define CFI_SET_SEED(status) CFI_SEED()

/**
 * @brief Increase current CFI step
 * @param status CFI tracking variable "CoT"
 * @param n the number of the destination step
 * @return updated status
 * @details update status from step n-1 to step n
 */
#define CFI_NEXT_STEP(status,n) CFI_TFunc16(status,TOSTEP_##n)

/**
 * @brief update status with value V
 * @param status CFI tracking variable "CoT"
 * @param S size of value V
 * @param V value to be applied to the current status
 * @return updated status
 * @details include value V into the computation of the CoT
 */
#define CFI_FEED(status,S,V) CFI_TFunc##S(status,V)

/**
 * @brief Final update of the status
 * @param status CFI tracking variable "CoT"
 * @param n current CoT step
 * @return updated status
 * @details update the status from the n th step to the final step
 * CFI_TOFINAL(n) such that:
 * CFI_TFunc16(CFI_STEP(n),CFI_TOFINAL(n)) = CFI_TFunc16(CFI_SEED(),CFI_KEY)
 */
#define CFI_FINAL_STEP(status,n) CFI_TFunc16(status,CFI_TOFINAL(n))

/**
 * @brief Compensation of a previously fed value
 * @param status CFI tracking variable "CoT"
 * @param n source step
 * @param m destination step
 * @param S1 size of value V1
 * @param V1 value to compensate
 * @return updated status
 * @details update the status from the n th step + a fed value to the m th step
 * CFI_COMPENSATE(n,m,V...) such that 
 * CFI_TFunc16(status,CFI_COMPENSATE(n,m,V...)) = CFI_STEP(m)
 */
#define CFI_COMPENSATE_STEP1(status,n,m,S1,V1) CFI_TFunc16(status,CFI_TOCOMPENSATE1(n,m,S1,V1))

/**
 * @brief Compensation of 2 previously fed values
 * @param status CFI tracking variable "CoT"
 * @param n source step
 * @param m destination step
 * @param S1 size of value V1
 * @param V1 value to compensate
 * @param S2 size of value V1
 * @param V2 value to compensate
 * @return updated status
 * @details update the status from the n th step + 2 fed values to the m th step
 * CFI_COMPENSATE(n,m,V...) such that 
 * CFI_TFunc16(status,CFI_COMPENSATE(n,m,V...)) = CFI_STEP(m)
 */
#define CFI_COMPENSATE_STEP2(status,n,m,S1,V1,S2,V2) CFI_TFunc16(status,CFI_TOCOMPENSATE2(n,m,S1,V1,S2,V2))

/**
 * @brief Verify status
 * @param status CFI tracking variable "CoT"
 * @param n current step number
 * @return updated status
 * @details verify that status is equal to the expected n th step value
 */
#define CFI_CHECK_STEP(status,n) status; if(status != CFI_STEP(n)) \
status = CFI_ERROR

/**
 * @brief Verify status
 * @param status CFI tracking variable "CoT"
 * @return updated status
 * @details verify that status is equal to the expected final step value
 */
#define CFI_CHECK_FINAL(status) status; if(status != CFI_TFunc16(CFI_SEED(),CFI_KEY)) \
status = CFI_ERROR

/**
 * @brief contextual masking
 * @param data data value to mask
 * @param name name of the masked data
 * @param size size of data
 * @return masked data
 * @details apply the mask to data, mask and data are identified through name and size
 */
#define CFI_MASK(data,name,size) (data) ^ (__CFI_MASK_##size##_##name)

/**
 * @brief contextual unmasking
 * @param Mdata data value to unmask
 * @param name name of the masked data
 * @param size size of data
 * @param n current CFI step number
 * @return unmasked data
 * @details unmask the Mdata under the condition that the CoT takes it's expected value
 */
#define CFI_UNMASK(Mdata,status,size,n) (Mdata) ^ (status) ^ (CFI_MRG(CFI_SEED(),UNMASK_##n##_##size##_##Mdata))

/**
 * @brief contextual unmasking
 * @param MPointer masked address name
 * @param indice location of the masked pointer in the dedicated array CFI_F_Pointer
 * @param status current CFI status
 * @param size size of masked pointer
 * @param n current CFI step number
 * @return unmasked address
 * @details Unmask the address stored in CFI_F_Pointer[indice] under the 
 * condition that the CoT takes it's expected value
 */
#define CFI_UNMASK_POINTER(MPointer,indice,status,size,n) (CFI_F_Pointer[indice]) ^ (status) ^ (CFI_MRG(CFI_SEED(),UNMASK_POINTER_##n##_##size##_##indice##_##MPointer))

/**
 * @brief Transition function
 * @param[in] Value transition variable
 * @param[inout] status variable to be updated
 * @return uint16_t the CFI updated status
 * @details transition function used in every CFI operation, three transition 
 * functions are defined
 * TFuncXOR exclusive or
 * TFuncAfM affine modular
 * TFuncCRC cyclic redoundancy check
 */
extern uint16_t CFI_TFunc16(uint16_t status, uint16_t Value);

/**
 * @brief cyclic redundancy check
 * @param[in] crc current crc value
 * @param[in] buffer data to process
 * @param[in] len buffer len
 * @return uint16_t the computed crc16
 * @details 16-bit cyclic redundancy check
 */
extern uint16_t crc16(uint16_t crc, uint8_t *buffer, uint16_t len);

/**
 * @brief disable fault injection
 * @details place a flag in order to precisely choose the adress to fault 
 * between the usual start and end address
 */
extern void FAULT_OFF (void);

/**
 * @brief enable fault injection
 * @details place a flag in order to precisely choose the adress to fault 
 * between the usual start and end address
 */
extern void FAULT_ON (void);

#endif