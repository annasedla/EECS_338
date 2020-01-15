// Anna Sedlackova (axs1202@case.edu)

#include <unistd.h> // Required for execvp
#include <stdio.h> // Required for I/O
#include <string.h> //for string operations
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h> 

#define MAX_LINE 80 /* 80 chars per line, per command */

int main() {

	pid_t pid;
	char line[50]; //????
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int should_run = 1;

 	while (should_run == 1){ 

  		printf("anna>");
		gets(line);

		args[0] = strtok(line, " ");

		int i = 0;

		while (args[i] != NULL) {
			args[++i] = strtok(NULL, " ");
		}

		if (!strcmp(args[0], "exit")) {	
			should_run = 0;
			break;

		} else {

			pid = fork();

			if (pid == 0){
				execvp(args[0], args); // Execute the command
				kill(getpid(), SIGKILL);

			} else {
			
				wait(NULL);
			}
		}
	}
}
