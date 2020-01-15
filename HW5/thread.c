//Anna Sedlackova

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
#include <pthread.h>
#include <stdio.h>

// Prototype for computation
double shubert(double x1, double x2);

double y, min1 = 0.0, min2 = 0.0;

void *child1(); /* the thread */
void *child2(); /* the thread */


int main (){
  

  pthread_t tid1, tid2; /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create the threads */
  pthread_create(&tid1,&attr,child1,NULL);
  pthread_create(&tid2,&attr,child2,NULL);

  /* now wait for the thread to exit */
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);

  printf("min1 = %f\n",min1);
  printf("min2 = %f\n",min2);
  

  if (min1 > min2) {
      printf("Overall minimum = %.2f\n", min2);
  } else {
      printf("Overall minimum = %.2f\n", min1);
  }
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

/**
 * Child threads will begin control in one of these functions
 */
void *child1() 
{
  double x1, x2;
    for (x1 = -2; x1 <= 0; x1 += 0.5) {
      for (x2 = -2; x2 <= 2; x2 += 0.5) {
        y = shubert(x1, x2);
        //printf("%.2f ", y);
        if (y < min1)
          min1 = y;
      }
      //printf("%f\n", y);
    }
  //printf("MINIMUMONE = %.2f ", min1);

  pthread_exit(0);
}

void *child2() 
{
  double x1, x2;
    for (x1 = 0; x1 <= 2; x1 += 0.5) {
     for (x2 = -2; x2 <= 2; x2 += 0.5) {
       y = shubert(x1, x2);
       //printf("%.2f ", y);
       if (y < min2)
         min2 = y;
      }
      //printf("%f\n", y);
    }
  //printf("MINIMUMTWO = %.2f ", min2);

  pthread_exit(0);
}