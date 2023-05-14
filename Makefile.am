AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = faults_CFI_MEM_NOPROT.elf \
                  faults_CFI_MEM_COT.elf \
                  faults_CFI_MIN_ARRAY_NOPROT.elf \
                  faults_CFI_MIN_ARRAY_YACCA.elf \
                  faults_CFI_MIN_ARRAY_COT.elf \
		  faults_CFI_VP_1_PWD.elf \
                  faults_CFI_VP_1_CNT.elf \
                  faults_CFI_VP_11_PWD.elf \
                  faults_CFI_VP_11_CNT.elf \
                  faults_CFI_VP_31_CNT.elf \
                  faults_CFI_VP_32_CNT.elf \
                  faults_CFI_SWITCH_NOPROT.elf \
                  faults_CFI_SWITCH_COT.elf \
                  faults_CFI_FCALL_NOPROT.elf \
                  faults_CFI_FCALL_COT.elf

SUFFIXES = .cfi.h .pp

.c.pp:
	echo "PREP   $*.c $*.cfi.h -> $@"
	truncate --size 0 $*.cfi.h
	$(CC) -E -x c $< -DCFI_PREPROCESS -DCFICOT -DMPOINTER -DVP11 -DVP31 -DVP32 -D${TFUNC} -I${top_srcdir}/include -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include -I${top_srcdir}/VERIFY_PIN/share -I. > $@ || (rm $*.cfi.h && false)


.pp.cfi.h:
	echo "python $< -> $@"
	python3 ../DEFINE_CFI.py $* ${TFUNC}

faults_CFI_MEM_NOPROT_elf_CFLAGS = -DNOPROTECTION -I${top_srcdir}/include
faults_CFI_MEM_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld
faults_CFI_MEM_NOPROT_elf_LDADD =
faults_CFI_MEM_NOPROT_elf_SOURCES = MEM_FUNC/faults_CFI_MEM.c  src/startup.s  src/faults.c  src/crc.c
EXTRA_faults_CFI_MEM_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MEM_COT_elf_CFLAGS = -DCFICOT -D${TFUNC} -I${top_srcdir}/include -DMPOINTER
faults_CFI_MEM_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MEM_COT_elf_LDADD =
faults_CFI_MEM_COT_elf_SOURCES = MEM_FUNC/faults_CFI_MEM.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c
EXTRA_faults_CFI_MEM_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES = MEM_FUNC/faults_CFI_MEM.cfi.h

faults_CFI_MIN_ARRAY_NOPROT_elf_CFLAGS = -DNOPROTECTION -I${top_srcdir}/include
faults_CFI_MIN_ARRAY_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_NOPROT_elf_LDADD =
faults_CFI_MIN_ARRAY_NOPROT_elf_SOURCES = MIN_ARRAY/faults_CFI_MIN_ARRAY.c  src/startup.s  src/faults.c
EXTRA_faults_CFI_MIN_ARRAY_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MIN_ARRAY_YACCA_elf_CFLAGS = -DYACCA -I${top_srcdir}/include
faults_CFI_MIN_ARRAY_YACCA_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_YACCA_elf_LDADD =
faults_CFI_MIN_ARRAY_YACCA_elf_SOURCES = MIN_ARRAY/faults_CFI_MIN_ARRAY.c  src/YACCA.c  src/startup.s  src/faults.c
EXTRA_faults_CFI_MIN_ARRAY_YACCA_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MIN_ARRAY_COT_elf_CFLAGS = -DCFICOT -D${TFUNC} -I${top_srcdir}/include
faults_CFI_MIN_ARRAY_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_COT_elf_LDADD =
faults_CFI_MIN_ARRAY_COT_elf_SOURCES = MIN_ARRAY/faults_CFI_MIN_ARRAY.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c
EXTRA_faults_CFI_MIN_ARRAY_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += MIN_ARRAY/faults_CFI_MIN_ARRAY.cfi.h

faults_CFI_VP_1_PWD_elf_CFLAGS = -DVP1 -DAUTH -DPWD
faults_CFI_VP_1_PWD_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_1_PWD_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_1_PWD_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_1_PWD_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_1_PWD_elf_LDADD =
faults_CFI_VP_1_PWD_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/startup.s  src/faults.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_1_HB/src/code.c 
EXTRA_faults_CFI_VP_1_PWD_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_VP_1_CNT_elf_CFLAGS = -DVP1 -DAUTH -DCNT
faults_CFI_VP_1_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_1_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_1_CNT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_1_CNT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_1_CNT_elf_LDADD =
faults_CFI_VP_1_CNT_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/startup.s  src/faults.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_1_HB/src/code.c 
EXTRA_faults_CFI_VP_1_CNT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_VP_11_PWD_elf_CFLAGS = -DVP11 -DAUTH -DPWD -DCFICOT -D${TFUNC}
faults_CFI_VP_11_PWD_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_11_PWD_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_11_PWD_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_11_PWD_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_11_PWD_elf_LDADD =
faults_CFI_VP_11_PWD_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_1_HB/src/code.c
EXTRA_faults_CFI_VP_11_PWD_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += VERIFY_PIN/VerifyPIN_1_HB/src/code.cfi.h VERIFY_PIN/faults_verifypin.cfi.h

