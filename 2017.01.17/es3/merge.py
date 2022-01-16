#!/usr/bin/python
import glob
import os
import sys
import json
import shutil


# Function to create new folder if not exists
def make_new_folder(folder_name, parent_folder):
	
	# Path
	path = os.path.join(parent_folder, folder_name)
	
	# Create the folder
	# 'new_folder' in
	# parent_folder
	try:
		# mode of the folder
		mode = 0o777

		# Create folder
		os.mkdir(path, mode)
	except OSError as error:
		print(error)

# current folder path
current_folder = os.getcwd()
folder1 = sys.argv[1]
folder2 = sys.argv[2]
destination = sys.argv[3]
# list of folders to be merged
list_dir = [folder1, folder2]

# enumerate on list_dir to get the
# content of all the folders ans store
# it in a dictionary
firstFiles = []
secondFIles = []
for file in os.listdir(folder1):
    firstFiles.append(file)
  
find=0
for file in os.listdir(folder2):
    find=0
    print(file)
    for other in firstFiles:
        print(other)
        if file == other:
            find=1
            if os.path.getmtime(file) >= os.path.getmtime(other):#se file è + vecchio
                firstFiles.remove(file)
                secondFIles.append(other)
    if find == 0:   
        secondFIles.append(file)

print(firstFiles)
print(secondFIles)

# non funziona se i files esistono già ma non ho voglia di controllare
for file in firstFiles:
    f = open(os.path.join(destination, file), 'x')


for file in secondFIles:
    f = open(os.path.join(destination, file), 'x')
    



  

# # folder in which all the content will
# # be merged
# merge_folder = "merge_folder"

# # merge_folder path - current_folder
# # + merge_folder
# merge_folder_path = os.path.join(current_folder, merge_folder)

# # create merge_folder if not exists
# make_new_folder(merge_folder, current_folder)

# # loop through the list of folders
# for sub_dir in content_list:

# 	# loop through the contents of the
# 	# list of folders
# 	for contents in content_list[sub_dir]:

# 		# make the path of the content to move
# 		path_to_content = sub_dir + "/" + contents

# 		# make the path with the current folder
# 		dir_to_move = os.path.join(current_folder, path_to_content )

# 		# move the file
# 		shutil.move(dir_to_move, merge_folder_path)
