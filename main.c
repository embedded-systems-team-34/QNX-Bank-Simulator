#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include "customer.h"
#include "queue.h"
#include "teller.h"
#include "stats.h"

#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOUR (60)

// These are in terms of 1.6 ms ticks
// 9am
#define START_OF_DAY (0)
// 4pm
#define END_OF_DAY (SECONDS_PER_MINUTE*MINUTES_PER_HOUR*7)

pthread_mutex_t lock;
pthread_cond_t cv;
struct queue inqueue;
struct queue outqueue;

// Represents seconds 
unsigned int count = 0;
// Number of 
unsigned int max_num_in_line = 0;

unsigned int randomNum(unsigned int min, unsigned int max) {
    return rand() % (max + 1 - min) + min;
}

void *teller_thread(void *args) {
    
    unsigned int fifo_empty;
    unsigned int transactionTime = 0;
    unsigned int breakTime = 0;
    BOOL onBreak = FALSE;
    unsigned int timeUntilBreak = randomNum(30 * SECONDS_PER_MINUTE, 60 * SECONDS_PER_MINUTE);
    
    struct teller *t = (struct teller *) args;
    
    while(TRUE) {
        pthread_mutex_lock(&lock);
        // wait on count to update
        pthread_cond_wait(&cv, &lock);
        fifo_empty = isEmpty(&inqueue); 
        
        // If time for the teller to go on break and teller not busy and teller not currently on break and not the end of day
        // Once the day is over tellers will no longer take breaks as they know as soon as they finish the queue they can go home
        if ((timeUntilBreak == 0) && (transactionTime == 0) && (onBreak == FALSE) && (count < END_OF_DAY)) {
            onBreak = TRUE;
            breakTime = randomNum(1*SECONDS_PER_MINUTE, 4*SECONDS_PER_MINUTE);
            logBreakStart(t, count);
        }
        // Teller break is over, calculate next break time
        if ((onBreak == TRUE) && (breakTime == 0)) {
            timeUntilBreak = randomNum(30 * SECONDS_PER_MINUTE, 60 * SECONDS_PER_MINUTE);
            onBreak = FALSE;
            logBreakEnd(t, count);
        }
        
        // If the teller is on break, decrement time left on break
        if (breakTime > 0) {
            breakTime -= 1;
        } 
        // Teller is not on break, process customers normally
        else {
            // The queue is not empty and the current teller is free
            if ((fifo_empty == FALSE) && (transactionTime == 0)) {
                // Pop from the queue
                logStartTransaction(t, count);
                struct customer c;
                c = pop(&inqueue);
                // Determine the length of the transactionStartTime
                transactionTime = randomNum(30,8*SECONDS_PER_MINUTE);
                c.transactionStartTime = count;
                c.transactionEndTime = count + transactionTime;
                // Push customers that have been fully processed into the queue for stats processing
                push(&outqueue, c);
            }
            // Do not want to subtract one if a teller is not busy will underflow
            if (transactionTime > 0) {
                transactionTime -= 1;
            } 
            
            // Log the end of a customer transaction
            if (transactionTime == 1) {
                logEndTransaction(t, count + 1);
                logTellerAvailable(t, count + 1);
            }
            
            if (timeUntilBreak > 0 ) {
                timeUntilBreak -= 1;
            }
            
            // If end of day and the queue is empty
            if ((count > END_OF_DAY) && (fifo_empty == TRUE)) {
                pthread_exit(NULL);
            }
        }

        pthread_mutex_unlock(&lock);
        
    }
}

void *bankDoor() {
    
    unsigned int random_next_arrival = 0;
    unsigned int last_processed_cnt = 1;
    unsigned int customer_cnt = 0;
    unsigned int i = 0;
    
    while( count < END_OF_DAY) {
        pthread_mutex_lock(&lock);
        // Wait on count to update
        pthread_cond_wait(&cv, &lock);
        // If next customer is arriving
        if (random_next_arrival == 0) {      
            customer_cnt += 1;
            // Push current customer into the FIFO
            struct customer c;
            c.bankEntryTime = count;
            push(&inqueue,c);
            // Pick time until next customer arrives
            random_next_arrival = randomNum(SECONDS_PER_MINUTE,4*SECONDS_PER_MINUTE);
        } 
        last_processed_cnt = count;
        random_next_arrival -= 1;
        pthread_mutex_unlock(&lock);
    }
    //printf("The bank is closed!!\n");
    //printf("Bank had: %u\n", customer_cnt);
}

void *timer() {
    
    BOOL isempty;
    
    while(TRUE) {
        // Simulate timer which will fire every 1.666 using
        // 1.666 us represents 1 second of reallife time
        // 100 ms -> 60 seconds
        // 100ms/60 = 1.6666ms = 1 second 
        usleep(10);//(1666);
        pthread_mutex_lock(&lock);
        count += 1;
        pthread_cond_broadcast(&cv);
        isempty = isEmpty(&inqueue); 
        pthread_mutex_unlock(&lock);
        if (inqueue.length > max_num_in_line) {
            max_num_in_line = inqueue.length;
        }
        
        // If current time of day is past closing time and queue is empty
        if ((count > END_OF_DAY) && (isempty == TRUE)) {
            break;
        }
        
    }   
} 

int main(int argc, char *argv[]) {
    
    pthread_t teller1;
    pthread_t teller2;
    pthread_t teller3;
    pthread_t bankdoor_th;
    pthread_t timer_th;
    
    srand(time(NULL));
    init_queue(&inqueue);
    init_queue(&outqueue);
    
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init (&cv, NULL);
    
    struct teller tell1;
    struct teller tell2;
    struct teller tell3;
    
    initTeller(&tell1,1);
    initTeller(&tell2,2);
    initTeller(&tell3,3);
    
    pthread_create(&teller1, NULL, teller_thread, &tell1);
    pthread_create(&teller2, NULL, teller_thread, &tell2);
    pthread_create(&teller3, NULL, teller_thread, &tell3);
    
    pthread_create(&bankdoor_th, NULL, bankDoor, NULL);
    pthread_create(&timer_th, NULL, timer, NULL);
    
    pthread_join(timer_th, NULL);
    
    struct stats s;
    initStats(&s, &tell1, &tell2, &tell3, &outqueue, max_num_in_line);
    
    outputStats(&s);
    
	return EXIT_SUCCESS;
}