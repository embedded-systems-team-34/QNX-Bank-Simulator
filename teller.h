/******************************************************************************
* FILENAME : teller.h          
*
* DESCRIPTION : 
*     Teller function prototypes
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#ifndef TELLER_H
#define TELLER_H

#include "queue.h"

struct teller {
	unsigned int teller_id;
    unsigned int customers_processed;
    unsigned int num_breaks;
    unsigned int available_times[MAX_QUEUE_SIZE];
    unsigned int start_transaction_times[MAX_QUEUE_SIZE];
    unsigned int end_transaction_times[MAX_QUEUE_SIZE];
    unsigned int break_start_time[MAX_QUEUE_SIZE];
    unsigned int break_end_time[MAX_QUEUE_SIZE];
};

void initTeller(struct teller* t, unsigned int teller_id);    
void logTellerAvailable(struct teller* t, unsigned int timestamp);
void logStartTransaction(struct teller* t, unsigned int timestamp);
void logEndTransaction(struct teller* t, unsigned int timestamp);
void logBreakStart(struct teller* t, unsigned int timestamp);
void logBreakEnd(struct teller* t, unsigned int timestamp);
unsigned int getLongestBreak(struct teller * t);
unsigned int getShortestBreak(struct teller * t);
unsigned int getAverageBreak(struct teller *t);
unsigned int getTotalWaitTime(struct teller *t);
unsigned int getMaximumWaitTime(struct teller *t);
void viewArrays(struct teller *t);
    
#endif
