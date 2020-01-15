//Anna Sedlackova axs1202
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/time.h>
#include <semaphore.h>  


// Prototype for computation
double shubert(double step, double x2);


int main(int argc, char *argv[]) {

  const int SIZE = 1;
  const char *name = "AnnaS";
  int shm_fd, shm_fd2;

  // Create shared memory for semaphore
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  ftruncate(shm_fd,SIZE);
  double *min = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (min == MAP_FAILED) {
    printf("Map failed\n");
    exit(0);
  }


  const int SIZE2 = 1;
  const char *name2 = "AnnaS";
  // Create shared memory for semaphore
  shm_fd2 = shm_open(name2, O_CREAT | O_RDWR, 0666);

  ftruncate(shm_fd2,SIZE2);
  sem_t *sem = mmap(0,SIZE2, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (sem == MAP_FAILED) {
    printf("Map failed\n");
    exit(0);
  }

  sem_init(sem, 1, 1); // 1 = multiprocess

  int nProcs;
  if (argc > 1)
    nProcs = atoi(argv[1]);
  else
    nProcs = 1; // Default
  double interval = 4.0 / nProcs;

  double stepSize = 0.0005; // Intervals between x-values within each thread
  min[0] = 0;
  int iProc = 0; // Index of process
  int nextProc; // Iterator for process creation
  pid_t pid = 0;
  struct timeval start_time, stop_time, elapsed_time;  // timers
  gettimeofday(&start_time,NULL); // Unix timer
  for (nextProc = 1; nextProc < nProcs & !pid; nextProc++) {
    pid = fork();
    if (!pid)
      iProc = nextProc;
  }

  double x1, x2, y;
  double x1_start = -2 + iProc * interval;
  double x1_stop = x1_start + interval - stepSize;

  double localMin = 0; // Local minimum for each thread
  for (x1 = x1_start; x1 <= x1_stop; x1 += stepSize) {
    for (x2 = -2; x2 <= 2; x2 += stepSize) {
      y = shubert(x1, x2);
      if (y < localMin)
        localMin = y;
    }
  }

    sem_wait(sem);
    if (localMin < min[0])
      min[0] = localMin;
    sem_post(sem);
    wait(NULL);

  // Parent does printing and cleanup
  if (iProc == 0) {
    gettimeofday(&stop_time, NULL);
    timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
    printf("time = %.1f sec\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
    // Unlink shared memory
    if (shm_unlink(name) == -1) {
      printf("Error removing %s\n", name);
      exit(-1);
    }
    if (shm_unlink(name2) == -1) {
      printf("Error removing %s\n", name2);
      exit(-1);
    }
  }
  return 0;

}


double shubert(double step, double x2){
  double sum1 = 0; 
  double sum2 = 0;
  int i;
  for (i = 1; i <= 5; i++) {
    sum1 += i * cos((i + 1) * step + i);
    sum2 += i * cos((i + 1) * x2 + i);
  }
  return sum1 * sum2;
}