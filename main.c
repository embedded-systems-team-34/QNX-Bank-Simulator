#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include <sys/neutrino.h>
#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>

#define a1_MS (1000000)
#define a10_MS (10000000)
#define a100_MS (100000000)


uintptr_t port_a;
uintptr_t dir;

void print(){
	static unsigned int val = 0;
	val ^= 1;
	out8(port_a, val);
}

int main(int argc, char *argv[]) {
	//What to do when the timer expires
	struct sigevent event;
	struct sigaction action;

	//Parameters of the timer
	struct itimerspec timerSpec;
	timer_t timerID;

	//Amount of time to pass before the timer will expire
	//seconds
	timerSpec.it_value.tv_sec = 0;
	//Nanoseconds
	timerSpec.it_value.tv_nsec = 500000000;

	//Define the period of timer
	timerSpec.it_interval.tv_sec = 0;
	timerSpec.it_interval.tv_nsec = 100000;

	struct _clockperiod clkper;

	//Event, method address, method parameter, code

	// Set clock to a period of 10 us
	clkper.nsec       = 10000;
	clkper.fract      = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0  );

	struct timespec res;


	action.sa_sigaction = print;
	action.sa_flags = SA_SIGINFO;
	sigaction( SIGUSR1, &action, NULL );
	SIGEV_SIGNAL_INIT(&event, SIGUSR1);


	clock_getres( CLOCK_REALTIME, &res);

	printf( "Resolution is %ld micro seconds.\n",
	          res.tv_nsec / 1000 );

	timer_create(CLOCK_REALTIME , &event, &timerID);
	timer_settime(timerID, 0, &timerSpec, NULL);

    if ( ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {

    	printf("Failed to get I/O access permission");
    }

    port_a = mmap_device_io(1, 0x280 + 8);
    dir = mmap_device_io(1, 0x280 + 11);

    out8(dir,0x00);
    //out8(port_a,1);


    while(1) {

    }

	printf("Welcome to the QNX Momentics IDE\n");
	return EXIT_SUCCESS;
}
