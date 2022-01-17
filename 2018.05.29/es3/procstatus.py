import glob
import collections
import os
child = 0
path = "./filebuffer.txt"
fd = os.open(path, os.O_WRONLY | os.O_CREAT)

class Uid :
    def __init__(self):
        self.user_role = [0 for _ in range(0,4)]

uids = {}

for file in glob.glob('/proc/*/status') :
    fd = open(file, "r");
    for ln in fd:
        if ln.startswith("Uid:"):
            arr = (ln[4:].strip()).split('\t')
            for i in range(0,3) :
                if arr[i] not in uids.keys() :
                    uids[arr[i]] = Uid();
                uids[arr[i]].user_role[i] = uids[arr[i]].user_role[i]  + 1;

for x in uids.keys() :
    print(x + ':  ' + str(uids[x].user_role[0]) + ' ' + str(uids[x].user_role[1]) + ' ' + str(uids[x].user_role[2]) + ' ' + str(uids[x].user_role[3]))
