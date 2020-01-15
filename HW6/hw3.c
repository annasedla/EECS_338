//Anna Sedlackova axs1202
#include <stdio.h>
#include <stdlib.h> 
#include <semaphore.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h> // For wait


// Prototype for computation
double shubert(double x1, double x2);
double x1, x2, y, min = 0;
pid_t pid;

int main(int argc, char *argv[]) {

  sem_t *sem; //global semaphore
  const int SIZE = 1;
  const char *name = "AnnaS";
  int shm_fd;

  // Create shared memory for semaphore
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd,SIZE);
  sem = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (sem == MAP_FAILED) {
    printf("Map failed\n");
    exit(0);
  }

  // Set up semaphore
  if(sem_init(sem, 1, 1) < 0) { // 0 = multithreaded
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  pid = fork();

  if (pid == 0){

    sem_wait(sem); // Decrement semaphore and lock/wait till available
    for (x1 = 0; x1 <= 2; x1 += 0.5) {
      for (x2 = 0; x2 <= 2; x2 += 0.5) {
        y = shubert(x1, x2);
        if (y < min)
         min = y;
      }
    printf("values of x in child: %f\n", (x1));
    }
    sem_post(sem); // Done, so increment semaphore
  } else {

  sem_wait(sem); // Decrement semaphore and lock/wait till available
  for (x1 = -2; x1 <= 0; x1 += 0.5) {
    for (x2 = -2; x2 <= 0; x2 += 0.5) {
      y = shubert(x1, x2);
      if (y < min)
        min = y;
      }
      printf("values of x in parent: %f\n", (x1));
    }
    sem_post(sem); // Done, so increment semaphore

    wait(NULL); // Need to wait because someone needs to clean up
    printf("Minimum = %f \n", min);
    
    sem_destroy(sem); // Clean up semaphore
    if (shm_unlink(name) == -1) { // Clean up shared memory
      printf("Error removing %s\n",name);
      exit(0);
    }
  }
}

double shubert(double x1, double x2){
  double sum1 = 0; 
  double sum2 = 0;
  int i;
  for (i = 1; i <= 5; i++) {
    sum1 += i * cos((i + 1) * x1 + i);
    sum2 += i * cos((i + 1) * x2 + i);
  }
  return sum1 * sum2;
}