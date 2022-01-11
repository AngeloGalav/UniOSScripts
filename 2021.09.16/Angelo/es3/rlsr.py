#non va....

import os
import sys

visited = []
files_final = dict()

inputdir = sys.argv[1]

for root, dirs, files in os.walk(inputdir):
    for file in files:
        if (file not in visited) :
            visited.append(file)
            files_final.setdefault(file, []).append((os.path.relpath(root, inputdir) + '/' + file));
        else:
            files_final.setdefault(file, []).append((os.path.relpath(root, inputdir) + '/' + file));

end=''
visited.sort();

for key in files_final.keys() :
    files_final[key].sort();


for file in visited :
    print (file, files_final[file])