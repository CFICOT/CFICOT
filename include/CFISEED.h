#ifndef CFISEED_INC_H
#define CFISEED_INC_H
#include <stdint.h>

/**
 * @brief Arbitrary seeds for each CFI protected functions
 */
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
#define __CFI_SEED_SEC_uECC_vli_equal 0x7c31
#define __CFI_SEED_byteArrayCompare 0xad41
#define __CFI_SEED_verifyPIN_11 0x2ba6
#define __CFI_SEED_verifyPIN_31 0xc9a3
#define __CFI_SEED_verifyPIN_32 0xbab8
#define __CFI_SEED_switch_func 0xe14d
#define __CFI_SEED_switch_call 0x2590
#define __CFI_SEED_Function_F 0x7d32
#define __CFI_SEED_Function_G 0x43ab

#endif