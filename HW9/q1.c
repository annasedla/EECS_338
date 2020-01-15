//Anna Sedlackova axs1202
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <omp.h> 
#include <sys/time.h>

// Prototype for computation
double shubert(double step, double x2);

int main (){
  double x1, x2, y, min = 0, stepSize = 0.0005;
  int stepIndex;
  int numSteps = (int) (4.0 / stepSize);

  struct timeval start_time, stop_time, elapsed_time; 
  gettimeofday(&start_time, NULL);

  // Loops 
  #pragma omp parallel for
  for (stepIndex = 0; stepIndex <= numSteps; stepIndex++){
    double local_min = 0;
    int tid = omp_get_thread_num();

    if (tid == 0 && stepIndex == 0) {
      int nThreads = omp_get_num_threads();
      printf("%d threads, ", nThreads);
    }

    x1 = stepIndex * stepSize - 2;

    for (x2 = -2; x2 <= 2; x2 += stepSize) {
      y = shubert(x1, x2);
      if (y < local_min)
        local_min = y;
    }
    // Update global minimum less frequently
    #pragma omp critical
    if (local_min < min);

  }

  gettimeofday(&stop_time, NULL);
  timersub(&stop_time, &start_time, &elapsed_time); 
  printf("time = %.1f sec\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
}


// Compute Shubert value for x1,x2 combination
double shubert(double step, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}
