/*******************************************************************************
* main.c
* 3/7/2017
* Donald MacIntyre - djm4912
*
* Description:
*******************************************************************************/

/* Hello World program */

#include<stdio.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;
pthread_t my_thread1;
pthread_t my_thread2;
pthread_t my_thread3;


void handleOne() {
    while (1) {
        sem_wait(&semaphore);
        printf("Executing handleOne()\n");
        sleep(3);
        printf("Done processing 1\n");
    }
}

void handleTwo() {
    while (1) {
        sem_wait(&semaphore);
        printf("Executing handleTwo()\n");
        sleep(2);
        printf("Done processing 2\n");
    }
}

void handleThree() {
    while (1) {
        sem_wait(&semaphore);
        printf("Executing handleThree()\n");
        sleep(1);
        printf("Done processing 3\n");
    }
}

void main()
{    

    int num;
    
    srand(time(NULL));

    // Initalize semaphore to -1 (so all threads will block by default)
    // 0 - don't share semaphore with forked processes????
    sem_init(&semaphore, 0, 0);
    
    pthread_create(&my_thread1, NULL, handleOne); 
    pthread_create(&my_thread2, NULL, handleTwo); 
    pthread_create(&my_thread3, NULL, handleThree); 
    
    while(1) {
        sem_getvalue(&semaphore, &num);
        printf("Currently waiting: %d \n", num);
        sleep(2);
        sem_post(&semaphore);
    }
    
}