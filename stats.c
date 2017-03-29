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

void initStats(struct stats *s, struct teller *t1, struct teller *t2, struct teller *t3) {
    s->tellers[0] = *t1;
    s->tellers[1] = *t2;
    s->tellers[2] = *t3;
}

void outputStats(struct stats *s) {
    
    unsigned int i = 0;
    
    printf("*****************************************************************\n");
    printf("Printing Bank Metrics\n");
    printf("*****************************************************************\n");
    
    // Required metrics
    
    printf("* Total numbers of customers serviced during: %u\n", 0);
    printf("* Average time customers spent waiting in the queue: %u\n", 0);
    printf("* Average time customers spent with tellers: %u\n", 0);
    printf("* Average time tellers spent waiting for customers: %u\n", 0);
    printf("* Maximum customer wait time : %u\n", 0);
    printf("* Maximum wait time for tellers waiting for customers: %u\n", 0);
    printf("* Maximum transaction time for the tellers: %u\n", 0);
    printf("* Maximum depth of the queue: %u\n", 0);
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