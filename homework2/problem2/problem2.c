
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int res = 0, num = 2, pd[2], arg[32];
pthread_t thread[32];
sem_t sem;
char c[10];
//printf("Final result2: %c \n", *c[1]);

void h(int s)
{
	read(pd[0], &num, sizeof(num));
	sem_post(&sem);
}

void *f(void *var)
{
	int tmp = *(int *)var;
	res += tmp;
	pthread_exit((void *)0);
}

int main()
{
	int i, k, p, pstatus;
	void *tstatus;

	sem_init(&sem, 0, 0);
	signal(SIGUSR1, h);
	pipe(pd);

	for(i = 0; i < num; i++) {
		p = fork();

		if(p == 0) {
			sem_wait(&sem);
			num++;
			for(k = 0; k < num; k++)
			{
				arg[k] = k;
				pthread_create(&thread[k], NULL, f, (void *)&arg[k]);
			}

			for(k = 0; k < num; k++) pthread_join(thread[k], &tstatus);
			if(num < 4) write(pd[1], &num, sizeof(num));
			printf("res before exit %d\n",res );
			exit(res);
		}
	}

	write(pd[1], &num, sizeof(num));
	signal(SIGUSR1, SIG_IGN); 
	kill(0, SIGUSR1);

	for(i = 0; i < num; i++){
		wait(&pstatus);
		printf("res %d\n",res);
		printf("Pstatus %d\n",pstatus );
		printf("WIFEXITED %d\n", WIFEXITED(pstatus));
		printf("WEXITSTATUS %d\n",WEXITSTATUS(pstatus) );
		if(WIFEXITED(pstatus)) res += WEXITSTATUS(pstatus);
	}

	

	printf("Final result1: %d \n", res);
	read(pd[0], &num, sizeof(num));
	printf("Final result2: %d \n", num);
	

	return 0;  
}
