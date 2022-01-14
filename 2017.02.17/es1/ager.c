#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <errno.h>


struct stat file_stat;

int main(int argc,const char *argv[])
{

    if(argc == 1)
    {// scorro tutta la directory
        printf("SOlo il nome del programma \n");
        struct dirent *pDirent; //mi preparo per iterare sulla directory
        DIR *pDir;
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        pDir = opendir(cwd);
     //se non è una directory esco
       
        while ((pDirent = readdir(pDir)) != NULL) 
        {
            
            if(stat(pDirent->d_name, &file_stat) == 0)
            {
                // 10 giorni in secondi = 864000
                file_stat.st_mtime = file_stat.st_mtime  - 864000;
                // printf("Dieci giorni prima %ld\n", file_stat.st_mtime);
                // printf("Last modified time: %s", ctime(&file_stat.st_mtime));
                struct utimbuf toAdd;
                toAdd.actime = file_stat.st_mtime;
                toAdd.modtime = file_stat.st_mtime;
                printf("%s\n", ctime(&toAdd.actime));
                printf("%s\n", ctime(&toAdd.modtime));
                utime(pDirent->d_name, &toAdd);
            
            }else
            {
                perror("stat");
            }
        }
    }else
    {
        for(int i=1; i < argc; i++)
        {
            if(stat(argv[i], &file_stat) == 0)
            {
                // 10 giorni in secondi = 864000
                file_stat.st_mtime = file_stat.st_mtime  - 864000;
                // printf("Dieci giorni prima %ld\n", file_stat.st_mtime);
                // printf("Last modified time: %s", ctime(&file_stat.st_mtime));
                struct utimbuf toAdd;
                toAdd.actime = file_stat.st_mtime;
                toAdd.modtime = file_stat.st_mtime;
                printf("%s\n", ctime(&toAdd.actime));
                printf("%s\n", ctime(&toAdd.modtime));
                utime(argv[i], &toAdd);
            
            }else
            {
                perror("stat");
            }
        }
    }
    

}