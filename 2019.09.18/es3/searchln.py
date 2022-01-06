import os
import sys

rootdir = sys.argv[1];
explored_inodes = []; #lista degli inode esplorati
n_links = 0

# scorre tutti i file in una cartella
for subdir, dirs, files in os.walk(rootdir): 
    for file in files:
        n_links = 0
        if os.stat(os.path.join(subdir, file)).st_ino not in explored_inodes : 
            for file2 in files:
                if (os.stat(os.path.join(subdir, file)).st_ino == os.stat(os.path.join(subdir, file2)).st_ino) : 
                    n_links += 1
                    print(os.path.join(subdir, file2), end =" ")
            
        if (n_links > 0) : print(os.path.join(subdir, file));
        #explored_inodes.append(os.stat(os.path.join(subdir, file)).st_ino);