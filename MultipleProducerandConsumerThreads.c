#include <stdio.h>
#include <pthread.h>

/*Specify number of each producer threads and consumer threads to create*/
#define PRODUCERSANDCONSUMERS 8

/*Specify maximum number of items to be produced by each producer thread*/
#define MAXITEMS 64

/*Specify maximum number of items to be held in the buffer*/
#define BUFFER_SIZE 8

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;

/*Produce items*/
void *producer(void *tid){
	long threadid;
	threadid = (long)tid;
	int j;
	for(j = 1; j <= MAXITEMS; j++){
/*Get exclusive access to buffer*/
		pthread_mutex_lock(&the_mutex);
/*Wait if buffer is full*/
		while (buffer == BUFFER_SIZE){
			pthread_cond_wait(&condp, &the_mutex);
			printf("Producer %ld waiting because buffer is full\n", threadid);
		}
/*Put item in buffer*/
			buffer++;
			printf("Producer %ld produced an item\n", threadid);
/*Wake up consumer*/
			pthread_cond_signal(&condc);
/*Release access to buffer*/
			pthread_mutex_unlock(&the_mutex);
	}	
		

	pthread_exit(0);	
}

/*Consume items*/
void *consumer(void *ctid){
	long cthreadid;
	cthreadid = (long)ctid;
	int i;
	for(i = 1; i <=MAXITEMS; i++){
/*Get exclusive access to buffer*/
		pthread_mutex_lock(&the_mutex);
/*Wait if buffer is empty*/
		while(buffer == 0){
			pthread_cond_wait(&condc, &the_mutex);
			printf("Consumer %ld waiting because buffer is empty\n", cthreadid);
		}
/*Remove an item from buffer*/
		buffer--;
		printf("Consumer %ld consumed an item\n", cthreadid);
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

int main (int argc, char **argv){
/*Initialize a set of Consumer and Producer threads*/
	pthread_t pro[PRODUCERSANDCONSUMERS], con[PRODUCERSANDCONSUMERS];
	pthread_mutex_init(&the_mutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);
	
	
/*Create the threads*/	
	for( long i = 0; i < PRODUCERSANDCONSUMERS; i++){
	
		pthread_create(&pro[i], 0, producer, (void *)i);
		pthread_create(&con[i], 0, consumer, (void *)i);
	
		
		
	}
	
	for (int i = 0; i < PRODUCERSANDCONSUMERS; i++){
		pthread_join(pro[i], 0);
		pthread_join(con[i], 0);
	}
	

	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
}
