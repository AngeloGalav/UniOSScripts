#include <sys/syscall.h>
#include <stdio.h>

int main(){
    printf("number: %d\n", __NR_accept);
}