AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = faults_CFI_MEM_NOPROT.elf \
                  faults_CFI_MEM_COT.elf \
                  faults_CFI_MIN_ARRAY_NOPROT.elf \
                  faults_CFI_MIN_ARRAY_YACCA.elf \
                  faults_CFI_MIN_ARRAY_COT.elf

SUFFIXES = .cfi.h .pp

.c.pp:
	echo "PREP   $*.c $*.cfi.h -> $@"
	truncate --size 0 $*.cfi.h
	$(CC) -E -x c $< -DCFI_PREPROCESS -DCFICOT -D${TFUNC} -I. > $@ || (rm $*.cfi.h && false)


.pp.cfi.h:
	echo "python $< -> $@"
	python3 ../DEFINE_CFI.py $* ${TFUNC}

faults_CFI_MEM_NOPROT_elf_CFLAGS = -DNOPROTECTION 
faults_CFI_MEM_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MEM_NOPROT_elf_LDADD =
faults_CFI_MEM_NOPROT_elf_SOURCES = faults_CFI_MEM.c startup.s faults.c crc.c
EXTRA_faults_CFI_MEM_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MEM_COT_elf_CFLAGS = -DCFICOT -D${TFUNC}
faults_CFI_MEM_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MEM_COT_elf_LDADD =
faults_CFI_MEM_COT_elf_SOURCES = faults_CFI_MEM.c CFICOT.c startup.s faults.c crc.c
EXTRA_faults_CFI_MEM_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES = faults_CFI_MEM.cfi.h

faults_CFI_MIN_ARRAY_NOPROT_elf_CFLAGS = -DNOPROTECTION
faults_CFI_MIN_ARRAY_NOPROT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_NOPROT_elf_LDADD =
faults_CFI_MIN_ARRAY_NOPROT_elf_SOURCES = faults_CFI_MIN_ARRAY.c startup.s faults.c
EXTRA_faults_CFI_MIN_ARRAY_NOPROT_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MIN_ARRAY_YACCA_elf_CFLAGS = -DYACCA
faults_CFI_MIN_ARRAY_YACCA_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_YACCA_elf_LDADD =
faults_CFI_MIN_ARRAY_YACCA_elf_SOURCES = faults_CFI_MIN_ARRAY.c YACCA.c startup.s faults.c
EXTRA_faults_CFI_MIN_ARRAY_YACCA_elf_DEPENDENCIES = ${LinkerDependencies}

faults_CFI_MIN_ARRAY_COT_elf_CFLAGS = -DCFICOT -D${TFUNC}
faults_CFI_MIN_ARRAY_COT_elf_LDFLAGS = -T ${top_srcdir}/Linker_CortexM.ld 
faults_CFI_MIN_ARRAY_COT_elf_LDADD =
faults_CFI_MIN_ARRAY_COT_elf_SOURCES = faults_CFI_MIN_ARRAY.c CFICOT.c startup.s faults.c crc.c
EXTRA_faults_CFI_MIN_ARRAY_COT_elf_DEPENDENCIES = ${LinkerDependencies}
BUILT_SOURCES += faults_CFI_MIN_ARRAY.cfi.h