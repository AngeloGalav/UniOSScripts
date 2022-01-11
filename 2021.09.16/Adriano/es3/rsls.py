#!/usr/bin/python
import glob
import os
import sys
import json
dirname = sys.argv[1]
allFiles = []
answer = {}
for subdir, dirs, files in os.walk(dirname): #trova tutto dappertutto
    for file in files: # per ogni file mi salvo tutti i suoi path
        if file in allFiles:
            concat = subdir
            answer[file] = answer[file] + concat + ' '
        else:
            allFiles.append(file)
            answer[file] = subdir + ' '

        
# print(allFiles)
print(answer)
sortednames=sorted(answer.keys(), key=lambda x:x.lower())
for x in sortednames:
    print(x, answer[x])

