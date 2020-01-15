#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
int x = 10;
pid_t pid;
pid = fork();
x = x + 2;
if (pid == 0) {
	x = x + 1;
	printf("x=%d\n", x);
}
printf("x=%d\n", x);

}
