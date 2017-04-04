/******************************************************************************
* FILENAME : main.c      
*
* DESCRIPTION : 
*     Bank simulator implementation. Customers arrive, are placed into a queue
*     and processed by the teller threads. Teller threads assign timestamps based
*     on various events occurring and these timestaps are post-processed and after 
*     the bank simulation is complete at which point stats are output to the console
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include <sys/neutrino.h>
#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>

#include "customer.h"
#include "queue.h"
#include "teller.h"
#include "stats.h"


#define QNX_BASE_ADDRESS (0x280)
#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOUR (60)
#define TIMER_PERIOD_NS (1666666)
#define CLOCK_PERIOD_10US (10000)

// These are in terms of 3.3 ms ticks
// 9am
#define START_OF_DAY (0)
// 4pm
#define END_OF_DAY (SECONDS_PER_MINUTE*MINUTES_PER_HOUR*7)
#define DEBUG (FALSE)

pthread_mutex_t lock;
pthread_cond_t cv;
struct queue inqueue;
struct queue outqueue;
unsigned int customersInBank = 0;

#if DEBUG
uintptr_t port_a;
uintptr_t dir;
#endif

// Represents seconds
unsigned int count = 0;
// Number of customers in line
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

    // Log tellers as avaliable for start of day
    logTellerAvailable(t, 1);

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
                customersInBank += 1;
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
                customersInBank -= 1;
            }

            if ((timeUntilBreak > 0 ) && (BREAKS == TRUE)) {
                timeUntilBreak -= 1;
            }

            // If end of day and the queue is empty
            if ((count > END_OF_DAY) && (fifo_empty == TRUE) && customersInBank == 0) {
            	pthread_mutex_unlock(&lock);
                break;
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *bankDoor() {

    unsigned int random_next_arrival = 0;
    unsigned int last_processed_cnt = 1;
    unsigned int customer_cnt = 0;

    while (TRUE) {
        // While the bank is not closed
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
        
        // If the bank is closed and all customers have finished there transactions
        if ((count > END_OF_DAY) && (customersInBank == 0)) {
            break;
        }
    }
#if DEBUG
    printf("The bank is closed!!\n");
    printf("Bank had: %u\n", customer_cnt);
#endif
    return NULL;
}

// Action handler -> This fires periodically every 1.6 ms and corresponds 
// to a passage of 1 second of realtime
void timer() {

#if DEBUG
    static unsigned int val = 0;
#endif
    
    pthread_mutex_lock(&lock);
    count += 1;
#if DEBUG
    // debug code used to verify accuract of timer
    val ^= 1;
    out8(port_a,val);
#endif
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&lock);
    if (inqueue.length > max_num_in_line) {
        max_num_in_line = inqueue.length;
    }
}

int main(int argc, char *argv[]) {

    pthread_t teller1;
    pthread_t teller2;
    pthread_t teller3;
    pthread_t bankdoor_th;

    // What to do when the timer expires
    struct sigevent event;
    struct sigaction action;

    //Parameters of the timer
    struct itimerspec timerSpec;
    timer_t timerID;
    
    srand(time(NULL));
    // Initalize the queues
    init_queue(&inqueue);
    init_queue(&outqueue);

    // Initalize mutex and condition variable used to communicate between threads
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init (&cv, NULL);

    // Initalize teller structure
    struct teller tell1;
    struct teller tell2;
    struct teller tell3;
    
    initTeller(&tell1,1);
    initTeller(&tell2,2);
    initTeller(&tell3,3);

    struct _clockperiod clkper;

	// Set clock to a period of 10 us
	clkper.nsec       = CLOCK_PERIOD_10US;
	clkper.fract      = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0  );

#if DEBUG
    // Verify clock period has been set to 10 us
    struct timespec res;
    clock_getres( CLOCK_REALTIME, &res);
	printf( "Resolution is %ld micro seconds.\n", res.tv_nsec / 1000 );
#endif

    
    // Define the oneshot of the timer
	timerSpec.it_value.tv_sec = 0;
	timerSpec.it_value.tv_nsec = TIMER_PERIOD_NS;

	//Define the period of timer
	timerSpec.it_interval.tv_sec = 0;
	timerSpec.it_interval.tv_nsec = TIMER_PERIOD_NS;


#if DEBUG
    if ( ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {

    	printf("Failed to get I/O access permission");
    }

    port_a = mmap_device_io(1, QNX_BASE_ADDRESS + 8);
    dir = mmap_device_io(1, QNX_BASE_ADDRESS + 11);
    // Set PortA as output
    out8(dir,0x00);
#endif

    printf("Starting Simulation.\n");
    
    // Setup signal handler which occurrs when timer fires
	action.sa_sigaction = timer;
	action.sa_flags = SA_SIGINFO;
	sigaction( SIGUSR1, &action, NULL );
	SIGEV_SIGNAL_INIT(&event, SIGUSR1);
    
    // Start the timer 
    timer_create(CLOCK_REALTIME, &event, &timerID);
    timer_settime(timerID, 0, &timerSpec, NULL);

    // Create the three teller threads
    pthread_create(&teller1, NULL, teller_thread, &tell1);
    pthread_create(&teller2, NULL, teller_thread, &tell2);
    pthread_create(&teller3, NULL, teller_thread, &tell3);

    // Create the bank door thread responsible for generating customers
    pthread_create(&bankdoor_th, NULL, bankDoor, NULL);

    // Wait until the end of the day and no customers left in the bank
    pthread_join(bankdoor_th, NULL);

    // Initalize the statstics structure with teller and customer information
    struct stats s;
    initStats(&s, &tell1, &tell2, &tell3, &outqueue, max_num_in_line);

    outputStats(&s);
#if DEBUG
    viewArrays(&tell1);
    viewArrays(&tell2);
    viewArrays(&tell3);
#endif

    printf("EXITING SIMULATION\n");

	return EXIT_SUCCESS;
}
