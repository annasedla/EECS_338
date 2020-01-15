//Anna Sedlackova axs1202
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <sys/time.h>
#include <semaphore.h> 

#define N 10000
 
double globalMin = 0;
double x1, x2, y, z = 0;

sem_t sem; //global semaphore
double stepSize = 0.0005;
double threadInterval; 

void *child(void *x1_start_time);
double shubert(double step, double x2);




int main(int argc, char *argv[])
{

  // Set up semaphore
  if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

int nThreads;
  if (argc > 1)
    nThreads = atoi(argv[1]);
  else
    nThreads = 1; // Default




  // Prepare the child thread
  pthread_t tid[nThreads]; /* the thread identifier */
  double x1_start[nThreads];
  threadInterval = 4.0 / nThreads;

  struct timeval start_time, stop_time, elapsed_time;  // timers
  gettimeofday(&start_time, NULL); // Unix timer

  int i; // Loop iterator

  for (i = 0; i < nThreads; i++) {
    x1_start[i] = -2 + i * threadInterval;
    pthread_create(&tid[i], NULL, child, &x1_start[i]);
  }

  for (i = 0; i < nThreads; i++){
    pthread_join(tid[i], NULL);
    gettimeofday(&stop_time, NULL);
    timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
    printf("time = %.1f sec\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
  }
}



void *child(void *x1_start_time) {
  double x1, x2, y;
  double *x1_start = x1_start_time;
  double x1_stop = *x1_start + threadInterval - stepSize;

  double localMin = 0; // Local minimum for each thread
  for (x1 = *x1_start; x1 <= x1_stop; x1 += stepSize) {
    for (x2 = -2; x2 <= 2; x2 += stepSize) {
      y = shubert(x1, x2);
      if (y < localMin)
        localMin = y;
    }
  }

  sem_wait(&sem);
  if (localMin < globalMin)
    globalMin = localMin;
  sem_post(&sem);
  pthread_exit(0);
}


double shubert(double step, double x2){
  double sum1 = 0; 
  double sum2 = 0;
  int i;
  for (i = 1; i <= 5; i++) {
    sum1 += i * cos((i + 1) * x1 + i);
    sum2 += i * cos((i + 1) * x2 + i);
  }
  return sum1 * sum2;
}