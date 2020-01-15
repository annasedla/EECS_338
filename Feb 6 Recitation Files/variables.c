// Ian Waldschmidt: Show how variable manipulation works with multiple processes
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid; // The process identifier
int x = 5; // A global variable

int main() {
	pid = fork();	// Create a new process starting here

	if(pid < 0) {
		// An error occurred: stop
		printf("Something went wrong: terminating.\n");
		return 0;
	}
	else if(pid == 0) {
		printf("Before, x = %d\n", x);
		x = 10;
		printf("After, x = %d\n", x);
	}
	else {
		wait(NULL); // wait for the child to finish running
		printf("Child ran, x = %d\n", x);
	}

	printf("What's x here? x = %d\n", x);
	return 1;
	// Because the parent and child are different processes, the child gets its own copy of x, NOT the same as the parent's x
	// To share x between parent/child, you need SHARED MEMORY (later)
}