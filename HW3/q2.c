#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For wait()

int main (){

	pid_t pid;
	pid_t pid2;

	pid = fork();

	if (pid > 0) { //parent process
		printf("Process 1 = %d, parent = %d\n", getpid(), getppid());

 		pid2 = fork();
 		wait(NULL);  //wait for first child process to finnish

 		if (pid2 > 0) { 
 			wait(NULL); // wait for 2nd child to finnish

 			printf("Child %d exited.\n", pid);
 			printf("Child %d exited.\n", pid2);
 			
 		} else {//2nd child process
 			printf("Process 3 = %d, parent = %d\n", getpid(), getppid());
 		}
	}

	else { //1st child process
 		printf("Process 2 = %d, parent = %d\n", getpid(), getppid());;
 	}
}