# Makefile script with commands to generate the benchmark assembly and binaries.
# Description of the targets:
# asm-arm-auth: produce ARMv7M asm with oracle AUTH
# bin-arm-auth: uses a linker script to produce the binary with oracle AUTH
# asm-arm-ptc: produce ARMv7M asm with oracle PTC
# bin-arm-ptc: uses a linker script to produce the binary with oracle PTC
# lazart-ptc: produce lazart object files with oracle PTC
# lazart-auth: produce lazart object files with oracle AUTH
# clean: clean generated files
# mrproper: clean generated files, including output binaries

.PHONY: clean mrproper asm-arm-auth asm-arm-ptc bin-arm-ptc bin-arm-auth lazart-ptc lazart-auth

FILES = src/countermeasure.c src/initialize.c src/oracle.c src/code.c src/main.c

ARM_FLAGS = -mcpu=cortex-m4 -mthumb -mlittle-endian -mthumb-interwork

CFLAGS = -Wall -Wextra

asm-arm-auth:
	arm-none-eabi-gcc -I../share -Iinclude $(FILES) $(ARM_FLAGS) $(CFLAGS)  -DAUTH -S
	mkdir -p lst/gcc
	mv *.s lst/gcc

bin-arm-auth:
	arm-none-eabi-gcc -c -I../share -Iinclude $(FILES) $(ARM_FLAGS) $(CFLAGS) -DAUTH
	arm-none-eabi-gcc *.o -Tlinker.ld -Wl,-Map=code.map -o code
	arm-none-eabi-objcopy -O ihex code code.hex
	mkdir -p bin/gcc
	mv *.o code bin/gcc
	mv code.hex bin/gcc
	mkdir -p lst/gcc
	mv code.map lst/gcc

asm-arm-ptc:
	arm-none-eabi-gcc -I../share -Iinclude $(FILES) $(ARM_FLAGS) $(CFLAGS)  -DPTC -S
	mkdir -p lst/gcc
	mv *.s lst/gcc

bin-arm-ptc:
	arm-none-eabi-gcc -c -I../share -Iinclude $(FILES) $(ARM_FLAGS) $(CFLAGS) -DPTC
	arm-none-eabi-gcc *.o -Tlinker.ld -Wl,-Map=code.map -o code
	mkdir -p bin/gcc
	mv *.o code bin/gcc
	mkdir -p lst/gcc
	mv code.map lst/gcc

lazart-ptc:
	clang -c -I../share -DLAZART -DPTC -Iinclude src/countermeasure.c src/initialize.c src/oracle.c src/code.c src/main_lazart.c -Wall -Wextra 

lazart-auth:
	clang -c -I../share -DLAZART -DAUTH -Iinclude src/countermeasure.c src/initialize.c src/oracle.c src/code.c src/main_lazart.c -Wall -Wextra 

clean:
	@rm -rf appli* klee-out-* *.dot *.png *.tab *.o *.bc *.ll *~ out.txt *.out lst/ __lazart_transformed_out.c

mrproper: clean
	@rm -rf bin
