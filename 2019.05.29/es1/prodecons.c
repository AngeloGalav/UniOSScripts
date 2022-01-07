// ricorda di usare il flag -lpthread !!!!!!

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <sys/eventfd.h>
  
// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()

int value = -1; 
int can_read;
uint64_t sem_buf;
int mutex;
uint64_t mutex_buf;
int can_write;
uint64_t sem2_buf;


void* consumerf(void *vargp)
{
    while(1){
        read(can_read, &sem_buf, sizeof(uint64_t)); // ok2read.P()
            int toread = value;
        write(can_write, &sem2_buf, sizeof(uint64_t)); //ok2write.V()
        printf("value is: %d \n", toread);
        sleep(rand() % 5 + 1); // aspetta da 1 a 5 secondi
    }
    return NULL;
}

void* producerf(void *vargp)
{
    while(1){
        int val = rand() % 100;
        read(can_write, &sem2_buf, sizeof(uint64_t)); // ok2write.P()
            value = val;
        write(can_read, &sem_buf, sizeof(uint64_t)); // ok2read.V()
        sleep(rand() % 5 + 1); // aspetta da 1 a 5 secondi
    }
    return NULL;
}
   
int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    can_read = eventfd(0, EFD_SEMAPHORE);
    can_write = eventfd(1, EFD_SEMAPHORE);
    pthread_t consumer, producer;
    printf("Initiate\n");
    pthread_create(&producer, NULL, producerf, NULL);
    pthread_create(&consumer, NULL, consumerf, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    exit(0);
    printf("Over. \n");
}