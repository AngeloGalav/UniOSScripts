#include <dlfcn.h>
#include <gnu/lib-names.h> 
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <inttypes.h>      /* Definition of PRIu64 */
#include <stdlib.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>       /* Definition of uint64_t */
#include <stdio.h>
#include <sys/select.h>
#include <poll.h>

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

void printTime()
{  
	struct timeval tv;  
	gettimeofday(&tv, NULL);  
	printf("printTime:  current time:%ld.%ld ", tv.tv_sec, tv.tv_usec);
}

int main(int argc, char** argv)
{
    struct pollfd timerpfd[argc-1];
    struct itimerspec spec[argc-1];
    int millis_for_timer[argc-1];
    struct timeval current;
    struct timeval prev;

    for (int i = 0; i < argc-1; i++)
    {
        timerpfd[i].fd = timerfd_create(CLOCK_REALTIME,  0);
        timerpfd[i].events = POLLIN;
        timerpfd[i].revents = 0;
        millis_for_timer[i] = atoi(argv[i+1]);
            
        int parte_intera = (int) atoi(argv[i+1])/1000;
        float parte_decimale = (float)(atoi(argv[i+1]) % 1000)/1000;
        printf("parte intera %d, parte decimale %f\n", parte_intera, parte_decimale);
        spec[i].it_value.tv_sec = parte_intera;
        spec[i].it_value.tv_nsec = parte_decimale*1000000000;
        spec[i].it_interval.tv_sec = 0;
        spec[i].it_interval.tv_nsec = 0;

        timerfd_settime(timerpfd[i].fd, 0, &spec[i], NULL); // start timer
    }

    int i = 0;
    int timers = argc-1;
    while (timers > 0) {
        int numEvents = poll(&timerpfd[i], 1, -1);
        if (numEvents > 0) {
            uint64_t timersElapsed = 0;
            (void) read(timerpfd[i].fd, &timersElapsed, 8);
            printf("timers elapsed: %d, seconds %d\n", i, millis_for_timer[i]);
            timers--;
        }
        i++;
        if (i > argc - 1) i = 0;
    }

    return 0;
}