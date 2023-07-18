#include <stdint.h>
#include "YACCA.h"

/**
 * @brief YACCA constants for CFI protection
 */
const uint32_t __attribute__ ((section (".cfidata"))) B0 = 0x8a3eaa20;
const uint32_t __attribute__ ((section (".cfidata"))) B1 = 0x1ebd2f13;
const uint32_t __attribute__ ((section (".cfidata"))) B2 = 0x936796b6;
const uint32_t __attribute__ ((section (".cfidata"))) B3 = 0xf67ade7d;
const uint32_t __attribute__ ((section (".cfidata"))) B4 = 0xaa9f7faa;
const uint32_t __attribute__ ((section (".cfidata"))) B5 = 0x0cda1b33;
const uint32_t __attribute__ ((section (".cfidata"))) B6 = 0x1d5f6fa8;
const uint32_t __attribute__ ((section (".cfidata"))) B7 = 0xc7d9eed4;
const uint32_t __attribute__ ((section (".cfidata"))) B8 = 0x9c9b2434;
const uint32_t __attribute__ ((section (".cfidata"))) B9 = 0x2f19b123;
const uint32_t __attribute__ ((section (".cfidata"))) M1_1_8 = (B1 & B8)^(!B1 & !B8);
const uint32_t __attribute__ ((section (".cfidata"))) M1_1_9 = (B1 & B9)^(!B1 & !B9);
const uint32_t __attribute__ ((section (".cfidata"))) M2_1_8_2 = (B1 & B8)^B2;
const uint32_t __attribute__ ((section (".cfidata"))) M2_1_9_2 = (B1 & B9)^B2;
const uint32_t __attribute__ ((section (".cfidata"))) M1_4_6 = (B4 & B6)^(!B4 & !B6);
const uint32_t __attribute__ ((section (".cfidata"))) M2_4_6_7 = (B4 & B6)^B7;

