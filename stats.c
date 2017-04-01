/******************************************************************************
* FILENAME : stats.c         
*
* DESCRIPTION : 
*     Stats implementation
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include "stats.h"

void initStats(struct stats *s, struct teller *t1, struct teller *t2, struct teller *t3, struct queue *q, unsigned int max_num_in_line) {
    s->tellers[0] = *t1;
    s->tellers[1] = *t2;
    s->tellers[2] = *t3;
    s->q = *q;
    s->max_num_in_line = max_num_in_line;
}

void outputStats(struct stats *s) {
    
    unsigned int i = 0;
    
    printf("*****************************************************************\n");
    printf("Printing Bank Metrics\n");
    printf("*****************************************************************\n");
    
    // Required metrics
    
    unsigned int num_customers = s->q.length;
    unsigned int total_customer_wait_time_in_seconds = 0;
    unsigned int total_time_spent_with_tellers_seconds = 0;
    unsigned int max_customer_wait_time_seconds = 0;
    unsigned int max_transaction_time_seconds = 0;
    
    
    // Loop over all customers that visited the bank today
    for (i = 0; i < num_customers; i++) {
        struct customer c;
        c = pop(&s->q); 
        total_customer_wait_time_in_seconds += c.transactionStartTime - c.bankEntryTime;
        total_time_spent_with_tellers_seconds += c.transactionEndTime - c.transactionStartTime;
        if (max_customer_wait_time_seconds < (c.transactionStartTime - c.bankEntryTime)) {
            max_customer_wait_time_seconds = c.transactionStartTime - c.bankEntryTime;
        }
        if (max_transaction_time_seconds < (c.transactionEndTime - c.transactionStartTime)) {
            max_transaction_time_seconds = c.transactionEndTime - c.transactionStartTime;
        }
    }
    
    printf("* Total numbers of customers serviced during: %u\n", num_customers);
    printf("* Average time customers spent waiting in the queue: %u\n", total_customer_wait_time_in_seconds/num_customers);
    printf("* Average time customers spent with tellers: %u\n", total_time_spent_with_tellers_seconds/num_customers);
    
    unsigned int teller_total_wait_time = getTotalWaitTime(&s->tellers[0]) + 
                                          getTotalWaitTime(&s->tellers[1]) + 
                                          getTotalWaitTime(&s->tellers[2]);

    printf("* Average time tellers spent waiting for customers: %u\n", teller_total_wait_time / num_customers);
    printf("* Maximum customer wait time : %u\n", max_customer_wait_time_seconds);
    
    unsigned int teller1_max_time = getMaximumWaitTime(&s->tellers[0]);
    unsigned int teller2_max_time = getMaximumWaitTime(&s->tellers[0]);
    unsigned int teller3_max_time = getMaximumWaitTime(&s->tellers[0]);
    
    printf("* Maximum wait time for tellers waiting for customers: %u\n", getMax3(teller1_max_time,teller2_max_time,teller3_max_time));
    printf("* Maximum transaction time for the tellers: %u\n", max_transaction_time_seconds);
    printf("* Maximum depth of the queue: %u\n", s->max_num_in_line);
    printf("*****************************************************************\n");
    
    // Grad additional metrics (breaks)
    for (i = 0;  i < 3; i++) {
        printf("* Teller %u took %u breaks.\n", s->tellers[i].tellerId, s->tellers[i].numBreaks);
        printf("* Teller %u shortest break was: %u\n", s->tellers[i].tellerId,getShortestBreak(&s->tellers[i]));
        printf("* Teller %u average break was: %u\n", s->tellers[i].tellerId,getAverageBreak(&s->tellers[i]));
        printf("* Teller %u longest break was: %u\n", s->tellers[i].tellerId,getLongestBreak(&s->tellers[i]));
        printf("*****************************************************************\n");
    }
}

unsigned int getMax3(unsigned int num1, unsigned int num2, unsigned int num3) {
    unsigned int max_num1_2;
    
    if (num1 > num2) {
        max_num1_2 = num1;
    } else {
        max_num1_2 = num2;
    }
    
    if (max_num1_2 > num3) {
        return max_num1_2;
    } else {
        return num3;
    }
    
}
