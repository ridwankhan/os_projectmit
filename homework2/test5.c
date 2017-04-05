#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int pg[2];
void Fn(int i, int j)
{
 while(1){
    int c;
 }
}

void handler(int signum)
{ exit(6); Fn(0, -1); }

int main(void)
{
    int i, j, n;
    char c[10];
    pid_t pid[4];
    /*

    for (int x = 0; x < 10; x++)
    {
        printf("%d, ", c[x]);
    }
    */
    signal(SIGUSR1, handler);

    pipe(pg);
    for (i = 0; i < 4; i++)
    {
        //printf("[%d]\n", getpid());
        pid[i] = fork();
        if (pid[i] == 0)
        {
            n = read(pg[0], c, i + 2);
            printf("i %d n value %d\n",i,n );
            //printf("\t[pid = %d:%d] [n = %d] [i = %d] [c[0] = %d] [c[1] = %d]\n", getpid(), getppid(), n, i, c[0], c[1]);
            /* A */
            for (j = 0; j < n; j++)
            {
                //printf("j = %d, ", j);
                pid[i] = fork();
                if (pid[i] == 0)
                { Fn(getpid(), j); }
            }
            //printf("\n");
            for (j = 0; j < i; j++)
            { wait(NULL); }
            Fn(i, 12);
            exit(2);
        }
    }

    kill(pid[i - 2], SIGUSR1);
    for (i = 0; i < 4; i++)
    {
        wait(&n);
        printf("Exit status = %d, c[2] = %d\n", WEXITSTATUS(n), c[2]);
        write(pg[1], c, WEXITSTATUS(n));
    }
    Fn(i, -1);
   //sleep(10000);
   return 0;

}

