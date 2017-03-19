/******************************************************************************
* FILENAME : queue.h          
*
* DESCRIPTION : 
*     Queue function prototypes
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/
#ifndef QUEUE_H
#define QUEUE_H

#include "customer.h"

#define TRUE (1)
#define FALSE (0)
#define BOOL unsigned int

#define MAX_QUEUE_SIZE (200)

struct queue {
	struct customer customer_arr[MAX_QUEUE_SIZE];
	unsigned int in_ptr;
	unsigned int out_ptr;
	unsigned int length;
};

void init_queue(struct queue * q);
unsigned int isEmpty(struct queue* q);
unsigned int push(struct queue* q, struct customer c);
struct customer pop(struct queue* q) ;


#endif