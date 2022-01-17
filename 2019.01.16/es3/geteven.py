import os
child = 0
path = "./filebuffer.txt"
fd = os.open(path, os.O_WRONLY | os.O_CREAT)
child = os.fork()
if child == 0 :
    os.dup2(fd, 1);
    os.close(fd);
    os.execvp("getent", ["getent", "passwd"]);
print(child)
os.waitpid(child, 0)

# open and parse file
file1 = open(path, 'r')
Lines = file1.readlines()
users_hash_table = {}
i = 0
for line in Lines :
    words = line.split(':');
    length = len(words)
    path = words[length-2]
    username = words[0]
    i += 1
    if path in users_hash_table.keys() :
        users_hash_table[path] = users_hash_table[path] + username + ' '
    else :
        users_hash_table[path] = username + ' ' 

for x in users_hash_table.keys() :
    print(x + ': ' + users_hash_table[x])

