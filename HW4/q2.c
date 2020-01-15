// Anna Sedlackova (axs1202@case.edu)

#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N1 1000000000
#define N2 500000000
#define N3 333333333
#define N4 250000000

int main() {

	int number = 0;
	printf("What is the number of processes?");
	scanf( "%d", &number );

	long int i;
	long long int x = 0;
	pid_t pid;
	struct timeval start_time, stop_time, elapsed_time;  // timers
	gettimeofday(&start_time,NULL); // Unix timer

	/*
	1 PROCESS
	*/
	if (number == 1){
		for (i = 0; i < N1; i++) {
			x = x + 1;
		}
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
	} 

	/*
	2 PROCESSES
	*/
	else if (number == 2){
		pid = fork();
		if (pid > 0) {
			for (i = 0; i < N2; i++) {
				x = x + 1;
			}
			gettimeofday(&stop_time,NULL);
			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
			printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
		}
		else if (pid == 0) { /* child process */
			for (i = 0; i < N2; i++) {
				x = x + 1;
			}
			gettimeofday(&stop_time,NULL);
			timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
			printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
		}
	}

	/*
	3 PROCESSES
	*/
	else if (number == 3){
		pid = fork();

		if (pid == 0) {
			pid = fork(); 

		}
		for (i = 0; i < N3; i++) {
			x = x + 1;
		}
			
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
	}

	/*
	4 PROCESSES
	*/
	else if (number == 4) {
		pid = fork();

		if (pid > 0) {

			pid = fork();

			if (pid > 0) {

				pid = fork();
			}
		}

		for (i = 0; i < N4; i++) {
			x = x + 1;
		}

		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Process %d total time was %f seconds. x = %lld.\n", getpid(), elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
	}
}