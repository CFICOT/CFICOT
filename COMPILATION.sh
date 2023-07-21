#!/bin/bash
set -e
set -x

export LDFLAGS="-specs=nosys.specs -specs=nano.specs"
export CHOST=arm-none-eabi
export CPU="Cortex-M3"
export CPUDEF="CORTEX_M3"
export CFLAGS="-mcpu=cortex-m3 -mthumb -mfloat-abi=soft"

initialize(){
	mkdir -p build
	cd build 
	rm -rf *
	autoreconf -fi ..
	optimization=$OPT ../configure --prefix=$PREFIX --host=$CHOST && make clean && make TFUNC=$TFUNC
	cd -
	python3 Edit_CFI_F_Integrity_elf.py faults_CFI_MEM_COT.elf
}

#Configuration options:
#Transition function: TFUNC = TFuncCRC, TFuncAfM or TFuncXOR
TFUNC=${TFUNC:=TFuncXOR}
#Optimizations: OPT = 0,1,2,3,s
OPT=${OPT:="3"}

initialize

#Fault injection there

exit
