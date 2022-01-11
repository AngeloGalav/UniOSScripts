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

char** str_split(char* a_str, const char a_delim);


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
    wd = inotify_add_watch(fd, argv[1], IN_CREATE);

    // read to determine the event change happens on dir specified in argv directory. 
    // Actually this read blocks until the change event occurs
    length = read(fd, buffer, EVENT_BUF_LEN); 
    if ( length < 0 ) perror( "read" ); // error checking

    // actually read return the list of change events happens.
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[i];     
        if ( event->len ) {
            if ( event->mask & IN_CREATE ) {
                if ( !(event->mask & IN_ISDIR) ) {

                    // getting file name path
                    char created_file[PATH_MAX]; 
                    char pathname[PATH_MAX];
                    strcpy(pathname, argv[1]);
                    strcpy(created_file, event->name);

                    strcat(pathname, "/");
                    strcat(pathname, created_file);

                    char** tokens = str_split(event->name, ' ');
                    pid_t child;

                    int fd1 = open(pathname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                    
                    if ((child = fork()) == 0){
                        dup2(fd1, 1);
                        close(fd1);
                        execvp(tokens[0], tokens);
                    }

                    wait(&child); // wait for child process
                }
            }
        }
        i += EVENT_SIZE + event->len;
    }
    // removing the directory specified in argv directory from the watch list.
    inotify_rm_watch(fd, wd);

    /*closing the INOTIFY instance*/
    close(fd);
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}