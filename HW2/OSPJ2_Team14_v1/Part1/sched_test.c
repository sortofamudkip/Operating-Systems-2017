#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>
//Report must write: why can't while (((double)(clock()-S))/CLOCKS_PER_SEC <= 0.25f) ; use sleep instead
//use busy waiting you are always occupy CPU but sleep you will be kicked out of CPU then thread II will run immediately. 
//Then you can't reach the effect of FIFO
//Faced Difficulties: 
//1. we initially use sleep then it always be wrong.
//2. if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) forget to type sudo at the terminal.
//3. use SCHED_FIFO the terminal will freeze then output the all outputs at once(or at the same time).

void* run_thread(void *arg) {
	int t = (int)arg +1;
	// printf("recieved %d for tid %u\n", t, (unsigned int)pthread_self());
	for (int i = 0; i < 3; ++i) {
		printf("Thread %d is running\n", t); 
		clock_t S = clock();
		while (((double)(clock()-S))/CLOCKS_PER_SEC <= 0.25f) ;
		// printf("runtime: %f\n", (double)(clock()-S)/CLOCKS_PER_SEC);
	}
	return (void*)0;
}

void err_sys(char *S) {
	puts(S);
	exit(1);
}

int main(int argc, char const *argv[]) {

	if (argc == 2 && !strcmp(argv[1], "SCHED_FIFO")) {
		//set CPU affinity
		cpu_set_t set;
		CPU_ZERO(&set);
		CPU_SET(0, &set);
		if (sched_setaffinity(0, sizeof(cpu_set_t), &set) != 0) err_sys("could not set affinity");

		//setting sched_priority
		struct sched_param param;
		sched_getparam(0, &param);
		param.sched_priority = sched_get_priority_max(SCHED_FIFO)-1;


		// int result = sched_getscheduler(0);
		// if (result == SCHED_FIFO) puts("before: policy is FIFO");
		// if (result == SCHED_OTHER) puts("before: policy is other");

		//set the policy to FIFO
		if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) err_sys("could not change scheduler. Try using sudo.");

		// int result = sched_getscheduler(0);
		// if (result == SCHED_FIFO) puts("after: policy is FIFO");
		// if (result == SCHED_OTHER) puts("after: policy is other");
	}
	//run the two threads
	pthread_t pid[2];
	for (int i = 0; i < 2; ++i) {
		pthread_create(&pid[i], NULL, run_thread, (void*)i);
		printf("Thread %d was created\n", i+1); 
	}
	for (int i = 0; i < 2; ++i) {
		pthread_join(pid[i], NULL);
	}
	
	return 0;
}
