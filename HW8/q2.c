// Anna Sedlackova axs1202@case.edu

#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/sem.h>    
#include <sys/types.h>  
#include <stdlib.h>     
#include <semaphore.h>  
#include <string.h>

#define BUFFER_SIZE 20

// Struct for condition variable
struct condition {
	sem_t sem; // Semaphore to be initialized to 0
	int counter; // Count of threads waiting
};

/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t mutex, next;
struct condition notreading_condition;
int next_count = 0;
int version = 0;
int read_count = 0;

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *child(); // Child thread that will wait on condition variable
void writer(int v, char string[]);



int main(int argc, char *argv[]) {

	notreading_condition.counter = 0;

	
	sem_init(&mutex, 0, 1);
	sem_init(&(notreading_condition.sem), 0, 0);
	sem_init(&next, 0, 0);

	pthread_t tid1, tid2; // thread identifiers
	pthread_create(&tid1, NULL, child, NULL);
	pthread_create(&tid2, NULL, child, NULL);

	/* 
	WRITER
	*/
	writer(1, "Roses are red");
	sleep(1);
	writer(2, "Violets are blue");
	sleep(1);
	writer(3, "Synchronization is" );
	sleep(1);
	writer(4, "Quite fun to do");
	sleep(1);
	
	// Join
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	sem_destroy(&next);
	sem_destroy(&mutex);
	sem_destroy(&(notreading_condition.sem));

}

void *child() {
	pthread_t tid = pthread_self();
	int child_version = 0;

	while(child_version != 4){

		
		sem_wait(&mutex); //WAIT 1
		read_count++;
		//if (read_count == 1) {
		//	sem_wait(&rw_mutex);
		//}
	 	sem_post(&mutex); //POST 1

	 	/*
			READING IS PERFORMED HERE
	 	*/
		if (child_version != version){
			printf("%d: %s\n", (int)tid, buffer);
			child_version = version;
		}


		sem_wait(&mutex); //WAIT 2	
		read_count--;

		if (read_count == 0)
			cpost(&notreading_condition);

		if (next_count > 0)
			sem_post(&next);
		else
			sem_post(&mutex);

	}
	pthread_exit(0); 
}

void writer(int v, char string[]){

	sem_wait(&mutex);
	
	if (read_count > 0)
		cwait(&notreading_condition);

	sprintf(buffer, "%s", string);
	version = v;

	if (next_count > 0)
		sem_post(&next);
	else
		sem_post(&mutex);

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



