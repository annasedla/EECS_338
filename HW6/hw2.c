//Anna Sedlackova axs1202
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
 
double min = 0;
double x1, x2, y, z = 0;

sem_t sem; //global semaphore

void *child();
double shubert(double x1, double x2);

int main(int argc, char *argv[]) {
  // Set up semaphore
  if(sem_init(&sem, 0, 1) < 0) { // 0 = multithreaded
    fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
    exit(0);
  }

  // Prepare the child thread
  pthread_t tid; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, child, NULL);

  sem_wait(&sem); // Decrement semaphore and lock/wait till available
  for (x1 = -2; x1 <= 0; x1 += 0.5) {
    for (x2 = -2; x2 <= 0; x2 += 0.5) {
      y = shubert(x1, x2);
      if (y < min)
        min = y;
      }
      printf("values of x in parent: %f\n", (x1));
    }

  sem_post(&sem); // Done, so increment semaphore
  pthread_join(tid, NULL); // Need to wait because someone needs to clean up
  printf("Minimum = %f \n", min);
  sem_destroy(&sem); // Clean up semaphore
}

void *child() {
  sem_wait(&sem); // Decrement semaphore and lock/wait till available
  // Child tries to give a speech
  for (x1 = 0; x1 <= 2; x1 += 0.5) {
    for (x2 = 0; x2 <= 2; x2 += 0.5) {
      z = shubert(x1, x2);
      if (z < min)
        min = z;
    }
    printf("values of x in child: %f\n", (x1));
  }
  sem_post(&sem); // Done, so increment semaphore
  pthread_exit(0);
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