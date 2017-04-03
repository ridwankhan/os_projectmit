#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <stdbool.h>

sem_t numberOfTeacher;
sem_t numberOfChildren;
sem_t numberOfParent;

int currentTeacherValue;
int currentChildrenValue;
int currentParentValue;
int R = 3;
int currentTeachers;
int currentChildren;
int NUM_CHILD = 8;
int NUM_TEACHER = 3;
int NUM_PARENT = 3;

void teacher_enter()
{
	printf("Teacher Entered \n");
	sem_wait(&numberOfTeacher);
}
void child_enter(){
	printf("Child Entered \n");
	sem_wait(&numberOfChildren);
}
void parent_enter(){
	printf("Parent Entered \n");
	sem_wait(&numberOfParent);
}
void teach(){
	printf("Teacher is teaching \n");
	sleep(1);
}
void learn(){
	printf("Child is learning \n");
	sleep(1);

}
void verify_compliance(){
	printf("Parent checking compliance \n");

	sem_getvalue(&numberOfChildren, &currentChildrenValue);
	sem_getvalue(&numberOfTeacher, &currentTeacherValue);

	currentTeachers=NUM_TEACHER-currentTeacherValue;
	currentChildren=NUM_CHILD-currentChildrenValue;
	printf("number of children: %d \n", currentChildren);
	printf("number of teachers: %d \n", currentTeachers);
	if(ceil((double)(currentChildren / R)) <= currentTeachers)
	{
		printf("Regulation is complied with\n");
	}

	else
	{
		printf("Regulation NOT COMPLIED WITH \n");
	}

}
bool teacher_exit()
{
	
	printf("Teacher trying to leave \n");
	sem_getvalue(&numberOfChildren, &currentChildrenValue);
	sem_getvalue(&numberOfTeacher, &currentTeacherValue);
	currentTeachers=NUM_TEACHER-currentTeacherValue;
	currentChildren=NUM_CHILD-currentChildrenValue;
	printf("number of children: %d \n", currentChildren);
	printf("number of teachers: %d \n", currentTeachers);
	if(ceil((double)(currentChildren / R)) < currentTeachers)
	{
		printf("Teacher is allowed to leave\n");
		sem_post(&numberOfTeacher);
		return true;
	}

	else
	{
		printf("Teacher goes back to classroom \n");
		return false;
	}

}
void child_exit(){
	printf("Child is leaving \n");
	sem_post(&numberOfChildren);
}
void parent_exit(){
	printf("parent is exiting\n");
	sem_post(&numberOfParent);
}


void Teacher() {
	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);
		currentTeachers=NUM_TEACHER-currentTeacherValue;
		currentChildren=NUM_CHILD-currentChildrenValue;
		printf("number of children: %d \n", currentChildren);
		printf("number of teachers: %d \n", currentTeachers);
		teacher_enter();
		//critical section
		teach();

		if (teacher_exit())
		{	printf("teacher goes home\n");
			break;
		}
		else 
			continue;
	}
}

void Child(){
	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);

		child_enter();
		//critical section
		learn();
		child_exit();
		printf("child goes home\n");
		break;
	}
}

void Parent(){

	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);
		sem_getvalue(&numberOfParent, &currentParentValue);
		parent_enter();
		//critical seciton
		verify_compliance();
		parent_exit();
		printf("parent goes home\n");
		break;
	}
}

int main(int argc, char *argv[]){

	void * status;

	pthread_t childThreads[NUM_CHILD];
	pthread_t teacherThreads[NUM_TEACHER];
	pthread_t parentThreads[NUM_PARENT];

	sem_init(&numberOfTeacher,0,NUM_TEACHER);
	sem_init(&numberOfChildren,0,NUM_CHILD);
	sem_init(&numberOfParent,0,NUM_PARENT);

	for (int i = 0; i < NUM_CHILD; ++i)
	{
		
		pthread_create(&childThreads[i], NULL, (void *)Child, NULL);
		printf("child created\n");
	}
	
	for (int i = 0; i < NUM_TEACHER; ++i)
	{
		pthread_create(&teacherThreads[i], NULL, (void *)Teacher, NULL);
		printf("teacher created\n");
	}

	for (int i = 0; i < NUM_PARENT; ++i)
	{
		pthread_create(&parentThreads[i], NULL, (void *)Parent, NULL);
		printf("parent created\n");
	}

		
	for (int i = 0; i < NUM_TEACHER; ++i)
	{
		int rc = pthread_join(teacherThreads[i], &status);

	}

	for (int i = 0; i < NUM_PARENT; ++i)
	{
		int rc = pthread_join(parentThreads[i], &status);

	}

	for (int i = 0; i < NUM_CHILD; ++i)
	{
		int rc = pthread_join(childThreads[i], &status);

	}
	
	return 0;
}