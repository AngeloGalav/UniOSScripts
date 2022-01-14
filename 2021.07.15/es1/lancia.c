/** Va compilato con gcc -o lancia -ldl -rdynamic lancia.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h> 
#include <stdlib.h>
#include <limits.h>

int main(int argc, char **argv)
{
    char *symlinkpath = argv[1];
    char actualpath[PATH_MAX+1];
    char *ptr;


    ptr = realpath(symlinkpath, actualpath);


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
    char** bailalacalle = malloc(20 * sizeof * bailalacalle);
    int i = 1;
    while (argv[i] != NULL){
        bailalacalle[i-1] = argv[i];
        i++;
    }
    bailalacalle[i-1] = NULL;


    (*somain)((int)argc - 1, (char**)bailalacalle);
    dlclose(handle);
    free(bailalacalle);
    exit(EXIT_SUCCESS);
}