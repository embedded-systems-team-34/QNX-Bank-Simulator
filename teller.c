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

void initTeller(struct teller* t, unsigned int tellerId) {
    t->tellerId = tellerId;
    t->numBreaks = 0;
    t->customersProcessed = 0;
}

void logTellerAvailable(struct teller* t, unsigned int timestamp) {
    t->available_times[t->customersProcessed] = timestamp;
}
void logStartTransaction(struct teller* t, unsigned int timestamp) {
    t->start_transaction_times[t->customersProcessed] = timestamp;
}
void logEndTransaction(struct teller* t, unsigned int timestamp) {
    t->end_transaction_times[t->customersProcessed] = timestamp;
    t->customersProcessed += 1;
    
}
void logBreakStart(struct teller* t, unsigned int timestamp) {
    t->break_start_time[t->numBreaks] = timestamp;
}
void logBreakEnd(struct teller* t, unsigned int timestamp) {
    t->break_end_time[t->numBreaks] = timestamp;
    t->numBreaks += 1;
}

// Returns lenght of longest break is seconds
unsigned int getLongestBreak(struct teller * t) {
    unsigned int i = 0;
    unsigned int maxBreakInSeconds = 0;
    
    for (i = 0; i < t->numBreaks; i++) {
        if ((t->break_end_time[i] - t->break_start_time[i]) > maxBreakInSeconds) {
            maxBreakInSeconds = t->break_end_time[i] - t->break_start_time[i];
        }
    }
    return maxBreakInSeconds;
}

// Returns lenght of longest break is seconds
unsigned int getShortestBreak(struct teller * t) {
    unsigned int i = 0;
    unsigned int minBreakInSeconds = 1000000;
    
    for (i = 0; i < t->numBreaks; i++) {
        if ((t->break_end_time[i] - t->break_start_time[i]) < minBreakInSeconds) {
            minBreakInSeconds = t->break_end_time[i] - t->break_start_time[i];
        }
    }
    return minBreakInSeconds;
}

// Returns average time of break in seconds
unsigned int getAverageBreak(struct teller *t) {
    unsigned int i = 0;
    unsigned int totalBreakTimeInSeconds = 0;
    
    for (i = 0; i < t->numBreaks; i++) {
        totalBreakTimeInSeconds += t->break_end_time[i] - t->break_start_time[i];
    }
    return totalBreakTimeInSeconds / t->numBreaks;
}

unsigned int getTotalWaitTime(struct teller *t) {
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int transaction_wait_time = 0;
    unsigned int totalWaitTime = 0;
    
    // Loop over all transactions and determine how longer teller was waiting before each one started
    for (i = 0; i < t->customersProcessed; i++) {
        // Calculate the time between starting a transaction and becoming avaliable 
        transaction_wait_time = t->start_transaction_times[i] - t->available_times[i];
        // Teller may have gone on break between these two times, need to account for this as a teller is not waiting while on break
        // loop over all breaks to determine if any intersect with current transaction wait time 
        for (j = 0; j < t->numBreaks; j++) {
            // Teller went on break during this wait period so dont count this time as teller waiting
            if ((t->break_start_time[j] >= t->available_times[i]) && (t->break_end_time[j] <= t->start_transaction_times[i])) {
                transaction_wait_time -= (t->break_end_time[j] - t->break_start_time[j]);
            }
        }
        totalWaitTime += transaction_wait_time;
    }
    return totalWaitTime;
}

unsigned int getMaximumWaitTime(struct teller *t) {
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int transaction_wait_time = 0;
    unsigned int maxWaitTime = 0;
    
    // Loop over all transactions and determine how longer teller was waiting before each one started
    for (i = 0; i < t->customersProcessed; i++) {
        // Calculate the time between starting a transaction and becoming avaliable 
        transaction_wait_time = t->start_transaction_times[i] - t->available_times[i];
        // Teller may have gone on break between these two times, need to account for this as a teller is not waiting while on break
        // loop over all breaks to determine if any intersect with current transaction wait time 
        for (j = 0; j < t->numBreaks; j++) {
            // Teller went on break during this wait period so dont count this time as teller waiting
            if ((t->break_start_time[j] >= t->available_times[i]) && (t->break_end_time[j] <= t->start_transaction_times[i])) {
                transaction_wait_time -= (t->break_end_time[j] - t->break_start_time[j]);
            }
        }
        if (transaction_wait_time > maxWaitTime) {
            maxWaitTime = transaction_wait_time;
        }
        
    }
    return maxWaitTime;
}

unsigned int viewArrays(struct teller *t) {

	unsigned int i = 0;

    printf("*******************************************************\n");
    printf("Teller ID: %u\n", t->tellerId);
    printf("Customers Processed: %u\n", t->customersProcessed);
    printf("Number of Breaks: %u\n", t->numBreaks);
    printf("Avaliable Times Array\n");
    for (i = 0; i < t->customersProcessed; i++) {
    	printf("Index %u Time %u\n", i, t->available_times[i]);
    }
    printf("Start Times Array\n");
    for (i = 0; i < t->customersProcessed; i++) {
    	printf("Index %u Time %u\n", i, t->start_transaction_times[i]);
    }
    printf("End Times Array\n");
    for (i = 0; i < t->customersProcessed; i++) {
    	printf("Index %u Time %u\n", i, t->end_transaction_times[i]);
    }
    printf("Start Break Times Array\n");
    for (i = 0; i < t->numBreaks; i++) {
    	printf("Index %u Time %u\n", i, t->break_start_time[i]);
    }
    printf("End Break Times Array\n");
    for (i = 0; i < t->numBreaks; i++) {
    	printf("Index %u Time %u\n", i, t->break_end_time[i]);
    }

    printf("*******************************************************\n");


}
