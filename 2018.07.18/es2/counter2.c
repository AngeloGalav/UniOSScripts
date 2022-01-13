#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <sys/inotify.h>

#include <stdio.h>
#include <sys/inotify.h>

char commands[64][64];
pid_t pid;
char** com;

int main(int argc, char** argv)
{
    pid_t process;
    FILE* fptr = fopen("./tmp/countpid", "r");
    fscanf(fptr, "%d", &process);


    kill(process,SIGUSR1);
    com = commands;
    int i = 1, s = 0, nprocess = 0;
    while (argv[i] != NULL){
        com[i-s-1] = argv[i];
        i++;
    }
    com[i-s-1] = NULL;

    if ((pid = fork()) == 0){
        execvp(com[0], com);
    }
    wait(&pid);
    kill(process,SIGUSR2);
}