

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
//#include <sys/siginfo.h>

#define SIGMAX SIGRTMIN+1
#define SIGMIN SIGRTMIN+2
#define SIGSUM SIGRTMIN+3
#define SIGNOINFO SIGRTMIN
int noinfotosend =0;


void siguserhandler(int signum, siginfo_t *info,void *ptr ){
    printf("In siguserhandler\n");
    printf("Signal origninates from process %lu\n",(unsigned long) info->si_pid );
    printf("I am %d killing %d\n",getpid(), info->si_pid);
    if (kill(info->si_pid,SIGTERM) ==0)
    {
        printf("kill succesful again\n");
    }
}

void handler(int signum, siginfo_t *info, void *ptr) {
    printf("In handler\n");   
    printf("pid: %d,parentpid: %d\n",getpid(), getppid() );
    printf("updating noinfotosend to 1\n");
    noinfotosend = 1; // updating the flag

    if(kill(getppid(),SIGUSR1) == 0)
    {
        printf("kill succesful \n");
    }
}

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

void printarray(int *array, int arraysize)
{
    printf("[ ");
    for (int i =0; i<arraysize;i++)
    {
        printf(" %d ",array[i]);
    }
    printf(" ] \n");
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
        int childrensums =0;
        // wait for signal
        
        siginfo_t infomax0;
        sigset_t maskmax0;
        sigemptyset(&maskmax0);
        sigaddset(&maskmax0,SIGMAX);
        sigaddset(&maskmax0,SIGMIN);
        sigaddset(&maskmax0,SIGSUM);
        sigaddset(&maskmax0,SIGNOINFO);

        sigprocmask(SIG_BLOCK,&maskmax0,NULL);

        int minindex =0;
        int maxindex =0;
        ///
        
        for(int i =0; i<6; i++)
        {
            printf("child 0 waiting for %dth signal\n",i + 1);
            sigwaitinfo(&maskmax0,&infomax0);
            int signum = infomax0.si_signo;
           // printf("signal number is %d\n  ", signum);
            //getting values from the ch
            if(signum == SIGMAX){
                childrenmaxs[maxindex] = infomax0.si_value.sival_int;
                maxindex++;
                printf("signal %d recieved is max\n", i+1);
            }
            else if(signum == SIGMIN){
                childrenmins[minindex] = infomax0.si_value.sival_int;
                minindex++;
                printf("signal %d recieved is min\n", i+1);
            }
            else if(signum == SIGSUM){
                childrensums = infomax0.si_value.sival_int + childrensums;
                printf("signal %d recieved is sum\n", i+1);
                
            }
            else if(signum == SIGNOINFO){
                printf("signal %d recieved is SIGNOINFO\n", i+1);
                continue;
            }

        }   
            printf("printing childrenmaxs recieved by child 0: ");
            printarray(childrenmaxs,2);
            printf("printing childrenmins recieved by child 0: ");
            printarray(childrenmins,2);
            printf("printing childrensums recieved by child 0: %d \n", childrensums);
            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];
            int actualsum = childrensums + sumd;
            //printf("sum value of child 0 itself %d \n", sumd);

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
        
        union sigval child0sum;
        child0sum.sival_int= actualsum;

       

        
        sigqueue(getppid(),SIGSUM,child0sum);


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
        int childrensums =0;
        // wait for signal
        siginfo_t infomax1;
        sigset_t maskmax1;

        sigemptyset(&maskmax1);
        sigaddset(&maskmax1,SIGMAX);
        sigaddset(&maskmax1,SIGMIN);
        sigaddset(&maskmax1,SIGSUM);
        sigaddset(&maskmax1,SIGNOINFO);

        sigprocmask(SIG_BLOCK,&maskmax1,NULL);

        int minindex = 0;
        int maxindex =0;
        
        for(int i =0; i<6; i++)
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
            else if(signum == SIGSUM){
                childrensums = childrensums + infomax1.si_value.sival_int;

            }
            else if(signum == SIGNOINFO){
                continue;

            }

        }

            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];
            int actualsum = childrensums + sumd;
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
        
        //sending sum signal
        union sigval child1sum;
        child1sum.sival_int= actualsum;


        sigqueue(getppid(),SIGSUM,child1sum);

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
        
        alarm(3); // this will send itself the SIGALRM signal after 3 seconds 
        printf("Sleeping for 3 seconds\n");
        sleep(3);

        maxd = max(array,2*gap+1, 3*gap);
        mind = min(array,2*gap+1, 3*gap);
        sumd = sum(array,2*gap+1, 3*gap);
       
        //printf("Child 2 start: %d, end: %d , max value: %d, min value: %d \n",2*gap+1, 3*gap,maxd, mind);
        union sigval child2max;

        child2max.sival_int= maxd;
        
        // flag to check if you should send signal or not 
        printf("noinfotosend: %d \n",noinfotosend);
        if(noinfotosend == 0)
    {

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

        union sigval child2sum;
        child2sum.sival_int= sumd;

        printf("child2sum is %d \n", sumd);
        
        //sleep(3);
        if(sigqueue(getppid(),SIGSUM,child2sum) == -1)
        {
            printf("fail in child 2 sum \n");
        }
        else{
            printf("success in child 2 sum\n");
        }
    }
    // if flag is set to one, the process is going to be killed and should not send any info 

    else if(noinfotosend == 1){
        for (int i=0;i<3;i++)
        {
            union sigval child2novalue;
            child2novalue.sival_int = 0;
            if(sigqueue(getppid(), SIGNOINFO,child2novalue) == -1)
            {
                printf("Fail to send noinfotosend signal from child 2\n");
            }
            else
            {
            printf("noinfotosend was sent from child 2\n");
            }
        }
        return(0);
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

        //printf("child3sum is %d \n", sumd);
        if(sigqueue(getppid(),SIGMIN,child3min) == -1)
        {
            printf("fail in child 3 min \n");
        }

        else{
            printf("success in child 3 min \n");
        }

        //sending sum signal

        union sigval child3sum;
        child3sum.sival_int= sumd;

        if(sigqueue(getppid(),SIGSUM,child3sum) == -1)
        {
            printf("fail in child 3 sum \n");
        }

        else{
            printf("success in child 3 sum \n");
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
        
        union sigval child4sum;
        child4sum.sival_int= sumd;

        if(sigqueue(getppid(),SIGSUM,child4sum) == -1)
        {
            printf("failed in child 4 sum \n");
        }
        else 
        {
            printf("success in child 4 sum \n");
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

        union sigval child5sum;
        child5sum.sival_int= sumd;

        if(sigqueue(getppid(),SIGSUM,child5sum) == -1)
        {
            printf("fail in child 5 sum \n");
        }
        else
        {
            printf("success in child 5 sum \n");
        }
        exit(0);
    }
    //Parent
    else
    {	

 		int childrenmaxs[2];
        int childrenmins[2];
        int childrensums =0;

        // wait for signal
        siginfo_t infomaxmain;
        sigset_t maskmaxmain;

        sigemptyset(&maskmaxmain);
        sigaddset(&maskmaxmain,SIGMAX);
        sigaddset(&maskmaxmain,SIGMIN);
        sigaddset(&maskmaxmain,SIGSUM);

        sigprocmask(SIG_BLOCK,&maskmaxmain,NULL);

        int minindex =0;
        int maxindex =0;


        
        for(int i =0; i<6; i++)
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
            else if(signum == SIGSUM){
                childrensums = childrensums + infomaxmain.si_value.sival_int;
            }

        }
        
            int actualmax = childrenmaxs[0];
            int actualmin = childrenmins[0];
            int actualsum = childrensums;

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
            printf("Actual sum value is %d \n",actualsum);

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
//signal(SIGALRM, handler);
    struct sigaction act;
    memset(&act, 0, sizeof(act));

    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGALRM,&act,NULL);

     struct sigaction act1;
    memset(&act1, 0, sizeof(act1));

    act1.sa_sigaction = siguserhandler;
    act1.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act1, NULL);


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

