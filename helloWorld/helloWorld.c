#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

void print(){
	printf("Test\n");
}

int main(int argc, char *argv[]) {
	//What to do when the timer expires
	struct sigevent event;

	//Parameters of the timer
	struct itimerspec timerSpec;
	timer_t timerID;

	//Amount of time to pass before the timer will expire
	//seconds
	timerSpec.it_value.tv_sec = 5;
	//Nanoseconds
	timerSpec.it_value.tv_nsec = 500;

	//Define the period of timer
	timerSpec.it_interval.tv_sec = 10;
	timerSpec.it_interval.tv_nsec = 0;

	//Event, method address, method parameter, code
	SIGEV_THREAD_INIT(&event, print, NULL, 0 );

	timer_create(CLOCK_REALTIME, &event, &timerID);
	timer_settime(timerID, 0, &timerSpec, NULL);



	while(1){

	}

	printf("Welcome to the QNX Momentics IDE\n");
	return EXIT_SUCCESS;
}
