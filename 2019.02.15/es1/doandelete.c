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
#include <fcntl.h>

#include <stdio.h>
#include <sys/inotify.h>
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


int main(int argc, char** argv)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd = inotify_init();
    if ( fd < 0 ) perror( "inotify_init" ); // error checking

    //keep watching the dir in argv
    wd = inotify_add_watch(fd, argv[1], IN_MOVE | IN_CREATE);


    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) //Not a directory or doesn't exist
        return 1;
    while ((d = readdir(dir)) != NULL) {
        if(++n > 2){
            printf("la directory non è vuota!!\n");
            return 0;
        }
    }

    printf("la directory è vuota...\n");

    // read to determine the event change happens on dir specified in argv directory. 
    // Actually this read blocks until the change event occurs
    while (length = read(fd, buffer, EVENT_BUF_LEN)) {
        
        if ( length < 0 ) perror( "read" ); // error checking
        i = 0;
        // actually read return the list of change events happens.
        while ( i < length ) {     
            struct inotify_event *event = ( struct inotify_event * ) &buffer[i];     
            if ( event->len ) { // in caso di 0 errori
                if ( event->mask & IN_CREATE | IN_MOVE) {
                    if ( !(event->mask & IN_ISDIR) ) {
                        struct stat sb;                        
                        char path[PATH_MAX];
                        strcpy(path, "./");
                        strcat(path, argv[1]);
                        strcat(path, "/");
                        strcat(path, event->name);

                        if (stat(path, &sb) != 0) {
                            perror("stat");
                            return -1;
                        }

                        char** args = malloc(6*sizeof * args);
                        args[0] = path;
                        args[1] = NULL;
                        printf("executing %s\n", args[0]);
                        pid_t child;
                        printf("execute %s\n", path);
                        if ((sb.st_mode & S_IXUSR)) 
                        {
                            if ((child = fork()) == 0)
                                execvp(args[0], args);
                        }
                        int val;
                        waitpid(child,&val,0);
                        unlink(path);
                        printf("%s deleted\n", path);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    // removing the directory specified in argv directory from the watch list.
    inotify_rm_watch(fd, wd);

    /*closing the INOTIFY instance*/
    close(fd);
}