faults_CFI_VP_11_CNT_elf_CFLAGS = -DVP11 -DAUTH -DCNT -DCFICOT -D${TFUNC}
faults_CFI_VP_11_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_11_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_11_CNT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_11_CNT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_11_CNT_elf_LDADD =
faults_CFI_VP_11_CNT_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_1_HB/src/code.c
EXTRA_faults_CFI_VP_11_CNT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += VERIFY_PIN/VerifyPIN_1_HB/src/code.cfi.h VERIFY_PIN/faults_verifypin.cfi.h

faults_CFI_VP_31_CNT_elf_CFLAGS = -DVP31 -DAUTH -DCNT -DCFICOT -D${TFUNC}
faults_CFI_VP_31_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_31_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_31_CNT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_31_CNT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_31_CNT_elf_LDADD =
faults_CFI_VP_31_CNT_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_31_HB+CFICOT/src/code.c
EXTRA_faults_CFI_VP_31_CNT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += VERIFY_PIN/VerifyPIN_31_HB+CFICOT/src/code.cfi.h VERIFY_PIN/faults_verifypin.cfi.h

faults_CFI_VP_32_CNT_elf_CFLAGS = -DVP32 -DAUTH -DCNT -DCFICOT -D${TFUNC}
faults_CFI_VP_32_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/share
faults_CFI_VP_32_CNT_elf_CFLAGS += -I${top_srcdir}/VERIFY_PIN/VerifyPIN_1_HB/include
faults_CFI_VP_32_CNT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_VP_32_CNT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_VP_32_CNT_elf_LDADD =
faults_CFI_VP_32_CNT_elf_SOURCES = VERIFY_PIN/faults_verifypin.c  src/CFICOT.c  src/startup.s  src/faults.c  src/crc.c VERIFY_PIN/VerifyPIN_1_HB/src/countermeasure.c VERIFY_PIN/VerifyPIN_1_HB/src/initialize.c VERIFY_PIN/VerifyPIN_1_HB/src/oracle.c VERIFY_PIN/VerifyPIN_32_HB+CFICOND/src/code.c
EXTRA_faults_CFI_VP_32_CNT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += VERIFY_PIN/VerifyPIN_32_HB+CFICOND/src/code.cfi.h VERIFY_PIN/faults_verifypin.cfi.h

faults_CFI_SWITCH_NOPROT_elf_CFLAGS = -DNOPROTECTION
faults_CFI_SWITCH_NOPROT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_SWITCH_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_SWITCH_NOPROT_elf_LDADD =
faults_CFI_SWITCH_NOPROT_elf_SOURCES = SWITCH/faults_CFI_SWITCH.c src/startup.s src/faults.c
EXTRA_faults_CFI_SWITCH_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_SWITCH_COT_elf_CFLAGS = -DCFICOT -D${TFUNC} -DMPOINTER
faults_CFI_SWITCH_COT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_SWITCH_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_SWITCH_COT_elf_LDADD =
faults_CFI_SWITCH_COT_elf_SOURCES = SWITCH/faults_CFI_SWITCH.c src/startup.s src/faults.c src/CFICOT.c src/crc.c
EXTRA_faults_CFI_SWITCH_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += SWITCH/faults_CFI_SWITCH.cfi.h

faults_CFI_FCALL_NOPROT_elf_CFLAGS = -DNOPROTECTION
faults_CFI_FCALL_NOPROT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_FCALL_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_FCALL_NOPROT_elf_LDADD =
faults_CFI_FCALL_NOPROT_elf_SOURCES = FCALL/faults_CFI_FCALL.c src/startup.s src/faults.c
EXTRA_faults_CFI_FCALL_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_FCALL_COT_elf_CFLAGS = -DCFICOT -D${TFUNC} -DMPOINTER
faults_CFI_FCALL_COT_elf_CFLAGS += -I${top_srcdir}/include
faults_CFI_FCALL_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_FCALL_COT_elf_LDADD =
faults_CFI_FCALL_COT_elf_SOURCES = FCALL/faults_CFI_FCALL.c src/startup.s src/faults.c src/CFICOT.c src/crc.c
EXTRA_faults_CFI_FCALL_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += FCALL/faults_CFI_FCALL.cfi.h