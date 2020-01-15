//Anna Sedlackova axs1202
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <omp.h> 

// Prototype for computation
double shubert(double x1, double x2);

int main (){
  double x2, y, min = 0;
  int x1;
  int tid;
  // Loops 

  #pragma omp parallel for private(tid, y, x2)
  for (x1 = -4; x1 <= 4; x1 += 1) {
    for (x2 = -2; x2 <= 2; x2 += 0.5) {	
      y = shubert(x1/2.0, x2);
      #pragma omp critical
      if (y < min){
        min = y;
    	}	
    }
    tid = omp_get_thread_num();
    if (tid== 0){	
    	printf("values of x in thread %d: %f\n", tid,(x1/2.0));
	  }
    else if (tid==1){
	   printf("values of x in thread %d: %f\n", tid, x1/2.0);
	  }
  }
  printf("minimum = %.2f\n ", min);
}

// Compute Shubert value for x1,x2 combination
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
