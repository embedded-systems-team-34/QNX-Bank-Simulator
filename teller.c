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
