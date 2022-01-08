#include <signal.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main ()
{
char  nonno = 'nonno';
char figlio = 'figlio';
char nipote = 'nipote';
// creazione del padre
printf("Initial : Father = %d\n\n", getpid()); 
int status = -5;
/****** FORK *******/
prctl(PR_SET_NAME, "nonno\0");

pid_t varFork, varFork2;
int toKill;
prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0);

varFork = fork();
sleep(2);
prctl(PR_SET_NAME, "figlio\0");
if(varFork == -1) //If we have an error, we close the process
{
    printf("ERROR\n");
    exit(-1);
}
else if (varFork == 0) 
{
    printf("SON \n");
    toKill = getpid();
    printf(" ID = %d, Father's ID = %d\n", getpid(), getppid());
    varFork2 = fork();// creazione del nipote
    sleep(2);
    prctl(PR_SET_NAME, "nipote\0");
        

    if(varFork2 == -1) //Se abbiamo un errore chiudiamo il processo
    {
        printf("ERROR\n");
        exit(-1);
    }
    else if (varFork2 == 0) //qui abbiamo il nipote
    {
        printf("\nGRANDSON \n");
        printf(" ID = %d, Father's ID = %d\n", getpid(), getppid());
    }
    
    sleep(20);
    // i kill the son process so the grandson become son of the first process
    // printf("\nKILLED SON \n");
    // // kill(toKill, SIGKILL);

    // printf("Grandson ID = %d , FATHER ID = %d\n", getpid(), getppid());

    
    
}

wait(&status);
sleep(20);
if((status >= 0))
    printf("\nChild exited with status %d\n", status);


return 0;
}