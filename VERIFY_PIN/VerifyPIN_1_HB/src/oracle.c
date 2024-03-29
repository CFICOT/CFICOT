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

// define the oracle you want to use.
#ifdef AUTH
#define oracle_auth oracle
#endif

#ifdef PTC
#define oracle_ptc oracle
#endif

extern UBYTE g_countermeasure;
extern BOOL g_authenticated;
extern SBYTE g_ptc;

/**
 * @brief oracle function, defines the acceptance condition for the PIN code
 * @return BOOL status of the PIN verification
 * @details if countermeasure isn't triggerred and authentication is true, 
 * then the PIN code is accepted
 */
BOOL oracle_auth()
{
    return (g_countermeasure != BOOL_TRUE) && (g_authenticated == BOOL_TRUE);
}

BOOL oracle_ptc()
{
    return (g_countermeasure != BOOL_TRUE) && (g_authenticated == BOOL_TRUE);
}


