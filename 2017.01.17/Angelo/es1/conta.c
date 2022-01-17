#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

struct stat buf;

void tree_conta(char *basePath, const int root);

int inodes_list[300];
int inodes_found_n = 0;
char* path_dir;
int dir_count = 0;
int file_count = 0;

int main(int argc, char** argv)
{
    path_dir = malloc(64*sizeof(char));
    struct stat file_stat_buf;
    if (argv[1] == NULL)
    {
        printf("no valid directory specified... scanning set to '.'\n");
        strcpy(path_dir, ".");
    }
    else {
        if(stat(argv[1], &buf) < 0) {
            printf("invalid folder. scanning set to '.'\n");
            strcpy(path_dir, ".");
        } else {
            strcpy(path_dir, argv[1]);
        }
    }
    printf("scanning folder %s\n", path_dir);

    tree_conta(path_dir, 0);

    printf("dirs are %d, files are %d\n", dir_count, inodes_found_n);
}

void tree_conta(char *basePath, const int root)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            
            if (dp->d_type == DT_REG) { /* If the entry is a regular file */
                struct stat fbuf;
                stat (path, &fbuf);

                int found_inode = 0;

                //checking if inode is present
                for (int j = 0; j < inodes_found_n; j++) {
                    if (inodes_list[j] == fbuf.st_ino) {
                        found_inode = 1; 
                        break;
                    } 
                }

                if (found_inode == 0) {
                    //inode is new!!
                    inodes_found_n++;
                    inodes_list[inodes_found_n-1] = fbuf.st_ino;
                    printf("counting file %s\n", dp->d_name); 
                }

            } else if (dp->d_type == DT_DIR) {
                printf("counting dir %s\n", dp->d_name); 
                dir_count++;
            }
            tree_conta(path, root + 2);
        }
    }

    closedir(dir);
}