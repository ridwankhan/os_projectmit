#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>


void sieve(int myArray[]){
	
	int number = myArray[0];
	
	int root = sqrt(myArray[0]);
	//printf("root %d\n", root);
	

	for (int i=2; i<=root; i++){
		
		if (myArray[i] == 1){
			for (int j = i*i; j<=(number+1); j= j+i){
				
				myArray[j] = 0;
			}
		}
	}

	
	for (int i = 2; i<(number+1); i++){
		if (myArray[i] ==1){
			printf("Prime Number: %d\n", i);
		}
	}


}


void reversePrime(int myArray[]){
	int n;
	int reverse;
	int number = myArray[0];

	for (int i = 2; i < (number+1); ++i)
	{
		if (myArray[i] ==1)
		{
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
	//struct myStruct vars;
	//args = malloc(sizeof(struct myStruct);
	int number = atoi(argv[1]);
	int myArray[number+1];
	myArray[0] = number;

	for (int i = 1; i<(number+1); i++){
		myArray[i] = 1;
		//printf("main %d\n", myArray[i]);
	}
	

	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_create(&tid, &attr, (void*)sieve, &myArray);
	
	//sieve(myArray);
	//printf("returns\n");
	pthread_join(tid,NULL);
	//printf("number %d\n", number );
	
	pthread_t tid2;
	pthread_attr_t attr2;
	pthread_attr_init(&attr2);
	
	pthread_create(&tid2, &attr2, (void*)reversePrime, &myArray);
	 pthread_join(tid2,NULL);
	
	
	
	return 0;
	
}