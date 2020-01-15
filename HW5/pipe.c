//Anna Sedlackova
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 25
#define READ_END  0
#define WRITE_END 1

// Prototype for computation
double shubert(double x1, double x2);

int main (){
  
  pid_t pid;
  double x1, x2, y, min1 = 0, min2 = 0, min = 0;
  
  //write data
  double write_data[BUFFER_SIZE];

  //read data
  double read_data[BUFFER_SIZE];

  int fd[2];

  /* create the pipe */
  if (pipe(fd) == -1) {
    fprintf(stderr,"Pipe failed");
    return 1;
  }

  pid = fork();

  /*
  1st CHILD PROCESS
  */
  if (pid == 0){

    // 1st for loop
    for (x1 = -2; x1 <= 0; x1 += 0.5) {
      for (x2 = -2; x2 <= 2; x2 += 0.5) {
        y = shubert(x1, x2);
        //printf("%.2f ", y);
        if (y < min1)
          min1 = y;
      }
      //printf("%f\n", y);
    }
    printf("minimum1 = %.2f\n", min1);

    //Write Data
    write_data[0] = min1;

    /* write to the pipe */
    write(fd[WRITE_END], write_data, sizeof(write_data)); 
  }

  /*
  PARENT PROCESS
  */
  else if (pid > 0) {

    // 2nd for loop
    for (x1 = 0; x1 <= 2; x1 += 0.5) {
     for (x2 = -2; x2 <= 2; x2 += 0.5) {
       y = shubert(x1, x2);
       //printf("%.2f ", y);
       if (y < min2)
         min2 = y;
      }
      //printf("%f\n", y);
    }
    printf("minimum2 = %.2f\n", min2);

    wait (NULL);

    /* read from the pipe */
    read(fd[READ_END], read_data, sizeof(write_data));

    if (*read_data > min2) {
      printf("Overall minimum = %.2f\n", min2);
    }
    else {
      printf("Overall minimum = %.2f\n", read_data[0]);
    }
  }
}

/* 
Compute Shubert value for x1,x2 combination
*/
double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}