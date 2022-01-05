#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

struct stat file_stat_buf;

void tree(char *basePath, const int root, char* prev_dir);

int main(int argc, char** argv)
{
    if (argv[1] == NULL)
        printf("no valid file specified...\n");
    else
        stat (argv[1], &file_stat_buf);

    if (argv[2] == NULL)
    {
        printf("no valid directory specified...\n");
        return;
    }
    else
        tree(argv[2], 0, "");
}

void tree(char *basePath, const int root, char* previous_dir)
{
    int i;
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

            // check if link 
            struct stat sbuf;
            struct stat lbuf;
            stat (path, &sbuf);
            lstat (path, &lbuf);

            if (S_ISLNK(lbuf.st_mode)){
                if (sbuf.st_ino == file_stat_buf.st_ino)
                {
                    printf("symlink ");
                    printf("%s%s\n", previous_dir, dp->d_name);
                }
            } else if (lbuf.st_ino == file_stat_buf.st_ino)
            {
                printf("link ");
                printf("%s%s\n", previous_dir, dp->d_name);
            }

            // formattazione del path come vuole il prof
            char prev_path[1000];
            
            strcpy(prev_path, previous_dir);
            if (strcmp(prev_path, "") != 0)
                strcat(prev_path, "/");
            strcat(prev_path, dp->d_name);
            strcat(prev_path, "/");
            
            tree(path, root + 2, prev_path);
        }
    }

    closedir(dir);
}