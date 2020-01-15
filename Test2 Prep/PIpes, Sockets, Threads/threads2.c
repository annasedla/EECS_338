// Chris Fietkiewicz (chris.fietkiewicz@case.edu)
// Based on sample code from Gagne, Galvin, Silberschatz

#include <pthread.h>
#include <stdio.h>

int sum1, sum2; /* this data is shared by the thread(s) */

void *child1(); /* the thread */
void *child2(); /* the thread */

int main(int argc, char *argv[])
{
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

	printf("sum1 = %d\n",sum1);
	printf("sum2 = %d\n",sum2);
	printf("total = %d\n",sum1 + sum2);
}

/**
 * Child threads will begin control in one of these functions
 */
void *child1() 
{
	int i;
	sum1 = 0;
	for (i = 1; i <= 10; i++)
		sum1 += i;
	pthread_exit(0);
}

void *child2() 
{
	int i;
	sum2 = 0;
	for (i = 11; i <= 20; i++)
		sum2 += i;
	pthread_exit(0);
}
