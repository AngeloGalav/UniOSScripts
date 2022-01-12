import os
import sys

visited_init_file = []
files_final = dict()

inputdir = sys.argv[1]

for root, dirs, files in os.walk(inputdir):
    for file in files:
        with open((root + '/' + file), "r") as file_1 :
            first_line = file_1.readline()
            if (first_line not in visited_init_file) :
                visited_init_file.append(first_line)
                files_final.setdefault(first_line, []).append((os.path.relpath(root, inputdir) + '/' + file));
            else:
                files_final.setdefault(first_line, []).append((os.path.relpath(root, inputdir) + '/' + file));

for file in visited_init_file :
    print (file, files_final[file])