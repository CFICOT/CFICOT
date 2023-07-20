This project is associated to the DSD2023 submission of "Software-only Control-Flow Integrity against Fault Injection Attacks"
by François Bonnal, Vincent Dupaquis, Jean-Max Dutertre and Olivier Potin.

## Abstract: 
In this paper, we introduce a new Control-Flow Integrity (CFI) scheme
for detecting Fault Injection Attacks (FIA). Our scheme is designed to be as
generic as possible and to cover any microcontroller on the market, including
non-secure ones. It is a full software approach, designed to detect CFI disruptions
caused by FIA. The proposal is portable and designed for a high-level language
implementation (C in our case). The main characteristic of our scheme is to link
a predictable computed Chain of Trust (CoT) with the assets of a program.
This approach classically allows the detection of fault injections leading to an
illegitimate path of execution.In addition, this solution is designed to detect
when a legitimate execution path is wrongly followed due to FIA. Simulations on
several benchmarks finally validate the effectiveness of the method, using a
multiple instruction skip faults model.

## Summary:
- COMPILATION.sh build the examples for Arm Cortex® M3
- CFI protection and protected/unprotected code from Examples are provided
- Fault injection simulation tools are not provided

For demonstrating the method, we are including few code examples :
- switch (Figures 1-5)
- FCall (Figure 6)
- Min_array (YACCA)
- Memory functions (memset memcpy memcmp)
- VerifyPIN

Faults were injected between the following labels:
- asm("SPUN_XXXX_start:");
- asm("SPUN_XXXX_end:");

For compiling these examples, you need to have :
- arm-none-eabi-gcc (download it freely form https://developer.arm.com/downloads/-/gnu-rm)
- It has been tested with version 9.3.1

## Files Details:

### Protection source files
- src/CFICOT.c and include/CFICOT.h: macros and constants used for the CFI
protection
- src/crc.c and include/crc.h: generated crc16 code in order to use a crc
transition function.
- src/YACCA.c and include/YACCA.h: Alternative CFI protection implementation
(see Improved software-based processor control-flow errors detection technique.
Goloubeva et al)

### Preprocessing scripts
- DEFINE_CFI.py preprocessing script for computation of the necessary CFI values
(stored in .cfi.h files in build directory).
- Edit_CFI_Integrity_elf.py preprocessing script used to link the integrity of
MEM_FUNC's binary to its CoT.

### Example source code
- SWITCH/faults_CFI_SWITCH.c: switch case scenario (Figures 1-5) compiled with
and without CFI protection.
- FCall/faults_CFI_FCALL.c: FCall scenario (Figure 6) compiled with and without
CFI protection.
- MEM_FUNC/faults_CFI_MEM.c: Memory functions scenario compiled with
and without CFI protection (memset memcpy memcmp).
- MIN_ARRAY/faults_CFI_MIN_ARRAY.c: Example code from YACCA's paper compiled
without any protection with our CFI scheme and with enriched YACCA.

VERIFY_PIN folder:
VerifyPIN_1_HB is the version number 1 coming from FISSC: the Fault Injection
and Simulation Secure Collection, it comes with the functionnality
"HB" Hardenned booleans.
We compile 2 scenarios of attack:
- "CNT" which starts with a correct PIN but 0 attempt lefts
- "PWD" which starts with 3 attempts left but an incorrect PIN.
Version 11 is the protected code of version 1
version 31 is the same protected code with an additional test to detect the
execution of "byteArrayCompare". as described in section VI-C of the paper
we don't want the attacker to be able to execute the function at all. with this
version we found 2 possible FIA which leads to version 32.
Version 32 protects against these 2 attack scenario with the
CFI function GET_SEED (only byteArrayCompare call and seed initialization
slightly differs from version 31).

### build directory:
- faults_*****_NOPROT.elf: CFI disabled resulting elf files tested with fault
injection.
- faults_*****_COT.elf: CFI enabled resulting elf files tested with fault
injection.
- faults_CFI_VP_****.elf: ELF files resulting for the differents VerifyPIN
scenario described above.
- *****.cfi.h: Each *****.c file is processed with DEFINE_CFI.py which computes
the necessary CFI values and stores them in a dedicated *****.cfi.h file before
compilation.