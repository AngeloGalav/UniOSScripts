#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>


int compareFiles(FILE *fp1, FILE *fp2, long start, int length);


int main(int argc, char** argv)
{
    int nprocess = 2;
    int choice = 0;
    if (strcmp("-j", argv[1]) == 0) {
        printf("segfault here?\n");
        if (argv[3] == NULL || argv[4] == NULL){
            printf("not enough arguments for -j flag...\n");
            return 0;
        }
        if (atoi(argv[2]) == 0) {
            printf("invalid process quantity\n");
            return 0;
        }

        choice = 2;
        nprocess = atoi(argv[2]);
    }
    // Open the file
    FILE* fp1 = fopen(argv[1 + choice], "r");
    FILE* fp2 = fopen(argv[2 + choice], "r");

    fseek(fp1, 0L, SEEK_END);
    size_t sz1 = ftell(fp1);

    fseek(fp2, 0L, SEEK_END);
    size_t sz2 = ftell(fp2);

    if (sz1 != sz2){
        printf("Files are different (different sizes).");
        fclose(fp1);
        fclose(fp2);
        return 0;
    }

    printf("size of file is: %d\n", sz1);
    printf("size of file is: %d\n", sz2);

    fseek(fp1, 0L, SEEK_SET);
    fseek(fp2, 0L, SEEK_SET);
    

    int size = sz1/nprocess;

    if (compareFiles(fp1, fp2, 0, sz1) == 1){
        printf("files are different\n");
    } else printf("files are the same\n");
    
    int returnval = 0;
    pid_t pid[nprocess];

    for (int i = 0; i < nprocess; i++)
    {   // crea n processi...
         if ((pid[i]=fork()) == 0) { // con exit invia un valore al padre
            sleep(1);
            printf("process %d started\n", i);
            exit(compareFiles(fp1, fp2, (sz1/nprocess)*i, sz1/nprocess));
            break;
        }
    }
    
    for (int i = 0; i < nprocess; i++) { 
        if (pid[i] > 0) {
            waitpid(pid[i], &returnval, 0);
            if (returnval == 1) {
                printf("files are really different!!\n");
                for (int j = 0; j < nprocess; j++){
                    kill(pid[j], SIGTERM);
                }
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }
    }

    printf("files are not different!!\n");
    fclose(fp1);
    fclose(fp2);
    return 0;
}

int compareFiles(FILE *fp1, FILE *fp2, long start, int length)
{
    int i = 1;
    fseek(fp1, start, SEEK_SET);
    fseek(fp2, start, SEEK_SET);
    // fetching character of two file
    // in two variable ch1 and ch2
    char ch1 = getc(fp1);
    char ch2 = getc(fp2);

    // iterate loop till end of file
    while (ch1 != EOF && ch2 != EOF && i != length)
    {
        i++;
        // if fetched data is not equal then
        // error is incremented
        if (ch1 != ch2) {
            printf("returned diff!!\n");
            return 1;
        }
  
        // fetching character until end of file
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
    printf("returned no diff..\n");
    return 2;
}