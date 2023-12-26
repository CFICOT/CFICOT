/**
 * @file switch fault injection scenario (Figures 1-5)
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "faults.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef CFICOT
#include "CFICOT.h"
#include "SWITCH/faults_CFI_SWITCH.cfi.h"
#endif

/**
 * @brief global variables
 * @param[in] key1 : 128 sized key
 * @param[in] key2 : 256 sized key
 * @param[in] plaintext1 : plaintext encrypted after the key is chosen
 */
typedef uint16_t ret_t;
uint8_t* key;
uint8_t key1[16] = {0xc8,0x49,0x77,0xd3,0xa8,0x6b,0x70,0x4d,0xa4,0x35,0x58,0xf5,0x45,0xc5,0x06,0x6b};
uint8_t key2[32] = {0x37,0x58,0x49,0x15,0x3f,0x94,0x41,0x58,0x16,0x60,0x8d,0x17,0xc1,0xd1,0x11,0xeb,0xb5,0x2f,0x46,0x70,0x20,0x6a,0x22,0x51,0x41,0x35,0x98,0x37,0xdc,0x33,0x6e,0xfd};
uint8_t plaintext1[16] = {0x3a,0x97,0x60,0xe3,0x74,0x25,0xcc,0x32,0x04,0x28,0x54,0xd2,0x9d,0xe5,0xfb,0x6d};

/**
 * @brief Dump usefull results to analyze fault scenarios
 * @param[in] Element variable to be dumped
 * @return returns Element value or its size
 * @details Dump chosen variables and their size
 */
uint32_t __attribute__((noinline, noclone)) fault_dump(int Element)
{
	uint32_t ret;

	switch (Element)
	{
	case 0:
		ret = (uint32_t)key;
		break;
	case 1:
		ret = sizeof(key2);
		break;
	default:
		ret = 0;
		break;
	}

	fault_end((uint32_t)ret);

	return ret;
}

/**
 * @brief Returns the 256 sized key address 
 * @return key2 address
 * @details 
 */
uint8_t* __attribute__((noinline,noclone)) get_key256(void) {
	return (uint8_t*)&key2;
}

/**
 * @brief Returns the 128 sized key address
 * @return key1 address
 * @details
 */
uint8_t* __attribute__((noinline,noclone)) get_key128(void) {
	return (uint8_t*)&key1;
}

/**
 * @brief Dummy encryption function
 * @param[in] key key adress
 * @param[in] key_size key size
 * @param[in] plaintext plaintext to be encrypted
 * @param[out] cipher resulting ciphertext
 * @return 0
 * @details computes term by term XOR between key and plaintext and stores the result into cipher
 */
uint16_t __attribute__((noinline,noclone)) encrypt(uint8_t* key, uint16_t key_size, uint8_t* plaintext, uint8_t* cipher) {
	if(key_size == 128) {
		for(int i = 0; i < 16; i++) {
			cipher[i] = key[i] ^ plaintext[i];
		}
	} else if(key_size == 256) {
		for(int i = 0; i < 16; i++) {
			cipher[i] = key[i] ^ key[i+16] ^ plaintext[i];
		}
	}
	return 0;
}

/**
 * @brief Computes a dummy key integrity
 * @param[in] key key adress
 * @param[in] key_size key size
 * @return computed key integrity
 * @details XOR each key term and returns the result
 */
uint8_t __attribute__((noinline,noclone)) compute_integrity_key(uint8_t* key, uint16_t key_size) {
	uint8_t temp = 0;
	for(int i = 0; i < key_size; i++) {
		temp ^= key[i];
	}
	return temp;
}

/**
 * @brief Verify the integrity of a given key
 * @param[in] key key adress
 * @param[in] key_size key size
 * @param[in] key_integrity key integrity
 * @return 1 if key_integrity is correct, 0 otherwise
 * @details XOR each key term and compares the result with key_integrity
 */
uint8_t __attribute__((noinline,noclone)) check_integrity_key(uint8_t* key, uint16_t key_size, uint8_t key_integrity) {
	return (compute_integrity_key(key, key_size) == key_integrity);
}

/**
 * @brief Dummy decryption function
 * @param[in] key key adress
 * @param[in] key_size key size
 * @param[in] cipher ciphertext to be decrypted
 * @param[out] plaintext resulting decrypted plaintext
 * @return 0
 * @details computes term by term XOR between key and cipher and stores the result into plaintext
 */
uint16_t __attribute__((noinline,noclone)) decrypt(uint8_t* key, uint16_t key_size, uint8_t* cipher, uint8_t* plaintext) {
	return encrypt(key, key_size, cipher, plaintext);
}

