#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>


char* delimiter = " // " ;
struct stat file_stat_buf;
char commands[64][64];
pid_t pid[5];
char** com;

int main(int argc, char** argv)
{
    com = commands;
    int i = 1, s = 0, nprocess = 0;
    while (argv[i] != NULL){
        if (strcmp(argv[i], "//") == 0 || argv[i+1] == NULL) {
            // for (int j = s; j < i; j++){
            //     printf("%s\n", argv[j]); 
            // }
            com[i+1] = NULL;
            if ((pid[nprocess]=fork()) == 0) {
                int k = 0;
                while (com[k] != NULL) {
                    printf("%s ", com[k]);
                    k++; 
                }
                printf("\n"); 
                //execvp(com[0], com);
                break;
            }
            nprocess++;
            s = i;
            
        } else {
            com[i-s-1] = argv[i];
        }
        i++;
    }

    // int status;
    // for (i = 0; i < nprocess; i++) { 
    //     if (pid[i] > 0) {
    //         waitpid(pid[i], &status, 0);
    //     }
    // }
}