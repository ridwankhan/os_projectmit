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

void sieve(struct myStruct* args){

	printf("here now %d\n", args->number);
	int root = sqrt(args->number);
	printf("root %d\n", root);
	for (int i = 0; i<args->number; i++){
		
		printf("array %d\n", args->myArray[i]);
	}

	for (int i=2; i<=root; i++){
		printf("hi there ");
		printf("%d\n", args->myArray[i-1]);
		if (args->myArray[i-1] == 1){
			for (int j = i*i; j<=args->number; j= j+i){
				printf("j number %d\n",j);
				printf("editing %d\n", j-1);
				args->myArray[j-1] = 0;
			}
		}
	}

	printf("this index %d\n",args->myArray[6]);

}

/*
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
*/

int main(int argc, char *argv[]){
	struct myStruct vars;
	//args = malloc(sizeof(struct myStruct);
	vars.number = atoi(argv[1]);
	vars.myArray[vars.number];
	for (int i = 0; i<vars.number; i++){
		vars.myArray[i] = 1;
		printf("main %d\n", vars.myArray[i]);
	}
	

	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_create(&tid, &attr, (void*)sieve, &vars);

	//sieve(&vars);
	//printf("returns\n");
	//pthread_join(tid,NULL);
	printf("number %d\n", vars.number );
	
	for (int i = 1; i<vars.number; i++){
		if (vars.myArray[i] ==1){
			printf("prime %d\n", i+1);
		}
	}

	//reversePrime(myArray, number);
	 
	for (int i = 0; i<vars.number; i++){
		
		printf("main %d\n", vars.myArray[i]);
	}
	printf("%d\n", vars.number);
	
	return 0;
	
}