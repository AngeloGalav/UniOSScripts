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
#include <sys/inotify.h>

#include <stdio.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int counter = 0;
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo fdsi;
    ssize_t s;

    time_t mytime = time(NULL);
    char * time_str;

    pid_t father = getpid();

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    printf("pid is: %d\n", father);

    /* Block signals so that they aren't handled
        according to their default dispositions. */
    if (1){//fork() != 0){
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1){
            handle_error("sigprocmask");
        }

        sfd = signalfd(-1, &mask, 0);
        if (sfd == -1){
            handle_error("signalfd");
        }

        for (;;) {
            s = read(sfd, &fdsi, sizeof(fdsi));
            if (s != sizeof(fdsi)){
                handle_error("read");
            }
            
            if (fdsi.ssi_signo == SIGUSR1) {
                counter++;
                printf("counter: %d\n", counter);

            } else if (fdsi.ssi_signo == SIGUSR2) {
                counter--;
                printf("counter: %d\n", counter);
            } else {
                printf("Read unexpected signal\n");
            }

        }
    } 
    // else {
    //     // creo altri 2 processi per testare
    //     sleep(1);
    //     kill(father, SIGUSR1);
    //     sleep(1);
    //     kill(father, SIGUSR2);
    //     sleep(1);
    //     kill(father, SIGUSR1);
    //     sleep(1);
    //     kill(father, SIGUSR1);
    //     sleep(1);
    //     kill(father, SIGUSR1);
    //     sleep(1);
    //     kill(father, SIGUSR1);
    //     exit(0);
    // } 

    return 0;
}