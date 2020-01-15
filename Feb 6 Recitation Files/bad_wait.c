// Ian Waldschmidt: The wrong way to use wait
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
		// A child process
		wait(NULL); // THIS DOES NOTHING because this process (the child) has no child processes.
		printf("I thought this would print second.\n");
	}
	else {
		pid = fork();

		if(pid < 0) {
			// An error occurred: stop
			printf("Something went wrong: terminating.\n");
			return 0;
		}
		else if(pid == 0) {
			// Another child process (a "sibling" to the child process above)
			printf("This is supposed to print first.\n");
		}
		else {
			wait(NULL); // Since this process (the parent) has 2 child processes, this waits for EITHER ONE to finish (NOT BOTH).
			printf("This won't be first, but it could be second or third.\n");
		}
	}

	return 1;
	// Since this has siblings, the run order is much harder to control. wait(NULL) doesn't work on siblings or parents, so use that wisely.
	
}