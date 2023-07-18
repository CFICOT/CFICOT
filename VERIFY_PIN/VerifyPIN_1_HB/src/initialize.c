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

#include "types.h"
#include "interface.h"
#include "commons.h"

// global variables definition
BOOL g_authenticated;
SBYTE g_ptc;
UBYTE g_countermeasure;
volatile UBYTE g_userPin[PIN_SIZE];
UBYTE g_cardPin[PIN_SIZE];

/**
 * @brief initialize the verifyPIN scenario
 * @details
 * PWD scenario : Attacker aims to make an inccorect PIN code accepted
 * CNT scenario : Attacker tries to bypass the maximum number of attempts
 */
void initialize()
{
   // local variables
   int i;
   // global variables initialization
   g_authenticated = BOOL_FALSE;
#ifdef PWD
   g_ptc = 3;
#elif defined CNT
   //g_ptc = 0 impossible to try a new password
   g_ptc = 0;
#endif
   g_countermeasure = 0;
   // card PIN = 1 2 3 4 5...
   for (i = 0; i < PIN_SIZE; ++i) {
       g_cardPin[i] = i+1;
   }
#ifdef PWD
   // user PIN = 0 0 0 0 0...
   for (i = 0 ; i < PIN_SIZE; ++i) {
       g_userPin[i] = 0;
   }
#elif defined CNT
   // user PIN = 1 2 3 4 5...
   for (i = 0 ; i < PIN_SIZE; ++i) {
       g_userPin[i] = i+1;
   }
#endif 
}