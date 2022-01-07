# extremely hacky solution lololol

def remove_prefix(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]
    return text  # or whatever

program_c = open("sysnumber.c","a")
reference = open("reference.txt", "r");
elems = []


for ln in reference:
    if ln.startswith("#define "):
        text = remove_prefix(ln, "#define ")
        cmd = text.split();
        if (cmd[0].startswith("SYS_")) : 
            name, number = cmd;
            print(number);
            elems.append((remove_prefix(name, "SYS_"), number));


program_c.write(
"#include <sys/syscall.h>\n" +
"#include <stdio.h>\n" + 
"#include <string.h>\n" +
"#include <stdlib.h>\n" +
"int main(int argc, char** argv){\n" + 
"if (0) ;\n"
);


for el in elems :
    name, number = el;
    program_c.write("else if(strcmp(\"" + name + "\", argv[1]) == 0) printf(\"%d" + '\\' +"n\", "+ number +" );\n");

for el in elems :
    name, number = el;
    program_c.write("else if(" + number + " == atoi(argv[1])) printf(\"%s" + '\\' +"n\", \""+ name +"\" );\n");

program_c.write("\n}");