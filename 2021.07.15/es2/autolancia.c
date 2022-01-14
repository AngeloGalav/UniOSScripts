/** Va compilato con gcc -o lancia -ldl -rdynamic lancia.c
 * 
 */
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
#include <elf.h> // ho provato a creare un programma che legge l'header del file elf, ma non funziona a causa del pie (da sempre ET_DYN),
                 // che penso che non sia quello che voglia il prof

char** str_split(char* a_str, const char a_delim);

int main(int argc, char **argv)
{
    char *symlinkpath = argv[1];
    char actualpath[PATH_MAX+1];
    char *ptr;
    ptr = realpath(symlinkpath, actualpath);
    int type;
    
    struct stat sb;
    stat (argv[1], &sb);

    char** bailalacalle = malloc(20 * sizeof * bailalacalle);
    int i = 1;
    while (argv[i] != NULL){
        bailalacalle[i-1] = argv[i];
        i++;
    }
    bailalacalle[i-1] = NULL;
    
    char filename[64];
    strcpy(filename, argv[1]);
    char** filename_parts = str_split(filename, '.');

    i = 0;
    while (filename_parts[i] != NULL){
        i++;
    }   

    if (strcmp(filename_parts[i-1], "so") == 0) type = 1;
    else type = 0;

    if(type){ //se non è un eseguibile, quindi assumo sia un .so
        void *handle;
        void (*somain)(int, char**);
        char *error;

        handle = dlopen(ptr, RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }

        dlerror();    /* Clear any existing error */

        *(void **) (&somain) = dlsym(handle, "main");

        if ((error = dlerror()) != NULL)  {
            fprintf(stderr, "%s\n", error);
            exit(EXIT_FAILURE);
        }

        (*somain)((int)argc - 1, (char**)bailalacalle);
        dlclose(handle);
        free(bailalacalle);
        exit(EXIT_SUCCESS);
    } else {
        // formatting pathname
        char execpathname[PATH_MAX];
        strcpy(execpathname, "./");
        strcat(execpathname, bailalacalle[0]);

        pid_t child;
        int status;
        // esegue il file e scrive il risultato della 
        if ((child = fork()) == 0){
            execvp(execpathname, bailalacalle); // questo programma stampa hw 1 2 3 4, se il prof voleva hw.so basta modificare un parametro di bailalacalle 
        }
        wait(&status); // wait for child process
    }
    free(bailalacalle);
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