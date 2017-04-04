/******************************************************************************
* FILENAME : stats.h          
*
* DESCRIPTION : 
*     Stats function prototypes
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include <stdio.h>

#include "teller.h" 
#include "queue.h"
#include "customer.h"

#define BREAKS (FALSE)

#ifndef STATS_H
#define STATS_H

struct stats {
	struct teller tellers[3];
    struct queue q;
    unsigned int max_num_in_line;
};

void initStats(struct stats *s, struct teller *t1, struct teller *t2, struct teller *t3, struct queue *q, unsigned int max_num_in_line);
void outputStats(struct stats *s);
unsigned int getMax3(unsigned int num1, unsigned int num2, unsigned int num3);
    
#endif
