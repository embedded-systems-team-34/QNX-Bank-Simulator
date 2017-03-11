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

#include "queue.h"

void init_queue(struct queue* q) {
    q->in_ptr = 0;
	q->out_ptr = 0;
	q->length = 0;
}

unsigned int isEmpty(struct queue* q) {
    if (q->length == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Push an element onto the queue and return TRUE iff the push is successful
unsigned int push(struct queue* q, struct customer c) {
    // First check that Queue is not full 
    if (q->length == MAX_QUEUE_SIZE) {
        return FALSE;
    }
    // Queue is not full so add the customer
    q->customer_arr[q->in_ptr] = c;
    q->in_ptr = (q->in_ptr + 1) % MAX_QUEUE_SIZE; 
    return TRUE;
}

struct customer pop(struct queue* q) {
    struct customer c;
    c = q->customer_arr[q->out_ptr];
    q->out_ptr = (q->out_ptr + 1) % MAX_QUEUE_SIZE; 
    return c;
    
}
