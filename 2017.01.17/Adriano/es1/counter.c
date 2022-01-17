#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/sysmacros.h>

// USO QUESTE SOLO PERCHÈ NON ME LE TROVA NEGLI INCLUDES
#define SIZE_MAX		(18446744073709551615UL)

#define	__S_IFMT	0170000	/* These bits determine file type.  */

/* File types.  */
#define	__S_IFDIR	0040000	/* Directory.  */
#define	__S_IFCHR	0020000	/* Character device.  */
#define	__S_IFBLK	0060000	/* Block device.  */
#define	__S_IFREG	0100000	/* Regular file.  */
#define	__S_IFIFO	0010000	/* FIFO.  */
#define	__S_IFLNK	0120000	/* Symbolic link.  */
#define	__S_IFSOCK	0140000	/* Socket.  */

# define S_IFMT		__S_IFMT
# define S_IFDIR	__S_IFDIR
# define S_IFCHR	__S_IFCHR
# define S_IFBLK	__S_IFBLK
# define S_IFREG	__S_IFREG
# ifdef __S_IFIFO
#  define S_IFIFO	__S_IFIFO
# endif
# ifdef __S_IFLNK
#  define S_IFLNK	__S_IFLNK
# endif
#define MAX_ARRAY 10000
///////


struct stat sb;
int count=0;
int indx=0;
unsigned int array[MAX_ARRAY];

void tree(char *basePath, const int root);

int main(int argc, char** argv)
{
      for(int x=0; x < 10000;x ++)
    {
        array[x] = 0;
    }
    
    if(argv[1] == NULL)
        { 
            char cwd[1000];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                tree(cwd, 0);
            }
        }
    else
        tree(argv[1], 0);

    int x ;
    int z;
    //CONTO SOLO I NON DUPLICATI
    for(x=0; x < MAX_ARRAY; x++)
    {
        printf("x :%d\n", array[x]);
        printf("count : %d\n", count);

        if(array[x] == 0)
            break;
       for(z=x+1; z< MAX_ARRAY; z++)
       {
           
           if(array[z] == array[x])
           {
               array[z] == 0;
           }
       }
        count++;
    }
    printf("count : %d\n", count);
}

void tree(char *basePath, const int root)
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

            printf("%s\n", path);

            stat(path, &sb);

           switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  count++;               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  
                array[indx] = sb.st_ino;
                printf("%ld\n", sb.st_ino);
                indx++;

                break;
           case __S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }
            tree(path, root + 2);
        }
    }

    closedir(dir);
}
