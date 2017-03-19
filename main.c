#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include "customer.h"
#include "queue.h"

#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOUR (60)

// These are in terms of 1.6 ms ticks
// 9am
#define START_OF_DAY (0)
// 4pm
#define END_OF_DAY (SECONDS_PER_MINUTE*MINUTES_PER_HOUR*7)

pthread_mutex_t lock;
pthread_cond_t cv;
struct queue q;

// Represents seconds 
unsigned int count = 0;

unsigned int randomNum(unsigned int min, unsigned int max) {
    return rand() % (max + 1 - min) + min;
}

void myThread(void) {
    
    unsigned int last_processed_cnt = 1;
    
    while(TRUE) {
        
    }
}

void bankDoor(void) {
    
    unsigned int random_next_arrival = 0;
    unsigned int last_processed_cnt = 1;
    unsigned int customer_cnt = 0;
    unsigned int i = 0;
    
    while( count < END_OF_DAY) {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cv, &lock);
        // If next customer is arriving
        if (random_next_arrival == 0) {      
            customer_cnt += 1;
            // Push current customer into the FIFO
            struct customer c;
            c.transactionStartTime = count;
            push(&q,c);
            // Pick time until next customer arrives
            random_next_arrival = randomNum(60,240);
        } 
        last_processed_cnt = count;
        random_next_arrival -= 1;
        pthread_mutex_unlock(&lock);
    }
    printf("The bank is closed!!\n");
    printf("Bank had: %u\n", customer_cnt);
}

void timer(void) {
    while(TRUE) {
        // Simulate timer which will fire every 1.666 using
        // 1.666 us represents 1 second of reallife time
        // 100 ms -> 60 seconds
        // 100ms/60 = 1.6666ms = 1 second 
        usleep(10);//(1666);
        pthread_mutex_lock(&lock);
        count += 1;
        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&lock);
        
    }   
} 

int main(int argc, char *argv[]) {
    
    pthread_t teller1;
    pthread_t teller2;
    pthread_t teller3;
    pthread_t bankdoor_th;
    pthread_t timer_th;
    
    srand(time(NULL));
    init_queue(&q);
    
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init (&cv, NULL);
    
    
    pthread_create(&teller1, NULL, myThread, NULL);
    pthread_create(&teller2, NULL, myThread, NULL);
    pthread_create(&teller3, NULL, myThread, NULL);
    
    pthread_create(&bankdoor_th, NULL, bankDoor, NULL);
    pthread_create(&timer_th, NULL, timer, NULL);
    
    pthread_join(timer_th, NULL);
    
	return EXIT_SUCCESS;
}