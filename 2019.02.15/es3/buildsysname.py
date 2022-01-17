import os

fd = open("/usr/lib/modules/5.9.16-1-MANJARO/build/arch/x86/include/generated/uapi/asm/unistd_64.h", "r");

filec = open("sysname.c", "w")

filec.write("char *syscall_name[] = {\n");

filec = open("sysname.c", "a")

for line in fd.readlines() :
    if line.startswith("#define __NR_") : 
        print(line)
        parts = (line.strip()).split(' ')
        filec.write("\""+ parts[1][5:]+ "\"" +",\n")
    
filec.write("}");
