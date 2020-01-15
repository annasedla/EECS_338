//Anna Sedlackova axs1202@case.edu

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>     
#include <sys/types.h>  
#include <stdlib.h>     
#include <semaphore.h>  

#define N 10
#define BUFFER_SIZE 5

// Struct for condition variable
struct condition {
	sem_t sem; // Semaphore to be initialized to 0
	int counter; // Count of threads waiting
};

/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t mutex, next;
struct condition notfull_condition, notempty_condition;
int next_count = 0;
int counter = 0;

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *child(); /* child thread */




int main(int argc, char *argv[]) {

	sem_init(&(notfull_condition.sem), 0, 0);
	sem_init(&(notempty_condition.sem), 0, 0);
	sem_init(&next, 0, 0);
	sem_init(&mutex, 0, 1);

	notempty_condition.counter = 0;
	notfull_condition.counter = 0;

	pthread_t tid; /* the thread identifiers */
	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, child, NULL);

	/*
	 Parent information (writes to buffer)
	 */
	int data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Must have N values
	int i, input = 0;

	for (i = 0; i < N; i++){
		sem_wait(&mutex);


		//meat of the hw
		if (counter == BUFFER_SIZE)
			cwait(&notfull_condition);
		buffer[input] = data[i];
		counter++; //increase counter
		cpost(&notempty_condition); //post that the buffer is not empty


		if (next_count > 0)
			sem_post(&next);
		else
			sem_post(&mutex);
		//increment buffer pointer
		input = (input + 1) % BUFFER_SIZE;
	}

	// Join and print result
	pthread_join(tid, NULL);
	sem_destroy(&mutex);
	sem_destroy(&next);
	sem_destroy(&(notempty_condition.sem));
	sem_destroy(&(notfull_condition.sem));
	
}

void *child() {

	/*
	 Child information (retrieves from buffer)
	 */

	int i, output = 0;

	for (i = 0; i < N; i++){

		sem_wait(&mutex);



		//meat of the HW
		if (counter == 0)
			cwait(&notempty_condition);
		int data = buffer[output];
		printf("%d\n", data);
		counter--; // decrement count
		cpost(&notfull_condition); //post that not full





		if (next_count > 0)
			sem_post(&next);
		else
			sem_post(&mutex);

		//increment buffer pointer
		output = (output + 1) % BUFFER_SIZE;
	}
	pthread_exit(0);
}

// Semaphore implementation of conditional wait
void cwait(struct condition *c) {
	c->counter++;
	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);
	sem_wait(&(c->sem));
	c->counter--;
}

// Semaphore implementation of conditional signal
void cpost(struct condition *c) {
	if (c->counter > 0) {
		next_count++;
		sem_post(&(c->sem));
		sem_wait(&next);
		next_count--;
	}
}
