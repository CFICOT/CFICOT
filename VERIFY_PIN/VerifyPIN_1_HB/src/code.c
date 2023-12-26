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

#ifdef CFICOT
#include "CFICOT.h"
#include "VERIFY_PIN/VerifyPIN_1_HB/src/code.cfi.h"
#endif

extern SBYTE g_ptc;
extern BOOL g_authenticated;
extern UBYTE g_userPin[PIN_SIZE];
extern UBYTE g_cardPin[PIN_SIZE];

/**
 * @brief byteArrayCompare compares array a1 and a2
 * @param[in] a1 array1
 * @param[in] b2 array2
 * @param[in] size array size
 * @return BOOL_FALSE or BOOL_TRUE
 * @details compares array a1 and a2, returns a boolean
 * conditionnal execution is disabled
 */
#ifndef CFICOT
#ifdef INLINE
inline BOOL byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size) __attribute__((always_inline))
#else
BOOL byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size)
#endif
{
  int i;
  for(i = 0; i < size; i++) {
    if(a1[i] != a2[i]) {
      return BOOL_FALSE;
    }
  }
  return BOOL_TRUE;
}

/**
 * @brief main function faulted in verifyPIN scenario
 * @param[out] BOOL status of the PIN verification
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details checks the try counter then verify the user PIN vs card PIN
 * version 1 with CFI disabled and without conditionnal execution of byteArrayCompare
 */
#if defined INLINE && defined PTC
inline BOOL verifyPIN_1() __attribute__((always_inline))
#else
BOOL verifyPIN_1()
#endif
{
  int comp;
  g_authenticated = BOOL_FALSE;

  if(g_ptc > 0) {
    comp = byteArrayCompare(g_userPin, g_cardPin, PIN_SIZE);
    if(comp == BOOL_TRUE) {
      g_ptc = 3;
      g_authenticated = BOOL_TRUE; // Authentication();
      return BOOL_TRUE;
    }
    else if(comp == BOOL_FALSE) {
      g_ptc--;
      return BOOL_FALSE;
    }
    else {
      countermeasure();
    }
  }

  return BOOL_FALSE;
}

/**
 * @brief byteArrayCompare compares array a1 and a2
 * @param[in] a1 array1
 * @param[in] b2 array2
 * @param[in] size array size
 * @param[out] BOOL resulting comparison
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details compares array a1 and a2, stores the results into BOOL and returns 
 * a CFI status
 * conditionnal execution is disabled
 */
#elif defined CFICOT
#undef CFI_FUNC
#define CFI_FUNC byteArrayCompare
#ifdef INLINE
inline uint16_t byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size, volatile BOOL *result) __attribute__((always_inline))
#else
uint16_t byteArrayCompare(UBYTE* a1, UBYTE* a2, UBYTE size, volatile BOOL *result)
#endif
{
  volatile int i;
  volatile uint16_t status;
  volatile uint16_t sum = CFI_CST;

  if(size == 0) {
    return CFI_ERROR;
  }
  if(a1 == a2) {
		return CFI_ERROR;
  }
  CFI_SET_SEED(status);

  for(i = 0; i < size; i++) {
    if(a1[i] != a2[i]) {
      *result = BOOL_FALSE;
      CFI_FINAL_STEP(status, 0);
      return status;
    }
    sum += CFI_access(a1[i])-CFI_access(a2[i]);
    sum += i;
  }
  sum -= (size * (size - 1)) / 2;
  CFI_FEED(status,16,sum);
  CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_CST);
  CFI_FINAL_STEP(status, 1);
	CFI_CHECK_FINAL(status);
  *result = BOOL_TRUE;
  return status;
}

/**
 * @brief main function faulted in verifyPIN scenario
 * @param[out] BOOL status of the PIN verification
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details checks the try counter then verify the user PIN vs card PIN
 * version 11 with CFI enabled and without conditionnal execution of byteArrayCompare
 */
#undef CFI_FUNC
#define CFI_FUNC verifyPIN_11
#if defined INLINE && defined PTC
inline uint16_t verifyPIN_11(volatile BOOL *result) __attribute__((always_inline))
#else
uint16_t verifyPIN_11(volatile BOOL *result)
#endif
{
  g_authenticated = BOOL_FALSE;
  volatile uint16_t status;

  CFI_SET_SEED(status);

  CFI_FEED(status, 16, CFI_access(g_ptc));

  if(CFI_access(g_ptc) > 0) {
    if(CFI_access(g_ptc) == 1) {
      CFI_COMPENSATE_STEP1(status, 0, 1, 16, 1);
    } else if(CFI_access(g_ptc) == 2) {
      CFI_COMPENSATE_STEP1(status, 0, 1, 16, 2);
    } else if(CFI_access(g_ptc) == 3) {
      CFI_COMPENSATE_STEP1(status, 0, 1, 16, 3);
    } else {
      status = CFI_ERROR;
      countermeasure();
    }
    CFI_FEED(status, 16, byteArrayCompare(g_userPin, g_cardPin, PIN_SIZE, result));
    CFI_COMPENSATE_STEP1(status, 1, 2, 16, CFI_FINAL(byteArrayCompare));
    CFI_FEED(status, 16, *result);
    if(*result == BOOL_TRUE) {
      g_ptc = 3;
      g_authenticated = BOOL_TRUE; // Authentication();
      CFI_COMPENSATE_STEP1(status, 2, 3, 16, 0x00AA);
      CFI_FINAL_STEP(status, 3);
	    CFI_CHECK_FINAL(status);
      return status;
    }
    else if(*result == BOOL_FALSE) {
      CFI_COMPENSATE_STEP1(status, 2, 3, 16, 0x0055);
      g_ptc--;
      CFI_FINAL_STEP(status, 3);
	    CFI_CHECK_FINAL(status);
      return status;
    }
    else {
      status = CFI_ERROR;
      countermeasure();
    }  
  }

  CFI_COMPENSATE_STEP1(status, 0, 1, 16, CFI_CST);
  CFI_NEXT_STEP(status,2);
  CFI_FINAL_STEP(status, 2);
	CFI_CHECK_FINAL(status);
  *result = BOOL_FALSE;

  return status;
}

#endif