#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#define MEM 64000
ucontext_t T1, T2,Main;
ucontext_t a;
int fn1()
{
 printf("this is from 1\n");
 setcontext(&Main);
}
void fn2()
{
 printf("this is from 2\n");
  setcontext( &a);
 printf("finished 1\n");
}
void start()
{
 getcontext(&a);
 a.uc_link=0;
 a.uc_stack.ss_sp=malloc(MEM);
 a.uc_stack.ss_size=MEM;
 a.uc_stack.ss_flags=0;
 printf("making a\n");
 makecontext(&a, (void*)&fn1, 0);
}

int main(int argc, char *argv[])
{
 start();
 getcontext(&Main);
 getcontext(&T1);
 T1.uc_link=0;
 T1.uc_stack.ss_sp=malloc(MEM);
 T1.uc_stack.ss_size=MEM;
 T1.uc_stack.ss_flags=0;
 printf("making T1\n");
 makecontext(&T1, (void*)&fn1, 0);
 printf("swappping main and t1\n");
 swapcontext(&Main, &T1);
 printf("getting T2\n");
 getcontext(&T2);
 T2.uc_link=0;
 T2.uc_stack.ss_sp=malloc(MEM);
 T2.uc_stack.ss_size=MEM;
 T2.uc_stack.ss_flags=0;
 printf("making T2\n");
 makecontext(&T2, (void*)&fn2, 0);
 printf("swappping main and t2\n");
 swapcontext(&Main, &T2);
 printf("completed\n");
 exit(0);
}