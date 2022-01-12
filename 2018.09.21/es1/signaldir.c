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

    /* Block signals so that they aren't handled
        according to their default dispositions. */
    if (fork() != 0){
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1){
            printf("AQUI ESTAAA\n");
            handle_error("sigprocmask");
        }

        sfd = signalfd(-1, &mask, 0);
        if (sfd == -1){
            printf("AQUI ESTAAA\n");
            handle_error("signalfd");
        }

        for (;;) {
            s = read(sfd, &fdsi, sizeof(fdsi));
            if (s != sizeof(fdsi)){
                printf("AQUI ESTAAA\n");
                handle_error("read");
            }
            
            FILE* file;

            if (fdsi.ssi_signo == SIGUSR1) {
                printf("Got SIGUSR1\n");
                char name[32];
                sprintf(name, "%d", fdsi.ssi_pid);
                file = fopen(name, "a");
                time_str = ctime(&mytime);
                time_str[strlen(time_str)-1] = '\0';
                fprintf(file, "USR1 %s\n", time_str);

            } else if (fdsi.ssi_signo == SIGUSR2) {
                printf("Got SIGUSR2\n");
                char name[32];
                sprintf(name, "%d", fdsi.ssi_pid);
                file = fopen(name, "a");
                time_str = ctime(&mytime);
                time_str[strlen(time_str)-1] = '\0';
                fprintf(file, "USR2 %s\n", time_str);
            } else {
                printf("Read unexpected signal\n");
            }

            fclose(file);
        }
    } else {
        // creo altri 2 processi per testare
        sleep(1);
        for(int i = 0; i < 2; i++) {
            if (fork() == 0){
                printf("about to send signal to process %d, i'm process %d\n", father, getpid());
                kill(father, SIGUSR1);
                exit(0);
            }
        }

        printf("about to send signal to process %d, i'm process %d\n", father, getpid());
        kill(father, SIGUSR1);
        sleep(1);
        kill(father, SIGUSR2);
        sleep(1);
        kill(father, SIGUSR1);
        sleep(1);
        kill(father, SIGUSR1);
        sleep(1);
        kill(father, SIGUSR1);
        sleep(1);
        kill(father, SIGUSR1);
        exit(0);
    } 

    return 0;
}