#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

void sieve(int myArray[],int number){
	int root = sqrt(number);
	printf("%d\n", number);
	printf("%d\n", root);

	for (int i=2; i<=root; i++){
		if (myArray[i-1] == 1){
			for (int j = i*i; j<=number; j= j+i){
				printf("here %d\n",j);
				myArray[j-1] = 0;
			}
		}
	}

}

int main(int argc, char *argv[]){
	int number = atoi(argv[1]);
	int myArray[number];
	for (int i = 0; i<number; i++){
		myArray[i] = 1;
	}
	sieve(myArray, number);
	for (int i = 1; i<number; i++){
		if (myArray[i] ==1){
			printf("prime %d\n", i+1);
		}
	}
	printf("%d\n", number);
}