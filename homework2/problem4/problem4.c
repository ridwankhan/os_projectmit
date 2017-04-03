#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

int R = 3;

void Teacher() {

	for (int i = 0; i < count; ++i)
	{
		teacher_enter();
		//critical section
		teach();
		teacher_exit();
		go_home();
	}
}

void Child(){
	for (int i = 0; i < count; ++i)
	{
		child_enter();
		//critical section
		learn();
		child_exit();
		go_home();
	}
}

void Parent(){
	for (int i = 0; i < count; ++i)
	{
		parent_enter();
		//critical seciton
		parent_exit();
		go_home();
	}
}

int main(int argc, char *argv[]){

	int NUM_CHILD = 20;
	int NUM_TEACHER = 8;
	int NUM_PARENT = 20;

	pthread_t childThreads[NUM_CHILD];
	pthread_t teacherThreads[NUM_TEACHER];
	pthread_t parentThreads[NUM_PARENT];

	for (int i = 0; i < NUM_CHILD; ++i)
	{
		pthread_create(&childThreads[i], NULL, (void *)Child, NULL);
	}
	
	for (int i = 0; i < NUM_TEACHER; ++i)
	{
		pthread_create(&teacherThreads[i], NULL, (void *)Teacher, NULL);
	}

	for (int i = 0; i < NUM_PARENT; ++i)
	{
		pthread_create(&parentThreads[i], NULL, (void *)Parent, NULL);
	}




}