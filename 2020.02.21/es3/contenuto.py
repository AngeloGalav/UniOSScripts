import os
import sys

def reverse(e):
    return e[::-1]

dirname = sys.argv[1]
arr = os.listdir(dirname);
arr.sort(key=reverse);

print(arr)