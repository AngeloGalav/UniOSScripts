import sys
import os
inputdir = sys.argv[1]
list_date = [];

for dir_, _, files in os.walk(inputdir):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, inputdir)   # queste 3 righe ci servono per poter creare un pathc he funziona
        rel_file1 = os.path.join(rel_dir, file_name)
        rel_file = os.path.join(inputdir, rel_file1)
        statbuf =  os.stat((rel_file));
        list_date.append((rel_file1, statbuf.st_mtime));

list_date.sort(reverse=True);

print("most recent: " + list_date[0][0]);
print("most ancient: " + list_date[len(list_date)-1][0]);
