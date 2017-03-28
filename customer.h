/******************************************************************************
* FILENAME : customer.h          
*
* DESCRIPTION : 
*     Customer prototypes
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/
#ifndef CUSTOMER_H
#define CUSTOMER_H

struct customer {
	unsigned int bankEntryTime;
	unsigned int transactionStartTime;
	unsigned int transactionEndTime;
};

#endif