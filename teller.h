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

struct teller {
	unsigned int tellerId;
    unsigned int customersProcessed;
    unsigned int available_times[MAX_QUEUE_SIZE];
    unsigned int start_transaction_times[MAX_QUEUE_SIZE];
    unsigned int end_transaction_times[MAX_QUEUE_SIZE];
    unsigned int break_start_time[MAX_QUEUE_SIZE];
    unsigned int break_end_time[MAX_QUEUE_SIZE];
    };

#endif
