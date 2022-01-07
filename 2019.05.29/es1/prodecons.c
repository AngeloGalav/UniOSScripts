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
int can_read = 0;
uint64_t sem_buf = 0; // non so perché, ma se usi un solo buffer funziona, se ne usi due per semaforo no. 
int can_write = 1;


void* consumerf(void *vargp)
{
    int toread;
    while(1){
        read(can_read, &sem_buf, sizeof(uint64_t)); // ok2read.P()
        int toread = value;
        printf("cons value is: %d \n", toread);
        write(can_write, &sem_buf, sizeof(uint64_t)); //ok2write.V()
        sleep(rand() % 5 + 1); // aspetta da 1 a 5 secondi
    }
    return NULL;
}

void* producerf(void *vargp)
{
    int val;
    while(1){
        val = rand() % 100;
        read(can_write, &sem_buf, sizeof(uint64_t)); // ok2write.P()
        printf("prod value is: %d \n", val);
        value = val;
        write(can_read, &sem_buf, sizeof(uint64_t)); // ok2read.V()
        sleep(rand() % 5 + 1); // aspetta da 1 a 5 secondi
    }
    return NULL;
}
   
int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    can_write = eventfd(1, EFD_SEMAPHORE);
    can_read = eventfd(0, EFD_SEMAPHORE);
    pthread_t consumer, producer;
    printf("Initiate\n");
    pthread_create(&producer, NULL, producerf, NULL);
    pthread_create(&consumer, NULL, consumerf, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    exit(0);
    printf("Over. \n");
}