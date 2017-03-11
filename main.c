#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "customer.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    
    struct queue fifo;
    
    struct customer c1;
    struct customer c2;
    
    struct customer t1;
    struct customer t2;
    
    init_queue(&fifo);
    printf("Is the FIFO empty? %d\n", isEmpty(&fifo));
    c1.bankEntryTime = 5;
    c1.transactionStartTime=7123;
    c1.transactionEndTime=99;
    
    c2.bankEntryTime = 1;
    c2.transactionStartTime=2;
    c2.transactionEndTime=3;
    
    push(&fifo,c1);
    push(&fifo,c2);
    
    t1 = pop(&fifo);
    t2 = pop(&fifo);
    
    
	printf("Bank entry time is %d\n", t1.bankEntryTime);
    printf("Start time is %d\n", t1.transactionStartTime);
    printf("End time is %d\n", t1.transactionEndTime);
    
    printf("Bank entry time is %d\n", t2.bankEntryTime);
    printf("Start time is %d\n", t2.transactionStartTime);
    printf("End time is %d\n", t2.transactionEndTime);
    
	return EXIT_SUCCESS;
}