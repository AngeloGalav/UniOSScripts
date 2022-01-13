#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    pid_t father = atoi(argv[1]);
    sleep(1);
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