#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>


void sieve(int myArray[]){
	//calculate sieve
	
	int number = myArray[0];
	
	int root = sqrt(myArray[0]);
	
	

	for (int i=2; i<=root; i++){
		
		if (myArray[i] == 1){
			for (int j = i*i; j<=(number+1); j= j+i){
				
				myArray[j] = 0;
			}
		}
	}

	//print prime numbers
	for (int i = 2; i<(number+1); i++){
		if (myArray[i] ==1){
			printf("Prime Number: %d\n", i);
		}
	}


}


void reversePrime(int myArray[]){
	//find primes whose reverse is also a prime
	int n;
	int reverse;
	int number = myArray[0];

	for (int i = 2; i < (number+1); ++i)
	{
		if (myArray[i] ==1)
		{
			//to reverse an integer used the following source
			//http://www.programmingsimplified.com/c/source-code/c-program-reverse-number
			n = i;
			reverse = 0;
			while (n != 0){
				reverse = reverse*10;
				reverse = reverse + n%10;
				n = n/10;
			}
			if ((reverse <= number) && myArray[reverse])
			{
				printf("Reverse Primes: %d\n", i);
			}
		}
		
	}
	
	
}


int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}
	
	int number = atoi(argv[1]);
	int myArray[number+1];
	myArray[0] = number;

	//initialize arrat values to 1 to be true
	for (int i = 1; i<(number+1); i++){
		myArray[i] = 1;
	
	}
	

	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//create the thread
	pthread_create(&tid, &attr, (void*)sieve, &myArray);
	//wait for thread to return
	pthread_join(tid,NULL);
	
	
	pthread_t tid2;
	pthread_attr_t attr2;
	pthread_attr_init(&attr2);

	//create the thread
	pthread_create(&tid2, &attr2, (void*)reversePrime, &myArray);
	//wait for thread to return
	pthread_join(tid2,NULL);
	
	
	
	return 0;
	
}