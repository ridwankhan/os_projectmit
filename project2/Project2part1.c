

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
//#include <sys/siginfo.h>

#define SIGMAX SIGRTMIN+1
#define SIGMIN SIGRTMIN+2

int max(int array[], int first, int last)
{
    int maximum = array[first];
    for(int i = first;i<=last;i++)
    {
        if(array[i]>maximum)
            maximum = array[i];
    }
    return maximum;
}

int min(int array[], int first, int last)
{
    int minimum = array[first];
    for(int i = first;i<=last;i++)
    {
        if(array[i]<minimum)
            minimum = array[i];
    }
    return minimum;
}

int sum(int array[], int first, int last)
{
    int summation = 0;
    for(int i=first;i<=last;i++)
    {
        summation += array[i];
    }
    return summation;
}

int partd(int array[],int index1, int index2)
{
    int beginning=index1;
    int end=index2;
    int gap= ceil((end-beginning)/6);
    int child[6];
   
    pid_t pid=getpid();
    
    int maxd;
    int mind;
    int sumd;

    printf("parent pid is %d\n",getpid());
    // spawning the children
       pid=fork();
        if (pid==0)
        {
            child[0]=getpid();
            pid=fork();
            if (pid==0)
            {
                child[2]=getpid();
                
            }
            else
            {
             pid=fork();
             if (pid==0)
                {
                child[3]=getpid();
                
                }
            }
        }
        else if (pid>0)
            {
                pid=fork();
                if (pid==0)
                {
                    child[1]=getpid();
                    pid=fork();
                    if (pid==0)
                    {
                        child[4]=getpid();        
                    }
                
                else
                {
                pid=fork();
                 if (pid==0)
                    {
                        child[5]=getpid();
                    }    

                }
          }
      }

        //child 0
     if (getpid()==child[0])
    { 
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 0\n");
        maxd = max(array,beginning,gap);
        mind = min(array,beginning,gap);
        sumd = sum(array,beginning,gap);
        int status;
        
        int childrenmaxs[2];
        int childrenmins[2];
        // wait for signal
        
        siginfo_t infomax0;
        sigset_t maskmax0;
        sigemptyset(&maskmax0);
        sigaddset(&maskmax0,SIGMAX);
        sigaddset(&maskmax0,SIGMIN);

        sigprocmask(SIG_BLOCK,&maskmax0,NULL);

        int minindex = 0;
        int maxindex =0;
        
        for(int i =0; i<4; i++)
        {

            sigwaitinfo(&maskmax0,&infomax0);
            int signum = infomax0.si_signo;
            //getting values from the ch
            if(signum == SIGMAX){
                childrenmaxs[maxindex] = infomax0.si_value.sival_int;
                maxindex++;
                
            }
            else if(signum == SIGMIN){
                childrenmins[minindex] = infomax0.si_value.sival_int;
                minindex++;
            }

        }
        
            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];

            if(actualmax<childrenmaxs[1])
            {
                actualmax = childrenmaxs[1];
            }
            
            if(actualmin>childrenmins[1])
            {
                actualmin = childrenmaxs[1];
            }
            

        union sigval child0max;
        child0max.sival_int= maxd;

        
        if(actualmax>maxd)
        {
            child0max.sival_int = actualmax;
        }

        
        sigqueue(getppid(),SIGMAX,child0max);

        union sigval child0min;
        child0min.sival_int= mind;

        if(actualmin<mind)
        {
            child0min.sival_int = actualmin;
        }

        
        sigqueue(getppid(),SIGMIN,child0min);
        
        wait(&status);
        wait(&status);

        exit(0);

    }
    //child 1
    else if(getpid()==child[1])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 1\n");
        maxd = max(array,gap+1,2*gap);
        mind = min(array,gap+1,2*gap);
        sumd = sum(array,gap+1,2*gap);
        int status;

        int childrenmaxs[2];
        int childrenmins[2];

        // wait for signal
        siginfo_t infomax1;
        sigset_t maskmax1;

        sigemptyset(&maskmax1);
        sigaddset(&maskmax1,SIGMAX);
        sigaddset(&maskmax1,SIGMIN);

        sigprocmask(SIG_BLOCK,&maskmax1,NULL);

        int minindex = 0;
        int maxindex =0;
        
        for(int i =0; i<4; i++)
        {

            sigwaitinfo(&maskmax1,&infomax1);
            int signum = infomax1.si_signo;

            if(signum == SIGMAX){
                childrenmaxs[maxindex] = infomax1.si_value.sival_int;
                maxindex++;

            }
            else if(signum == SIGMIN){
                childrenmins[minindex] = infomax1.si_value.sival_int;
                minindex++;

            }

        }

            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];

            if(actualmax<childrenmaxs[1])
            {
                actualmax = childrenmaxs[1];
            }

            if(actualmin>childrenmins[1])
            {
                actualmin = childrenmaxs[1];
            }
            
            
        union sigval child1max;
        child1max.sival_int= maxd;

        if(actualmax>maxd)
        {
            child1max.sival_int = actualmax;
        }

        sigqueue(getppid(),SIGMAX,child1max);

        //sending min signal 

        union sigval child1min;
        child1min.sival_int= mind;

        if(actualmin<mind)
        {
            child1min.sival_int = actualmin;
        }

        sigqueue(getppid(),SIGMIN,child1min);
        
        //wait for child 4 and 5
       wait(&status);
       wait(&status);
        exit(0);

    }
    //child 2
    else if (getpid()==child[2])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 2\n");

        maxd = max(array,2*gap+1, 3*gap);
        mind = min(array,2*gap+1, 3*gap);
        sumd = sum(array,2*gap+1, 3*gap);
       
        //printf("Child 2 start: %d, end: %d , max value: %d, min value: %d \n",2*gap+1, 3*gap,maxd, mind);
        union sigval child2max;

        child2max.sival_int= maxd;

        if(sigqueue(getppid(),SIGMAX,child2max) == -1)
        {
            printf("fail in child 2 max \n");
        }
        else
        {
            printf("success in child 2 max \n");
        }
        //sending min signal 
        union sigval child2min;
        child2min.sival_int= mind;

        if(sigqueue(getppid(),SIGMIN,child2min) == -1)
        {
            printf("fail in child 2 min \n");
        }
        else{
            printf("success in child 2 min\n");
        }
        exit(0);
    }
    //child 3
    else if (getpid()==child[3])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 3\n");
        maxd = max(array,3*gap+1,4*gap);
        mind = min(array,3*gap+1,4*gap);
        sumd = sum(array,3*gap+1,4*gap);
        
        //printf("Child 3 start: %d, end: %d , max value: %d, min value: %d \n",3*gap+1, 4*gap,maxd, mind);

        union sigval child3max;
        child3max.sival_int= maxd;

        if(sigqueue(getppid(),SIGMAX,child3max) == -1){
            printf("fail in child 3 max \n");
        }
        else{
            printf("success in child 3 min \n");
        }

        //sending min signal

        union sigval child3min;
        child3min.sival_int= mind;

        if(sigqueue(getppid(),SIGMIN,child3min) == -1)
        {
            printf("fail in child 3 min \n");
        }

        else{
            printf("success in child 3 min \n");
        }
        exit(0);
    }
    //child 4
    else if (getpid()==child[4])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 4\n");
        maxd = max(array,4*gap+1,5*gap);
        mind = min(array,4*gap+1,5*gap);
        sumd = sum(array,4*gap+1,5*gap);

        //printf("Child 4 start: %d, end: %d , max value: %d, min value: %d \n",4*gap+1, 5*gap,maxd, mind);

        union sigval child4max;
        child4max.sival_int= maxd;

        if(sigqueue(getppid(),SIGMAX,child4max) == -1)
        {
            printf("failed in child 4 max \n");
        }
        else
        {
            printf("success in child 4 max \n");
        }

        //sending min signal 

        union sigval child4min;
        child4min.sival_int= mind;

        if(sigqueue(getppid(),SIGMIN,child4min) == -1)
        {
            printf("failed in child 4 min \n");
        }
        else 
        {
            printf("success in child 4 min \n");
        }
      
        exit(0);
    }
    //child 5
    else if (getpid()==child[5])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 5\n");
        maxd = max(array,5*gap+1,end);
        mind = min(array,5*gap+1,end);
        sumd = sum(array,5*gap+1,end);

        //printf("Child 5 start: %d, end: %d , max value: %d, min value: %d \n",5*gap+1, end,maxd, mind);
        union sigval child5max;
        child5max.sival_int= maxd;

        if(sigqueue(getppid(),SIGMAX,child5max) == -1)
        {
            printf("fail in child 5 max");
        }
        
        else
        {
            printf("success in child 5 max \n");
        }
        //sending min signal 

        union sigval child5min;
        child5min.sival_int= mind;

        if(sigqueue(getppid(),SIGMIN,child5min) == -1)
        {
            printf("fail in child 5 min \n");
        }
        else
        {
            printf("success in child 5 min \n");
        }
        exit(0);
    }
    //Parent
    else
    {	

 		int childrenmaxs[2];
        int childrenmins[2];

        // wait for signal
        siginfo_t infomaxmain;
        sigset_t maskmaxmain;

        sigemptyset(&maskmaxmain);
        sigaddset(&maskmaxmain,SIGMAX);
        sigaddset(&maskmaxmain,SIGMIN);

        sigprocmask(SIG_BLOCK,&maskmaxmain,NULL);

        int minindex =0;
        int maxindex =0;

        
        for(int i =0; i<4; i++)
        {

            sigwaitinfo(&maskmaxmain,&infomaxmain);
            int signum = infomaxmain.si_signo;
            //getting values from the ch

            if(signum == SIGMAX){
                childrenmaxs[maxindex] = infomaxmain.si_value.sival_int;
                maxindex++;
            }
            else if(signum == SIGMIN){
                childrenmins[minindex] = infomaxmain.si_value.sival_int;
                minindex++;
            }

        }
        
            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];


            if(actualmax<childrenmaxs[1])
            {
                actualmax = childrenmaxs[1];
            }
            if(actualmin>childrenmins[1])
            {
                actualmin = childrenmins[1];
            }
            
            printf("Actual max value is %d \n",actualmax);
            printf("Actual min value is %d \n",actualmin);

    	wait(NULL);
    	wait(NULL);


        for (int child=0; child<5;child++)
        {

            int status;
            pid_t pid=wait(&status);

        } 
     
    exit(0);

    }
}

int main(){
FILE* nums = fopen("numbers.txt", "r");
int readnum = 0;
int counter = 0;

while(fscanf(nums, "%d,", &readnum) > 0 )
{
    counter++;
}
fclose(nums);
int array[counter];

FILE* num = fopen("numbers.txt", "r");
int readnums = 0;
int index = 0;
while(fscanf(num, "%d,", &readnums) > 0 )
{
    array[index] = readnums;
    index++;

}
fclose(num);

partd(array,0,index-1);

return 0;
}

