#include <stdio.h>
#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 


// argv[1] is file
int main(int argc, char** argv){
    char buf[PATH_MAX]; /* PATH_MAX incudes the \0 so +1 is not required */
    DIR *d;
    char finalpath[PATH_MAX];
    struct dirent *dir;
    d = opendir(argv[1]);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            sprintf(finalpath, "%s/%s", argv[1], dir->d_name);
            char *res = realpath(finalpath, buf);
            if (res != NULL)
                printf("%s\n", res);
        }
        closedir(d);
    }
}
