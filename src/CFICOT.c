/**
 * @file main Control Flow Intergity file
 */
#include <stdint.h>
#include "CFICOT.h"
#include "crc.h"

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
uint16_t __attribute__ ((section (".cficustom"))) CFI_TFunc16(uint16_t status, uint16_t Value) {
#if defined TFuncXOR
    return status ^ Value;
#elif defined TFuncAfM
    return status + Value + (uint16_t)55773;
#elif defined TFuncCRC
    uint8_t Values[2];
    Values[0]= (uint8_t)(Value >> 8);
    Values[1]= (uint8_t)Value;
    return crc16(status, Values, sizeof(Values));
#else
/* Fallback in case of a missing TFunc's definition*/
    return status ^ Value;
#endif
}

/**
 * @brief enable fault injection
 * @details place a flag in order to precisely choose the adress to fault 
 * between the usual start and end address
 */
void __attribute__ ((noipa,noinline,noclone,optimize("O0"))) FAULT_ON (void)
{
    asm("INJ_FAULTS_ON:");
}

/**
 * @brief disable fault injection
 * @details place a flag in order to precisely choose the adress to fault 
 * between the usual start and end address
 */
void __attribute__ ((noipa,noinline,noclone,optimize("O0"))) FAULT_OFF (void)
{
    asm("INJ_FAULTS_OFF:");
}

/**
 * @brief cyclic redundancy check
 * @param[in] crc current crc value
 * @param[in] buffer data to process
 * @param[in] len buffer len
 * @return uint16_t the computed crc16
 * @details 16-bit cyclic redundancy check
 */
uint16_t crc16(uint16_t crc, uint8_t *buffer, uint16_t len)
{
    crc = crc_update(crc, buffer, len);
    crc = crc_finalize(crc);
    return crc;
}