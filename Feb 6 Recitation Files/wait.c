// Ian Waldschmidt: The right way to use wait
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid; // The process identifier

int main() {
	pid = fork();	// Create a new process starting here

	if(pid < 0) {
		// An error occurred: stop
		printf("Something went wrong: terminating.\n");
		return 0;
	}
	else if(pid == 0) {
		printf("Child pid = %d\n", getpid()); // This could also go above the wait(NULL) on line 33
		// Create a "grandchild" process (child process relative to the main child)
		pid = fork();

		if(pid < 0) {
			// An error occurred: stop
			printf("Something went wrong: terminating.\n");
			return 0;
		}
		else if(pid == 0) {
			// DO NOT PUT A WAIT STATEMENT HERE: IT WON'T DO ANYTHING
			printf("Grandchild pid = %d\n", getpid());
			printf("This gets printed first, always.\n");
		}
		else {
			wait(NULL); // This waits for its child to terminate (the grandchild)
			printf("This gets printed second, always.\n");
		}
	}
	else {
		// The parent process
		printf("Parent pid = %d\n", getpid());
		wait(NULL); // This waits for its child to terminate (the child)
					// This won't finish when the grandchild terminates because the grandchild isn't its child.
		printf("This gets printed last, always.\n");
	}

	printf("All done: pid = %d\n", getpid());
	return 1;
	// Notice that with this hierarchy (parent -> child -> grandchild -> ...), stuff before the wait(NULL) statements
	// could be done in any order, but stuff after the wait(NULL) statements are done in exactly 1 order.
	
}