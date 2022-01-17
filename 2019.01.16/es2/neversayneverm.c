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

// if you keep the fd open, you can recover the file ;)
typedef struct file{
    FILE* file;
    char name[PATH_MAX];
}file_t;

file_t files[50];
int main(int argc, char** argv)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd = inotify_init();
    if ( fd < 0 ) perror( "inotify_init" ); // error checking

    DIR * dirp;
    struct dirent * entry;
    int file_count = 0;

    dirp = opendir(argv[1]); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            strcpy(files[file_count].name, argv[1]);
            strcat(files[file_count].name, "/");
            strcat(files[file_count].name, entry->d_name);
            files[file_count].file = fopen(files[file_count].name, "r");
            file_count++;
        }
    }
    closedir(dirp);

    //keep watching the dir in argv
    wd = inotify_add_watch(fd, argv[1], IN_DELETE);

    // read to determine the event change happens on dir specified in argv directory. 
    // Actually this read blocks until the change event occurs
    while (length = read(fd, buffer, EVENT_BUF_LEN)) {
        
        if ( length < 0 ) perror( "read" ); // error checking
        i = 0;
        // actually read return the list of change events happens.
        while ( i < length ) {     
            struct inotify_event *event = ( struct inotify_event * ) &buffer[i];     
            if ( event->len ) { // in caso di 0 errori
                if ( event->mask & IN_DELETE ) {
                    if ( !(event->mask & IN_ISDIR) ) {
                        char path[PATH_MAX];
                        strcpy(path, argv[1]);
                        strcat(path, "/");
                        strcat(path, event->name);
                        printf("%s deleted\n", path);
                        int j = 0;
                        for(j = 0; j < file_count; j++){
                            if (strcmp(path, files[j].name)==0){
                                break;
                            }
                        }
                        FILE* ptr = fopen(path, "w");
                        // int keepit = creat(path, 0777); //questo almeno un po' funzionava
                        // char c[5000]; 
                        // int i = 0;
                        // while (c != NULL){
                        //     read(files[j].file_desc, &c + i, 1);
                        //     write(keepit, &c + i, 1);
                        //     i++;
                        //     printf("yeah..");
                        // }

                        char c;
                        c = fgetc(files[j].file);
                        while (c != EOF)
                        {
                            fputc(c, ptr);
                            c = fgetc(files[j].file);
                        }

                        fclose(files[j].file);
                        files[j].file = ptr;
                        printf("%s recreated\n", path);
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
