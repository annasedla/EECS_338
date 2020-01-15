#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For wait()

int main (){

	pid_t pid;
	pid_t pid2;

	pid = fork();

	if (pid > 0){ //parent process
		printf("Process 1 = %d, parent = %d\n", getpid(), getppid());
		wait(NULL); //wait for first child to finnish
 	} 

	else{ //1st child process
		
		pid2 = fork();

		if (pid2 > 0){ //2nd parent process
			printf("Process 2 = %d, parent = %d\n", getpid(), getppid());
			wait(NULL); // wait for 2nd child to finnish
		} 

		else {// grand child process
			printf("Process 3 = %d, parent = %d\n", getpid(), getppid());
		} 		
	}
}
  	
 		

