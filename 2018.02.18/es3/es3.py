import os, os.path, glob
os.chdir("/proc/") # mi muovo nella specifica cartella
for file in glob.glob("*/"): #scannerizzo le directories
    if file[:-1].isnumeric() : # scannerizzo le directories che hanno un nome,
        i = 0                  # rimuovendo il backslash dal nome
        for file2 in glob.glob(file + "/fd/*"):
            i += 1
            if i > 1 :
                print("has more than one fd: process " + file[:-1])
                break