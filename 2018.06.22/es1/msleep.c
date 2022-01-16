#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h> 
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>      /* Definition of PRIu64 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Definition of uint64_t */
#include <sys/timerfd.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>       /* Definition of uint64_t */

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

#include <stdio.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <time.h>

void printTime()
{  
	struct timeval tv;  
	gettimeofday(&tv, NULL);  
	printf("printTime:  current time:%ld.%ld ", tv.tv_sec, tv.tv_usec);
}

int main(int argc, char** argv)
{
    int tfd = timerfd_create(CLOCK_REALTIME,  0);
    
    printf("Starting at (%d)...\n", (int)time(NULL));
    
    if(tfd > 0)
    {
        char dummybuf[8];
        struct itimerspec spec;
        int parte_intera = (int) atoi(argv[1])/1000;
        float parte_decimale = (float)(atoi(argv[1]) % 1000)/1000;
        bzero(&spec, sizeof(spec));
        printf("parte intera %d, parte decimale %f\n", parte_intera, parte_decimale);
        spec.it_value.tv_sec = parte_intera;
        spec.it_value.tv_nsec = parte_decimale*1000000000;
        spec.it_interval.tv_sec = 0;
        spec.it_interval.tv_nsec = 0;
        

        timerfd_settime(tfd, 0, &spec, NULL);
    
        printTime();
        printf("timer started\n");

        /* Wait */
        fd_set rfds;
        int retval;

        /* Watch timefd file descriptor */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(tfd, &rfds);

        uint64_t exp;
	    ssize_t s = read(tfd, &exp, sizeof(uint64_t));
	    if (s != sizeof(uint64_t))
		   handle_error("read");

        printTime();
        printf("read: %llu; total=%llu\n",exp, 0);
    }
    
    return 0;
}


