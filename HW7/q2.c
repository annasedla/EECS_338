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

/* Global variables are shared by the thread(s) */
char buffer[BUFFER_SIZE];
sem_t rw_mutex, mutex;
int version = 0;
int read_count = 0;

void *child(); /* child thread */

int main(int argc, char *argv[]) {
	if(sem_init(&rw_mutex, 0, 1) < 0 || sem_init(&mutex, 0, 1) < 0) { // 0 = multithreaded
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	pthread_t tid1, tid2; /* the thread identifiers */
	pthread_create(&tid1, NULL, child, NULL);
	pthread_create(&tid2, NULL, child, NULL);

	/* 
	WRITER
	*/
	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Roses are red");
	version = 1;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Violets are blue");
	version = 2;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Synchronization is");
	version = 3;
	sem_post(&rw_mutex);
	sleep(1);

	sem_wait(&rw_mutex);
	sprintf(buffer, "%s", "Quite fun to do");
	version = 4;
	sem_post(&rw_mutex);
	sleep(1);

	// Join
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	sem_destroy(&rw_mutex);
	sem_destroy(&mutex);

}

void *child() {
	pthread_t tid = pthread_self();
	int child_version = 0;

	while(child_version != 4){
		sem_wait(&mutex); //WAIT 1
		read_count++;
		if (read_count == 1) {
			sem_wait(&rw_mutex);
		}
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

		if (read_count == 0){
			sem_post(&rw_mutex);
		}
		sem_post(&mutex); //POST 2

	}
	pthread_exit(0); 
}
