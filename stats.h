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

#ifndef STATS_H
#define STATS_H

struct stats {
	struct teller tellers[3];
};

void initStats(struct stats *s, struct teller *t1, struct teller *t2, struct teller *t3);
void printStats(struct stats *s);
    
#endif
