// Ian Waldschmidt: Basic example of fork() without wait()
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid; // The process identifier

int main() {
	printf("I'm only printed once: %d\n", getpid());

	pid = fork();	// Create a new process starting here
	// fork() returns:
	// - a negative value if an error occurred
	// - 0 if we're in the child process
	// - a positive value if we're in the parent process

	if(pid < 0) {
		// Error case
		printf("Something went wrong: terminating.\n");
		return 0;
	}
	else if(pid == 0) {
		// Only the child process runs this
		// getpid() gets the 
		printf("Only the child prints this: %d\n", getpid());
	}
	else if(pid > 0) {
		// Only the parent process runs this
		printf("Only the parent prints this: %d\n", getpid());
	}

	printf("I'm printed twice: %d\n", getpid());
	//sleep(30); // Uncomment this and run top to see that fork creates a new process.
	return 1;
	// Try running this multiple times: you may get statements in different places
	// You can also try adding getppid() to get the id of the parent process
}