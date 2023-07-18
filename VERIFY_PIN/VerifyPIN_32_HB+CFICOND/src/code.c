/**************************************************************************/
/*                                                                        */
/*  This file is part of FISSC.                                           */
/*                                                                        */
/*  you can redistribute it and/or modify it under the terms of the GNU   */
/*  Lesser General Public License as published by the Free Software       */
/*  Foundation, version 3.0.                                              */
/*                                                                        */
/*  It is distributed in the hope that it will be useful,                 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*  GNU Lesser General Public License for more details.                   */
/*                                                                        */
/*  See the GNU Lesser General Public License version 3.0                 */
/*  for more details (enclosed in the file LICENSE).                      */
/*                                                                        */
/**************************************************************************/

#include "interface.h"
#include "types.h"
#include "commons.h"
#include "CFICOT.h"
#include "VERIFY_PIN/VerifyPIN_32_HB+CFICOND/src/code.cfi.h"

extern SBYTE g_ptc;
extern BOOL g_authenticated;
extern UBYTE g_userPin[PIN_SIZE];
extern UBYTE g_cardPin[PIN_SIZE];

/**
 * @brief byteArrayCompare compares array a1 and a2
 * @param[in] a1 array1
 * @param[in] b2 array2
 * @param[in] size array size
 * @param SEEDIN execution token for conditionnal execution
 * @param[out] BOOL resulting comparison
 * @return CFI_WEXEC or CFI_ERROR depending if the execution happenned
 * @details compares array a1 and a2, stores the results into BOOL and returns 
 * a status indicating if execution happenned
 * conditionnal execution is enabled
 */
#undef CFI_FUNC
#define CFI_FUNC byteArrayCompare
#ifdef INLINE
inline uint16_t byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size, volatile BOOL *result, uint16_t SEEDIN) __attribute__((always_inline))
#else
uint16_t byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size, volatile BOOL *result, uint16_t SEEDIN)
#endif
{
  int i;
  volatile uint16_t status;
  volatile uint16_t sum = CFI_CST;

  if(size == 0)
    return CFI_ERROR;
  if(a1 == a2)
		return CFI_ERROR;

  status = CFI_GET_SEED(status,SEEDIN);
  status = CFI_FEED(status,16,size);

  for(i = 0; i < size; i++) {
    if(a1[i] != a2[i]) {
      *result = BOOL_FALSE;
      return status;
    }
    sum += CFI_access(a1[i])-CFI_access(a2[i]);
  }
  status = CFI_FEED(status,16,sum);
  status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_CST);
  status = CFI_FINAL_STEP(status, 1);
	status = CFI_CHECK_FINAL(status);
  *result = BOOL_TRUE;
  return CFI_WEXEC;
}

/**
 * @brief main function faulted in verifyPIN scenario
 * @param[out] BOOL status of the PIN verification
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details checks the try counter then verify the user PIN vs card PIN
 * version 32 with CFI enabled and conditionnal execution of byteArrayCompare
 */
#undef CFI_FUNC
#define CFI_FUNC verifyPIN_31
#if defined INLINE && defined PTC
inline uint16_t verifyPIN_32(volatile BOOL *result) __attribute__((always_inline))
#else
uint16_t verifyPIN_32(volatile BOOL *result)
#endif
{
  g_authenticated = BOOL_FALSE;
  volatile uint16_t status, status2, SEEDIN;

  status = CFI_SET_SEED(status);

  status = CFI_FEED(status, 16, CFI_access(g_ptc));

  if(CFI_access(g_ptc) > 0) {
    if(CFI_access(g_ptc) == 1)
      status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, 1);
    else if(CFI_access(g_ptc) == 2)
      status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, 2);
    else if(CFI_access(g_ptc) == 3)
      status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, 3);
    else {
      status = CFI_ERROR;
      countermeasure();
    }
    SEEDIN = CFI_STEP(1);
    SEEDIN ^= status;
    SEEDIN ^= _CFI_SEED(byteArrayCompare);
    status2 = byteArrayCompare(g_userPin, g_cardPin, PIN_SIZE, result, SEEDIN);
    if(status2 == CFI_WEXEC)
      return CFI_WEXEC;
    status = CFI_FEED(status, 16, status2);
    status = CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_FINAL(byteArrayCompare));
    status = CFI_FEED(status, 16, *result);
    if(*result == BOOL_TRUE) {
      g_ptc = 3;
      g_authenticated = BOOL_TRUE; // Authentication();
      status = CFI_COMPENSATE_STEP1(status, 2, 3, 16, 0x00AA);
      status = CFI_FINAL_STEP(status, 3);
	    status = CFI_CHECK_FINAL(status);
      return status;
    }
    else if(*result == BOOL_FALSE) {
      status = CFI_COMPENSATE_STEP1(status, 2, 3, 16, 0x0055);
      g_ptc--;
      status = CFI_FINAL_STEP(status, 3);
	    status = CFI_CHECK_FINAL(status);
      return status;
    }
    else {
      status = CFI_ERROR;
      countermeasure();
    }  
  }

  status = CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_CST);
  status = CFI_NEXT_STEP(status,2);
  status = CFI_FINAL_STEP(status, 2);
	status = CFI_CHECK_FINAL(status);
  *result = BOOL_FALSE;

  return status;
}