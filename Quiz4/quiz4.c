// Anna Sedlackova (axs1202@case.edu)

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define NC 100000000
#define NP 100000000

int main(void)
{
    long int i;
    long long int x = 0;
	pid_t pid;
	pid = fork();

	const int SIZE = 4069;
	const char *name = "x";
	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	//FIRST CHILD RUNS PID
	if (pid == 0){
		char *args[4];
		args[0] = "top";
		args[1] = 0;
		execvp(args[0], args);
	}
	else {

		pid = fork();

		//SECOND CHILD
		if (pid == 0){
			for (i = 0; i < NC; i++) {
				x = x + 1;
			}

		//writes to the shared memory	
		sprintf(ptr," %lld ", x);

		//PARENT	
		} else {
			for (i = 0; i < NP; i++) {
				x = x + 1;
			}

			wait(NULL);
			wait(NULL);
			
			printf("The sum of NC + NP is:  %lld ", (x + atoi(ptr)));

			/* remove the shared memory segment */
			if (shm_unlink(name) == -1) {
				printf("Error removing %s\n",name);
				exit(-1);
			}
		}
	}
}