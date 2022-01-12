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
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

char** str_split(char* a_str, const char a_delim);


int main(int argc, char** argv)
{
    FILE* fptr = fopen(argv[1], "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fptr == NULL) {
        printf("no such file...\n");
        exit(EXIT_FAILURE);
    }

    // extremely hacky way of doing it
    char** token;
    char oldline[128];
    int prev_mins = 0, prev_secs = 0;
    int first = 0;
    char line_to_read[128];
    strcpy(oldline, "");
    int seconds = 0;
    int minutes = 0;
   
    while ((read = getline(&line, &len, fptr)) != -1) {
        strcpy(line_to_read, line);
        token = str_split(line_to_read, ' ');
        char** times = str_split(token[2], ':');
        seconds = atoi(times[2]);
        minutes = atoi(times[1]);
        if (first == 0){
            first = 1;
            prev_mins = minutes;
            prev_secs = seconds;
        }
        int to_wait = (seconds - prev_secs) + 60*(minutes - prev_mins);
        printf("%s", oldline);
        sleep(to_wait);
        prev_mins = minutes;
        prev_secs = seconds;
        strcpy(oldline, line);
    }
    
    int to_wait = (seconds - prev_secs) + 60*(minutes - prev_mins);
    printf("%s", oldline);
    sleep(to_wait);
    prev_mins = minutes;
    prev_secs = seconds;
    strcpy(oldline, line);

    printf("\n");

    fclose(fptr);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

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