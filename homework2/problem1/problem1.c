#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

struct myStruct{

	
	int number;
	int myArray[];

	};

void sieve(struct myStruct args){
	printf("here now %d\n", args.number);
	int root = sqrt(args.number);
	//printf("%d\n", number);
	//printf("%d\n", root);

	for (int i=2; i<=root; i++){
		if (args.myArray[i-1] == 1){
			for (int j = i*i; j<=args.number; j= j+i){
				printf("here %d\n",j);
				args.myArray[j-1] = 0;
			}
		}
	}

}

void reversePrime(int myArray[], int number){
	int n;
	int reverse;

	for (int i = 1; i < number; ++i)
	{
		if (myArray[i] ==1)
		{
			n = i+1;
			reverse = 0;
			while (n != 0){
			reverse = reverse*10;
			reverse = reverse + n%10;
			n = n/10;
			}
			if ((reverse <= number) && myArray[reverse-1])
			{
				printf("%d\n", i+1);
			}
		}
		
	}
	
}

int main(int argc, char *argv[]){
	struct myStruct args;
	args.number = atoi(argv[1]);
	args.myArray[args.number];
	for (int i = 0; i<args.number; i++){
		args.myArray[i] = 1;
	}

	printf("pr %d\n", args.number);
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_create(&tid, &attr, (void*)sieve, &args);
	//sieve(myArray, number);
	pthread_join(tid,NULL);
	for (int i = 1; i<args.number; i++){
		if (args.myArray[i] ==1){
			printf("prime %d\n", i+1);
		}
	}

	//reversePrime(myArray, number);
	 

	printf("%d\n", args.number);
	
}