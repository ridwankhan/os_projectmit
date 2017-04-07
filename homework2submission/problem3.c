/*Implement a synchronization scheme that simulates the behavior of the customers of a bank as follows: 
The bank acquires K seats in the waiting room and a customer service desk. The customers can see from the window if there is free seating. 
If there is not, they go for a walk (take_a_walk ()) and retry later.If there is seating available, they enter the waiting room and 
attempt to be served one at a time in the customer service desk. The client is served by calling make_transaction (). 
Use shared variables and semaphores for your solution. Consider making any changes necessary at the points indicated with.  ... in 
the code segment that follows:
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


int shared;
sem_t roomAvailable;
sem_t serviceAvailable;
int k =3;

void make_transaction()
{
	// makes the transaction
	
	sleep(2);
	printf("Customer %u Transaction Made! \n",(int) pthread_self());
	sem_post(&serviceAvailable);

}

void take_a_walk()
{
	// makes the customer come back and try again 
	printf("Customer %u took a walk! \n",(int) pthread_self());
	sleep(2);
}
void return_home()
{
	//removes the customer
	printf("Customer %u Removed! \n",(int) pthread_self());
	sem_post(&roomAvailable);
	
}
void bank_client()
{

	while(1)
	{
		
		printf("Hi I am thread %u \n", (int) pthread_self());

		int currentvalue;


		sem_getvalue(&roomAvailable, &currentvalue);

		printf("Current number of seats available %d\n",currentvalue);


		if(currentvalue != 0)
		{
			sem_wait(&roomAvailable); // blocks if no room is available

			sem_wait(&serviceAvailable);
			printf("Customer %u making transaction \n", (int) pthread_self());
			make_transaction();
			break;

		}
		
		else
		{
			take_a_walk();

		}

	}

	return_home();
}

int main(int argc, char **argv)
{	
	sem_init(&roomAvailable,0,k);
	sem_init(&serviceAvailable,0,1);


	int NUM_THREADS = 5; // Create 15 customers

	pthread_t threads[NUM_THREADS];
	int rc;
	void *status;


	for(int i=0; i<NUM_THREADS; i++)

	{
		//create threads
		rc = pthread_create(&threads[i], NULL, (void *)bank_client, NULL);
		
	}

	
	
	for(int j =0; j<NUM_THREADS; j++)
	{
		//join threads
		int rc = pthread_join(threads[j], &status);
		
	}
	

	return 0;

}