#ifdef NOPROTECTION
/**
 * @brief Main faulted "switch case" function without CFI protection
 * @param[in] key_size key size
 * @param[in] key_integrity key integrity
 * @return 0 if no errors are detected, -1 otherwise
 * @details choose a key, encrypt + decrypt the plaintext and compares both plaintext
 */
uint16_t __attribute__((noipa, noinline, noclone, section(".noprot"))) switch_func(uint16_t key_size, uint8_t key_integrity){
	ret_t ret =-1;
	uint8_t cipher[16], plaintext2[16];
	
	switch(key_size){
		case 256:
			key = get_key256();
			break;
		case 128:
			key = get_key128();
			break;
		default:
			goto error_handler;
	}
	encrypt(key, key_size, plaintext1, cipher);
	ret = check_integrity_key(key,key_size,key_integrity);
	if(ret != 1) {
		goto error_handler;
	}
	decrypt(key, key_size, cipher, plaintext2);
	ret = memcmp(plaintext1,plaintext2,16);
	return ret;

	error_handler:
		memset(cipher,0x55,sizeof(cipher));
		return -1;
}

/**
 * @brief Call function for the fault scenario
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details  Call the switch case function and produces the fault return code
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".noprot"))) switch_call(void) {
	ret_t ret =-1;

	volatile uint16_t key_size = 256;
	uint8_t key_integrity = compute_integrity_key(get_key256(),key_size);

	asm("SPUN_switch_func_start:");
	ret = switch_func(key_size, key_integrity);
	asm("SPUN_switch_func_end:");
	if((ret == 0) && (memcmp(key,key2,16)==0)) {
		return SPUN_EXEC_OK;
	} else {
		return SPUN_FAULT_INJECTED;
	}
}
#endif

#ifdef CFICOT
#undef CFI_FUNC
#define CFI_FUNC switch_func
/**
 * @brief Main faulted "switch case" function with CFI protection enabled
 * @param[in] key_size key size
 * @param[in] key_integrity key integrity
 * @return uint16_t the CFI return code (CFI_xxx)
 * @details choose a key, encrypt + decrypt the plaintext and compares both plaintext
 */
uint16_t __attribute__((noipa, noinline, noclone, section(".COT"))) switch_func(uint16_t key_size, uint8_t key_integrity){
	ret_t ret =-1;
	uint8_t cipher[16], plaintext2[16];
	volatile uint16_t status = CFI_ERROR;

	CFI_SET_SEED(status);
	
	CFI_FEED(status,16,key_size);
	switch(key_size){
		case 256:
			key = get_key256();
			CFI_COMPENSATE_STEP1(status,0,1,16,256);
			break;
		case 128:
			key = get_key128();
			CFI_COMPENSATE_STEP1(status,0,1,16,128);
			break;
		default:
			goto error_handler;
	}
	CFI_CHECK_STEP(status,1);

	encrypt(key, key_size, plaintext1, cipher);
	ret = check_integrity_key(key,key_size,key_integrity);
	if(ret != 1) {
		goto error_handler;
	}
	decrypt(key, key_size, cipher, plaintext2);
	ret = memcmp(plaintext1,plaintext2,16);
	if(ret != 0) {
		goto error_handler;
	}

	CFI_FINAL_STEP(status, 1);
	CFI_CHECK_FINAL(status);

	return status;

	error_handler:
		memset(cipher,0x55,sizeof(cipher));
		return CFI_ERROR;
}

#undef CFI_FUNC
#define CFI_FUNC switch_call
/** 
 * @brief Call function for the fault scenario 
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 * @details  Call the switch case function and produces the fault return code
 */
uint32_t __attribute__((noipa, noinline, noclone, section(".COT"))) switch_call(void) {
	uint16_t status;

	volatile uint16_t key_size = 256;
	uint8_t key_integrity = compute_integrity_key(get_key256(),key_size);

	asm("SPUN_switch_func_start:");
	status = switch_func(key_size, key_integrity);
	asm("SPUN_switch_func_end:");
	if (status != (CFI_FINAL(switch_func))) {
		return SPUN_FAULT_DETECTED;
	} else if((memcmp(key,key2,16)==0)) {
		return SPUN_EXEC_OK;
	} else {
		return SPUN_FAULT_INJECTED;
	}
}
#endif





/**
 * @brief Executes the fault testing scenario for the mem functions
 *
 * @return uint32_t the injection simulation return code (SPUN_xxx)
 */
int main(void)
{
	fault_end(switch_call());

	fault_end(SPUN_SETUP_PROBLEM);
	exit(0);
}