import os
import sys


if __name__=="__main__":
    try:
        dir1 = sys.argv[1]
        dir2 = sys.argv[2]
        dir3 = sys.argv[3]
    except IndexError :
        print("Missing dirs...")
        sys.exit(1)

# list_date = [];

for dir_, _, files in os.walk(dir1):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, dir1)   # queste 3 righe ci servono per poter creare un pathc he funziona
        rel_file1 = os.path.join(rel_dir, file_name)
        rel_file = os.path.join(dir1, rel_file1)
        os.replace(rel_file, dir3 + '/' + file_name);


for dir_, _, files in os.walk(dir2):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, dir2)   # queste 3 righe ci servono per poter creare un pathc he funziona
        rel_file1 = os.path.join(rel_dir, file_name)
        rel_file = os.path.join(dir2, rel_file1)
        if (os.path.exists(dir3 + '/' + file_name)) :
            print("file 1 is " + rel_file + ", file 2 is " + dir3 + '/' + file_name);
            if os.path.getmtime(rel_file) > os.path.getmtime(dir3 + '/' + file_name) :
                os.replace(rel_file, dir3 + '/' + file_name);
        else :
            os.replace(rel_file, dir3 + '/' + file_name);

# list_date.sort(reverse=True);

# print("most recent: " + list_date[0][0]);
# print("most ancient: " + list_date[len(list_date)-1][0]);
