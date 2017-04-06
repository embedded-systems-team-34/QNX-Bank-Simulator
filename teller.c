/******************************************************************************
* FILENAME : teller.c          
*
* DESCRIPTION : 
*     Teller implementation
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include "teller.h"

void initTeller(struct teller* t, unsigned int teller_id) {
    t->teller_id = teller_id;
    t->num_breaks = 0;
    t->customers_processed = 0;
}

void logTellerAvailable(struct teller* t, unsigned int timestamp) {
    t->available_times[t->customers_processed] = timestamp;
}
void logStartTransaction(struct teller* t, unsigned int timestamp) {
    t->start_transaction_times[t->customers_processed] = timestamp;
}
void logEndTransaction(struct teller* t, unsigned int timestamp) {
    t->end_transaction_times[t->customers_processed] = timestamp;
    t->customers_processed += 1;
    
}
void logBreakStart(struct teller* t, unsigned int timestamp) {
    t->break_start_time[t->num_breaks] = timestamp;
}
void logBreakEnd(struct teller* t, unsigned int timestamp) {
    t->break_end_time[t->num_breaks] = timestamp;
    t->num_breaks += 1;
}

// Returns length of longest break in seconds
unsigned int getLongestBreak(struct teller * t) {
    unsigned int i = 0;
    unsigned int max_break_in_seconds = 0;
    
    for (i = 0; i < t->num_breaks; i++) {
        if ((t->break_end_time[i] - t->break_start_time[i]) > max_break_in_seconds) {
            max_break_in_seconds = t->break_end_time[i] - t->break_start_time[i];
        }
    }
    return max_break_in_seconds;
}

// Returns length of shortest break in seconds
unsigned int getShortestBreak(struct teller * t) {
    unsigned int i = 0;
    unsigned int min_break_in_seconds = 1000000;
    
    for (i = 0; i < t->num_breaks; i++) {
        if ((t->break_end_time[i] - t->break_start_time[i]) < min_break_in_seconds) {
            min_break_in_seconds = t->break_end_time[i] - t->break_start_time[i];
        }
    }
    return min_break_in_seconds;
}

// Returns average time of break in seconds
unsigned int getAverageBreak(struct teller *t) {
    unsigned int i = 0;
    unsigned int total_break_in_seconds = 0;
    
    for (i = 0; i < t->num_breaks; i++) {
        total_break_in_seconds += t->break_end_time[i] - t->break_start_time[i];
    }
    return total_break_in_seconds / t->num_breaks;
}

//Returns total amount of time Teller spent waiting
unsigned int getTotalWaitTime(struct teller *t) {
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int transaction_wait_time = 0;
    unsigned int total_wait_time = 0;
    
    // Loop over all transactions and determine how longer teller was waiting before each one started
    for (i = 0; i < t->customers_processed; i++) {
        // Calculate the time between starting a transaction and becoming avaliable 
        transaction_wait_time = t->start_transaction_times[i] - t->available_times[i];
        // Teller may have gone on break between these two times, need to account for this as a teller is not waiting while on break
        // loop over all breaks to determine if any intersect with current transaction wait time 
        for (j = 0; j < t->num_breaks; j++) {
            // Teller went on break during this wait period so dont count this time as teller waiting
            if ((t->break_start_time[j] >= t->available_times[i]) && (t->break_end_time[j] <= t->start_transaction_times[i])) {
                transaction_wait_time -= (t->break_end_time[j] - t->break_start_time[j]);
            }
        }
        total_wait_time += transaction_wait_time;
    }
    return total_wait_time;
}

//Returns maximum amount of time teller spent waiting
unsigned int getMaximumWaitTime(struct teller *t) {
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int transaction_wait_time = 0;
    unsigned int max_wait_time = 0;
    
    // Loop over all transactions and determine how longer teller was waiting before each one started
    for (i = 0; i < t->customers_processed; i++) {
        // Calculate the time between starting a transaction and becoming avaliable 
        transaction_wait_time = t->start_transaction_times[i] - t->available_times[i];
        // Teller may have gone on break between these two times, need to account for this as a teller is not waiting while on break
        // loop over all breaks to determine if any intersect with current transaction wait time 
        for (j = 0; j < t->num_breaks; j++) {
            // Teller went on break during this wait period so dont count this time as teller waiting
            if ((t->break_start_time[j] >= t->available_times[i]) && (t->break_end_time[j] <= t->start_transaction_times[i])) {
                transaction_wait_time -= (t->break_end_time[j] - t->break_start_time[j]);
            }
        }
        if (transaction_wait_time > max_wait_time) {
            max_wait_time = transaction_wait_time;
        }
        
    }
    return max_wait_time;
}

void viewArrays(struct teller *t) {

	unsigned int i = 0;

    printf("*******************************************************\n");
    printf("Teller ID: %u\n", t->teller_id);
    printf("Customers Processed: %u\n", t->customers_processed);
    printf("Number of Breaks: %u\n", t->num_breaks);
    printf("Avaliable Times Array\n");
    for (i = 0; i < t->customers_processed; i++) {
    	printf("Index %u Time %u\n", i, t->available_times[i]);
    }
    printf("Start Times Array\n");
    for (i = 0; i < t->customers_processed; i++) {
    	printf("Index %u Time %u\n", i, t->start_transaction_times[i]);
    }
    printf("End Times Array\n");
    for (i = 0; i < t->customers_processed; i++) {
    	printf("Index %u Time %u\n", i, t->end_transaction_times[i]);
    }
    printf("Start Break Times Array\n");
    for (i = 0; i < t->num_breaks; i++) {
    	printf("Index %u Time %u\n", i, t->break_start_time[i]);
    }
    printf("End Break Times Array\n");
    for (i = 0; i < t->num_breaks; i++) {
    	printf("Index %u Time %u\n", i, t->break_end_time[i]);
    }

    printf("*******************************************************\n");


}
