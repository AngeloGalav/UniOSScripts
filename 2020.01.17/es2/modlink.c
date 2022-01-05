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
char initial_file[1000];
int mode;
void tree(char *basePath, const int root, char* prev_dir);
void create_copyfile(char* src, char* dest);

int main(int argc, char** argv)
{
    if (strcmp("-c", argv[1]) == 0)
        mode = 1;
    else if (strcmp("-l", argv[1]) == 0)
        mode = 2;
    else if (strcmp("-s", argv[1]) == 0)
        mode = 3;
    else{
        printf("no valid mode specified...\n");
        return -1;
    }

    if (argv[2] == NULL)
        printf("no valid file specified...\n");
    else {
        strcpy(initial_file, argv[2]);
        stat(argv[2], &file_stat_buf);
    }

    if (argv[3] == NULL)
    {
        printf("no valid directory specified...\n");
        return -1;
    }
    else
        tree(argv[3], 0, "");
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

                    switch (mode)
                    {
                    case 1:
                        printf("debug %s", path);
                        unlink(path);
                        create_copyfile(initial_file, path);
                        printf("file copiato! \n");
                        break;
                    case 2:
                        unlink(path);
                        link(initial_file, path);
                        printf("hard link creato! \n");
                        break; 
                    }

                }
            } else if (lbuf.st_ino == file_stat_buf.st_ino) // caso in cui è hlink
            {
                printf("link ");
                printf("%s%s\n", previous_dir, dp->d_name);

                    switch (mode)
                    {
                    case 1:
                        printf("debug %s", path);
                        unlink(path);
                        create_copyfile(initial_file, path);
                        printf("file copiato! \n");
                        break;
                    case 3:
                        unlink(path);
                        symlink(initial_file, path);
                        printf("symlink creato! \n");
                        break;
                    }
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

void create_copyfile(char* src, char* dest){
    FILE *src1;
    FILE *final;

    final = fopen(dest, "w");

    if ((src1 = fopen(src,"rb")) == NULL)
        printf("Could not open src\n");   

    int c;
    while((c=fgetc(src1))!=EOF)
    {
        printf("%d\n",c);
        fputc(c,final);
    }
    struct stat st_buf;

    stat(src, &st_buf);
    chmod(dest, st_buf.st_mode);

    fclose(src1);
    fclose(final);
}