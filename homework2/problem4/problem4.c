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
	printf("Teacher %u Entered \n", (int) pthread_self());
	//sem_wait(&numberOfTeacher);
}
void child_enter(){
	printf("Child %u Entered \n", (int) pthread_self());
	sem_wait(&numberOfChildren);
}
void parent_enter(){
	printf("Parent %u Entered \n", (int) pthread_self());
	sem_wait(&numberOfParent);
}
void teach(){
	printf("Teacher %u is teaching \n",(int) pthread_self());
	sleep(2);
}
void learn(){
	printf("Child %u is learning \n", (int) pthread_self());
	sleep(2);

}
void verify_compliance(){
	printf("Parent %u checking compliance \n", (int) pthread_self());

	sem_getvalue(&numberOfChildren, &currentChildrenValue);
	sem_getvalue(&numberOfTeacher, &currentTeacherValue);

	currentTeachers=NUM_TEACHER-currentTeacherValue;
	currentChildren=NUM_CHILD-currentChildrenValue;

	printf("number of children: %d \n", currentChildren);
	printf("number of teachers: %d \n", currentTeachers);

	if(ceil((double)currentChildren / (double)R) <= currentTeachers)
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
	
	printf("Teacher %u trying to leave \n", (int) pthread_self());

	sem_getvalue(&numberOfChildren, &currentChildrenValue);
	sem_getvalue(&numberOfTeacher, &currentTeacherValue);

	currentTeachers=NUM_TEACHER-currentTeacherValue;
	currentChildren=NUM_CHILD-currentChildrenValue;

	printf("number of children: %d \n", currentChildren);
	printf("number of teachers: %d \n", currentTeachers);

	if(ceil((double)currentChildren / (double)R) < currentTeachers)
	{
		printf("Teacher %u is allowed to leave\n", (int) pthread_self());
		sem_post(&numberOfTeacher);
		return true;
	}

	else
	{
		printf("Teacher %u goes back to classroom \n", (int) pthread_self());
		return false;
	}

}
void child_exit(){
	printf("Child %u is leaving \n",(int) pthread_self());
	sem_post(&numberOfChildren);
}
void parent_exit(){
	printf("parent %u is exiting\n", (int) pthread_self());
	sem_post(&numberOfParent);
}


void Teacher() {
	printf("teacher %u created\n", (int) pthread_self());
	sem_wait(&numberOfTeacher);
	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);

		currentTeachers=NUM_TEACHER-currentTeacherValue;
		currentChildren=NUM_CHILD-currentChildrenValue;


		teacher_enter();
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);

		currentTeachers=NUM_TEACHER-currentTeacherValue;
		currentChildren=NUM_CHILD-currentChildrenValue;
		
		printf("number of children from teacher func: %d \n", currentChildren);
		printf("number of teachers from teacher func: %d \n", currentTeachers);

		//critical section
		teach();

		if (teacher_exit())
		{	printf("teacher %u goes home\n", (int) pthread_self());
			break;
		}

		else 
			continue;
	}
}

void Child(){
printf("child %u created\n", (int) pthread_self());
	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);

		currentTeachers=NUM_TEACHER-currentTeacherValue;
		currentChildren=NUM_CHILD-currentChildrenValue;
		//printf("before entering %d\n",currentChildren );


		child_enter();
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);

		currentTeachers=NUM_TEACHER-currentTeacherValue;
		currentChildren=NUM_CHILD-currentChildrenValue;
		printf("number of children from children func: %d \n", currentChildren);
		printf("number of teachers from children func: %d \n", currentTeachers);
		//critical section
		learn();
		child_exit();
		printf("child %u goes home\n", (int) pthread_self());
		break;
	}
}

void Parent(){
printf("parent %u created\n", (int) pthread_self());
	for (;;)
	{
		sem_getvalue(&numberOfChildren, &currentChildrenValue);
		sem_getvalue(&numberOfTeacher, &currentTeacherValue);
		sem_getvalue(&numberOfParent, &currentParentValue);

		parent_enter();
		//critical seciton
		verify_compliance();
		parent_exit();
		printf("parent %u goes home\n", (int) pthread_self());
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

	for (int i = 0; i < NUM_TEACHER; ++i)
	{
		pthread_create(&teacherThreads[i], NULL, (void *)Teacher, NULL);
		
	}
sleep(1);
	for (int i = 0; i < NUM_CHILD; ++i)
	{
		
		pthread_create(&childThreads[i], NULL, (void *)Child, NULL);
		
	}
		
sleep(1);
	for (int i = 0; i < NUM_PARENT; ++i)
	{
		pthread_create(&parentThreads[i], NULL, (void *)Parent, NULL);
		
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