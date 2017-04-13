#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int x;
	int i;
	FILE *myFile = fopen("numbers.txt", "w");

	if (myFile == NULL){
		printf("Error opening file\n");
		//exit(1);
	}
	for (i = 0; i<1000; i++){
		srand((i+1)*time(0));
		x = 1 + rand()%100000;
		fprintf(myFile, "%d\n", x );

	}
	//printf("done\n");
	fclose(myFile);

	return 0;
}