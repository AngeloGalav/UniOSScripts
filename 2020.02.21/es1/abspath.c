#include <stdio.h>
#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// argv[1] is file
int main(int argc, char** argv){
    char buf[PATH_MAX]; /* PATH_MAX incudes the \0 so +1 is not required */
    char *res = realpath(argv[1], buf);
    if (res == NULL) printf("file not found.. \n");
    else {
        //unlink(argv[1]); /* comment this line to replace the original link */
        if (symlink(res, strcat(argv[1], "_new")))
            printf("New symlink created for %s \n", res);
    }
}