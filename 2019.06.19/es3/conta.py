#!/usr/bin/python
import glob
import os
import sys
import json

files_c = []
total_c = 0
files_h = []
total_h = 0
makefiles = []
total_make = 0
total = 0

def count_lines(x):
    num_lines = sum(1 for line in open(x))
    return num_lines


dirname = sys.argv[1]

allFiles = []
answer = {}
for subdir, dirs, files in os.walk(dirname): #trova tutto dappertutto
    for file in files: # per ogni file mi salvo tutti i suoi path
        filename, file_extension = os.path.splitext(file)
        if file_extension == '.c':
            files_c.append(os.path.join(subdir, file))
        if file_extension == '.h':
            files_h.append(os.path.join(subdir, file))
        if filename == 'makefile' or filename == 'Makefile':
            makefiles.append(os.path.join(subdir, file))


for x in files_c:
    roundx = count_lines(x)
    total_c +=   roundx
    print(x, roundx)

print("Total lines in c files ", total_c)

print('  ')

for x in files_h:
    roundx = count_lines(x)
    total_h +=   roundx
    print(x, roundx)

print("Total lines in h files ", total_h)

print('  ')

for x in makefiles:
    roundx = count_lines(x)
    total_make +=   roundx
    print(x, roundx)

print("Total lines in makefiles ", total_make)

print("Total " ,total_c +  total_h + total_make)