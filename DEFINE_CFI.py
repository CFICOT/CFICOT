import sys

#__CFI_SEED_MIN_ARRAY = hex(int("0x7ede",16))
#__CFI_SEED_MIN_ARRAY_CALL = hex(int("0x4dfa",16))
#__CFI_SEED_mem = hex(int("0xa032",16))
#__CFI_SEED_mem_call = hex(int("0xf115",16))
#__CFI_SEED_sec_mem_set = hex(int("0x6a09",16))
#__CFI_SEED_sec_mem_cpy = hex(int("0xbb67",16))
#__CFI_SEED_sec_mem_cmp = hex(int("0x3c6e",16))
#__CFI_SEED_SIGN = hex(int("0xcfb5",16))
#__CFI_SEED_VERIFY = hex(int("0x8bfd",16))
#__CFI_SEED_PONCURVE = hex(int("0x719a",16))
KEY_CFI = int("0x8eb6",16)

STEP = [0x8a3e,0x1ebd,0x9367,0xf67a,0xaa9f,0x0cda,0x1d5f,0xc7d9,0x9c9b,
0x734a,0x7cbf,0xcd6f,0xb093,0x6e3a,0xb754,0xd840,0x4a3d,0xdd09]

crc_table = [0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040]

def compute_crc(crc, data, data_len):
    for i in range(data_len):
        tbl_idx = (crc ^ data[i]) & 0xff
        crc = (crc_table[tbl_idx] ^ (crc >> 8)) & 0xffff

    return crc & 0xffff

def TFunc(status,value):
        #XOR transition function
        if (sys.argv[2] == 'TFuncXOR'):
                return status ^ value
        #Affine modular transition function
        elif (sys.argv[2] == 'TFuncAfM'):
                return ((status + value) + 55773) % 65536
        #CRC transition function
        elif (sys.argv[2] == 'TFuncCRC'):
                Bvalue = [0x00,0x00]
                Bvalue[0] = (value >> 8) & 0xff
                Bvalue[1] = value & 0xff
                return compute_crc(status,Bvalue,2)

def CFI_NEXT_STEP(status,n):
        return TFunc(status,n)

def CFI_FEED(status,value):
        return TFunc(status,value)

def CFI_GET_COMP_VALUE(status,status_dest):
        for i in range(0,65536):
                if(TFunc(status,i) == status_dest):
                        return i
        print("ERROR: COMPENSATE Value not computable")

def WRITE_STEP(file1,seed,currentstep):
        status = seed
        for i in range(0,currentstep):
                status = TFunc(status,STEP[i+1])
        file1.write(" " + format(status, '#06x') + "\n")

def WRITE_TOFINAL(file1,seed,currentstep):
        final = TFunc(seed, KEY_CFI)
        status = seed
        for i in range(0,currentstep):
                status = TFunc(status,STEP[i+1])
        compensation = CFI_GET_COMP_VALUE(status,final)
        file1.write(" " + format(compensation, '#06x') + "\n")

def WRITE_FINAL(file1,seed):
        final = TFunc(seed, KEY_CFI)
        file1.write(" " + format(final, '#06x') + "\n")
        return final

def WRITE_TOCOMPENSATE1(file1,seed,currentstep,nextstep,size,value):
        status = seed
        status_dest = seed
        for i in range(0,currentstep):
                status = TFunc(status,STEP[i+1])
        for i in range(0,nextstep):
                status_dest = TFunc(status_dest,STEP[i+1])
        if(size == 16):
                status = CFI_FEED(status,value)
                compensation = CFI_GET_COMP_VALUE(status,status_dest)
                file1.write(" " + format(compensation, '#06x') + "\n")
        else:
                print(line)
                print("error compensate")

def WRITE_TOCOMPENSATE2():
        print('TODO')

def WRITE_TOCOMPENSATE3():
        print('TODO')

def WRITE_TOCOMPENSATE4():
        print('TODO')

def WRITE_MASK(file1,seed,currentstep):
        status = seed
        for i in range(0,currentstep):
                status = TFunc(status,STEP[i+1])
        file1.write(" " + format(status, '#06x') + "\n")

file1 = open(sys.argv[1] + '.cfi.h', 'w')

file2 = open(sys.argv[1] + '.pp', 'r')

Lines = file2.readlines()

for line in Lines:
        n = line.find("__CFI_")
        if (n != -1):
                p = line.find("___CFI_")
                if(p != -1):
                        q = p
                        while (line[q] != ")"):
                                q = q + 1
                        file1.write("#define ")
                        file1.write(line[p+1:q])
                        #Final CoT value
                        if(line[p+14:p+19] == "FINAL"):
                                seed = int(line[p+7:p+13],16)
                                final = WRITE_FINAL(file1,seed)
                        else:
                                #Only FINAL CoT values are supposed to be compensated
                                print(line)
                                print("error compensate final")

                        #COMPENSATE the previously computed final
                        if(line[n+13:n+25] == "TOCOMPENSATE"):
                                if(line[n+25] == "1"):
                                        seed = int(line[n+6:n+12],16)
                                        currentstep = int(line[n+27],16)
                                        nextstep = int(line[n+29],16)
                                        size = int(line[n+31:n+33],10)
                                        file1.write("#define " + line[n:p])
                                        file1.write("_" + format(final, '#06x'))
                                        WRITE_TOCOMPENSATE1(file1,seed,currentstep,nextstep,size,final)
                                else:
                                        print(line)
                                        print("multiple final feed TODO")
                        else:
                                print(line)
                                print("error compensate final")

                else:
                        m = n
                        while (line[m] != ")"):
                                m = m + 1
                        seed = int(line[n+6:n+12],16)

                        file1.write("#define ")
                        file1.write(line[n:m])

                        if(line[n+13:n+17] == "STEP"):
                                currentstep = int(line[n+18],16)
                                WRITE_STEP(file1,seed,currentstep)
                        elif(line[n+13:n+20] == "TOFINAL"):
                                currentstep = int(line[n+21],16)
                                WRITE_TOFINAL(file1,seed,currentstep)
                        elif(line[n+13:p+18] == "FINAL"):
                                WRITE_FINAL(file1,seed)
                        elif(line[n+13:n+25] == "TOCOMPENSATE"):
                                if(line[n+25] == "1"):
                                        currentstep = int(line[n+27],16)
                                        nextstep = int(line[n+29],16)
                                        size = int(line[n+31:n+33],10)
                                        if(line[n+34:n+36] == "0x"):
                                                value = int(line[n+34:n+40],16)
                                        else:
                                                value = int(line[n+34:m],10)
                                        WRITE_TOCOMPENSATE1(file1,seed,currentstep,nextstep,size,value)
                                else:
                                        file1.write(" 0x1234\n")
                        elif(line[n+13:n+17] == "MASK"):
                                if(line[n+17:n+19] == "16"):
                                        currentstep = int(line[n+20],16)
                                        WRITE_MASK(file1,seed,currentstep)
                                else:
                                        print("error MASK")

                        else:
                                print("error: " + line[n+13:n+25])
                                print(line[n+13:n+25] + "\n")
                        


file2.close()
file1.close()