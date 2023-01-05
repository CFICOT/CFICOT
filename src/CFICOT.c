#include <stdint.h>
#include "CFICOT.h"
#include "crc.h"

uint16_t __attribute__ ((section (".cficustom"))) CFI_TFunc16(uint16_t status, uint16_t Value) {
#if defined TFuncXOR
    return status ^ Value;
#elif defined TFuncAfM
    return status + Value + 55773;
#elif defined TFuncCRC
    uint8_t Values[2];
    Values[0]= (uint8_t)(Value >> 8);
    Values[1]= (uint8_t)Value;
    return crc16(status, Values, sizeof(Values));
#endif
}

void __attribute__ ((noipa,noinline,noclone,optimize("O0"))) FAULT_ON (void)
{
    asm("INJ_FAULTS_ON:");
}

void __attribute__ ((noipa,noinline,noclone,optimize("O0"))) FAULT_OFF (void)
{
    asm("INJ_FAULTS_OFF:");
}

uint16_t crc16(uint16_t crc, uint8_t *buffer, uint16_t len)
{
    //crc = crc_init();
    crc = crc_update(crc, buffer, len);
    crc = crc_finalize(crc);
    return crc;
}