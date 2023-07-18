/**
 * @brief uint32_t the injection simulation return code (SPUN_xxx)
 */
#define SPUN_FAULT_DETECTED		0xDEAD0001
#define SPUN_EXEC_OK			0xDEAD0002
#define SPUN_UNKNOWN_PROBLEM		0xDEAD0003
#define SPUN_SETUP_PROBLEM		0xDEAD0004
#define SPUN_FAULT_INJECTED		0xDEAD0005

extern void __attribute__ ((noinline,noclone)) fault_end(uint32_t ret);