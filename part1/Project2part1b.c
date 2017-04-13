

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
#define SIGNALTOKILLCHILD SIGRTMIN +4
//int noinfotosend =0;
int childloop=6;
int child[6];
void siguserhandler(int signum, siginfo_t *info,void *ptr ){
    printf("In siguserhandler\n");
    printf("Signal origninates from process %lu\n",(unsigned long) info->si_pid );
    printf("I am %d killing %d\n",getpid(), info->si_pid);

    int childtokill = info->si_pid;
    printf("----to kill%d\n",childtokill );
    union sigval infotopass;

    if(getpid() == child[0]){
        for(int i =2; i<4; i++)
        {
            printf("child[2] value is %d ",child[i]);
            printf("everyone tried killed me\n");
            infotopass.sival_int = childtokill;
            if(sigqueue(child[i],SIGNALTOKILLCHILD,infotopass)==-1){

                printf("failed in siguser handeler\n");
            }
            else {
                printf("pass\n");
            }

        }
        kill(child[2],SIGUSR2);
        kill(child[3],SIGUSR2);
    }

    if(getpid() == child[1]){
        kill(child[4],SIGUSR2);
        kill(child[5],SIGUSR2);
    }
    /*

    if (kill(info->si_pid,SIGTERM) ==0)
    {
        printf("kill succesful again\n");
    }
    */

    //childloop=childloop-3;
    union sigval child2novalue;
    //kill(getpid(), SIGNOINFO);
    //child2novalue.sival_int = -2147483647;
    //sigqueue(getpid(), SIGNOINFO,child2novalue);
}
void siguser2handler(int signum,siginfo_t *info, void *ptr)
{
        siginfo_t info3;
        sigset_t maskmax;
        sigemptyset(&maskmax);
        sigaddset(&maskmax,SIGNALTOKILLCHILD);

        sigprocmask(SIG_BLOCK,&maskmax,NULL);

        sigwaitinfo(&maskmax,&info3);

        kill(info3.si_value.sival_int,SIGTERM);

}
void handler(int signum, siginfo_t *info, void *ptr) {
    printf("In handler\n");   
    printf("pid: %d,parentpid: %d\n",getpid(), getppid() );
    //printf("updating noinfotosend to 1\n");
    //noinfotosend = 1; // updating the flag

    if(kill(getppid(),SIGUSR1) == 0)
    {
        printf("kill succesful \n");
    }
    //kill(getppid(), SIGNOINFO);
    union sigval childnovalue;

    sigqueue(getppid(),SIGNOINFO,childnovalue);
    while(1)
        //printf("in while loop\n");
        ;
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
        printf("array from index: %d to index: %d\n", beginning,gap);
        maxd = max(array,beginning,gap);
        mind = min(array,beginning,gap);
        sumd = sum(array,beginning,gap);
        int status;
        
        
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
        int sumindex =0;
        ///
        int defaultChild0Max;
        int defaultChild0Min;
        int defaultChild0Sum;

        int child0bothblocked=0;
        for(int i =0; i<childloop; i++)
        {
            printf("child 0 waiting for %dth signal\n",i + 1);
            sigwaitinfo(&maskmax0,&infomax0);
            int signum = infomax0.si_signo;
           // printf("signal number is %d\n  ", signum);
            //getting values from the ch
            if(signum == SIGMAX){
                if(maxindex == 0)
                {
                    defaultChild0Max = infomax0.si_value.sival_int;
                }
                if(maxindex == 1)
                {
                    if(infomax0.si_value.sival_int > defaultChild0Max)
                    {
                        defaultChild0Max = infomax0.si_value.sival_int;
                    }
                }
                maxindex++;
                printf("signal %d for child 0 recieved is max from pid: %d \n", i+1,infomax0.si_pid);
                //memset(&infomax0,0,sizeof(infomax0));
            }
            else if(signum == SIGMIN){
                if (minindex == 0)
                {
                    defaultChild0Min = infomax0.si_value.sival_int;   
                }
                if (minindex == 1)
                {
                    if (infomax0.si_value.sival_int < defaultChild0Max)
                    {
                        defaultChild0Min = infomax0.si_value.sival_int;
                    }
                }
                minindex++;
                printf("signal %d for child 0 recieved is min from pid: %d\n", i+1,infomax0.si_pid);
                //memset(&infomax0,0,sizeof(infomax0));
            }
            else if(signum == SIGSUM){
                if (sumindex == 0)
                {
                    defaultChild0Sum = infomax0.si_value.sival_int;   
                }
                if (sumindex == 1)
                {
                    defaultChild0Sum = defaultChild0Sum + infomax0.si_value.sival_int;
                }
                sumindex++;
                printf("signal %d for child 0 recieved is sum from pid: %d\n", i+1,infomax0.si_pid);
                memset(&infomax0,0,sizeof(infomax0));
                
            }
            else if(signum == SIGNOINFO){
                printf("signal %d for child 0 recieved is SIGNOINFO from pid: %d \n", i+1,infomax0.si_pid);
                //memset(&infomax0,0,sizeof(infomax0));
                child0bothblocked++;
                i=i+2;
            }

        }  
    
            printf("printing childrenmaxs recieved by child 0: %d \n", defaultChild0Max);
            printf("printing childrenmins recieved by child 0: %d \n", defaultChild0Min);
            printf("printing childrensums recieved by child 0: %d \n", defaultChild0Sum);
       

        union sigval child0max;
        child0max.sival_int= maxd;

        union sigval child0min;
        child0min.sival_int= mind;

        union sigval child0sum;
        child0sum.sival_int=sumd;

        if (child0bothblocked!=2)
        {

            if(defaultChild0Max>maxd)
            {
                child0max.sival_int = defaultChild0Max;
            }

            if(defaultChild0Min<mind)
            {
                child0min.sival_int = defaultChild0Min;
            }
            
                child0sum.sival_int = defaultChild0Sum + sumd;
        }

        printf("child 0 sending %d, %d, %d \n",child0max.sival_int,child0min.sival_int, child0sum.sival_int);
        sigqueue(getppid(),SIGMAX,child0max);
        sigqueue(getppid(),SIGMIN,child0min);
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
        printf("array from index: %d to index: %d\n", gap+1,2*gap);
        maxd = max(array,gap+1,2*gap);
        mind = min(array,gap+1,2*gap);
        sumd = sum(array,gap+1,2*gap);
        int status;

        siginfo_t infomax1;
        sigset_t maskmax1;

        sigemptyset(&maskmax1);
        sigaddset(&maskmax1,SIGMAX);
        sigaddset(&maskmax1,SIGMIN);
        sigaddset(&maskmax1,SIGSUM);
        sigaddset(&maskmax1,SIGNOINFO);

        sigprocmask(SIG_BLOCK,&maskmax1,NULL);

        int minindex =0;
        int maxindex =0;
        int sumindex =0;
        ///
        int defaultChild1Max;
        int defaultChild1Min;
        int defaultChild1Sum;

        int child1bothblocked=0;
        for(int i =0; i<childloop; i++)
        {

            sigwaitinfo(&maskmax1,&infomax1);
            int signum = infomax1.si_signo;

           if(signum == SIGMAX){
                if(maxindex == 0)
                {
                    defaultChild1Max = infomax1.si_value.sival_int;
                }
                if(maxindex == 1)
                {
                    if(infomax1.si_value.sival_int > defaultChild1Max)
                    {
                        defaultChild1Max = infomax1.si_value.sival_int;
                    }
                }
                maxindex++;
                printf("signal %d for child 1 recieved is max from pid: %d \n", i+1,infomax1.si_pid);
                //memset(&infomax0,0,sizeof(infomax0));
            }
            else if(signum == SIGMIN){
                if (minindex == 0)
                {
                    defaultChild1Min = infomax1.si_value.sival_int;   
                }
                if (minindex == 1)
                {
                    if (infomax1.si_value.sival_int < defaultChild1Max)
                    {
                        defaultChild1Min = infomax1.si_value.sival_int;
                    }
                }
                minindex++;
                printf("signal %d for child 1 recieved is min from pid: %d\n", i+1,infomax1.si_pid);
                //memset(&infomax1,0,sizeof(infomax1));
            }
            else if(signum == SIGSUM){
                if (sumindex == 0)
                {
                    defaultChild1Sum = infomax1.si_value.sival_int;   
                }
                if (sumindex == 1)
                {
                    defaultChild1Sum = defaultChild1Sum + infomax1.si_value.sival_int;
                }
                sumindex++;
                printf("signal %d for child 1 recieved is sum from pid: %d\n", i+1,infomax1.si_pid);
                memset(&infomax1,0,sizeof(infomax1));
                
            }
            else if(signum == SIGNOINFO){
                printf("signal %d  for child 1 recieved is SIGNOINFO from pid: %d \n", i+1,infomax1.si_pid);
                //memset(&infomax1,0,sizeof(infomax1));
                child1bothblocked++;
                i=i+3;
            }
        }

            printf("printing childrenmaxs recieved by child 1: %d \n", defaultChild1Max);
            printf("printing childrenmins recieved by child 1: %d \n", defaultChild1Min);
            printf("printing childrensums recieved by child 1: %d \n", defaultChild1Sum);

        union sigval child1max;
        child1max.sival_int= maxd;

        union sigval child1min;
        child1min.sival_int= mind;

        union sigval child1sum;
        child1sum.sival_int=sumd;

        if (child1bothblocked!=2)
        {

            if(defaultChild1Max>maxd)
            {
                child1max.sival_int = defaultChild1Max;
            }

            if(defaultChild1Min<mind)
            {
                child1min.sival_int = defaultChild1Min;
            }
            
                child1sum.sival_int = defaultChild1Sum + sumd;
        }

        printf("child 1 sending %d, %d, %d \n",child1max.sival_int,child1min.sival_int, child1sum.sival_int);
        sigqueue(getppid(),SIGMAX,child1max);
        sigqueue(getppid(),SIGMIN,child1min);
        sigqueue(getppid(),SIGSUM,child1sum);

        wait(&status);
        wait(&status);

        exit(0);

    }
    //child 2
    else if (getpid()==child[2])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 2\n");
        printf("array from index: %d to index: %d\n", 2*gap+1,3*gap);
        alarm(3); // this will send itself the SIGALRM signal after 3 seconds 
        printf("Sleeping for 3 seconds\n");
        sleep(4);

        maxd = max(array,2*gap+1, 3*gap);
        mind = min(array,2*gap+1, 3*gap);
        sumd = sum(array,2*gap+1, 3*gap);
       
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

        union sigval child2sum;
        child2sum.sival_int= sumd;
        
        //sleep(3);
        if(sigqueue(getppid(),SIGSUM,child2sum) == -1)
        {
            printf("fail in child 2 sum \n");
        }
        else{
            printf("success in child 2 sum\n");
        }

        exit(0);
    }
    //child 3
    else if (getpid()==child[3])
    {
       
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        //printf("child 3\n");

        //printf("array from index: %d to index: %d\n", 3*gap+1,4*gap);
        alarm(3); // this will send itself the SIGALRM signal after 3 seconds 
        //printf("Sleeping for 3 seconds\n");
        //sleep(4);

        maxd = max(array,3*gap+1,4*gap);
        mind = min(array,3*gap+1,4*gap);
        sumd = sum(array,3*gap+1,4*gap);
        
        union sigval child3max;
        child3max.sival_int= maxd;

        if(sigqueue(getppid(),SIGMAX,child3max) == -1)
        {
            printf("fail in child 3 max \n");
        }
        else
        {
            printf("success in child 3 max \n");
        }
        //sending min signal 
        union sigval child3min;
        child3min.sival_int= mind;

        if(sigqueue(getppid(),SIGMIN,child3min) == -1)
        {
            printf("fail in child 3 min \n");
        }
        else{
            printf("success in child 3 min\n");
        }

        union sigval child3sum;
        child3sum.sival_int= sumd;

        printf("child3sum is %d \n", sumd);
        
        //sleep(3);
        if(sigqueue(getppid(),SIGSUM,child3sum) == -1)
        {
            printf("fail in child 3 sum \n");
        }
        else{
            printf("success in child 3 sum\n");
        }

        exit(0);

    }
    //child 4
    else if (getpid()==child[4])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 4\n");
        printf("array from index: %d to index: %d\n", 4*gap+1,5*gap);
        alarm(3); // this will send itself the SIGALRM signal after 3 seconds 
        printf("Sleeping for 3 seconds\n");
        //sleep(4);
        maxd = max(array,4*gap+1,5*gap);
        mind = min(array,4*gap+1,5*gap);
        sumd = sum(array,4*gap+1,5*gap);

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
        printf("array from index: %d to index: %d\n", 5*gap+1,end);
        alarm(3); // this will send itself the SIGALRM signal after 3 seconds 
        //printf("Sleeping for 3 seconds\n");
        //sleep(4);
        maxd = max(array,5*gap+1,end);
        mind = min(array,5*gap+1,end);
        sumd = sum(array,5*gap+1,end);

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

    struct sigaction act2;
    memset(&act2, 0, sizeof(act2));

    act2.sa_sigaction = siguser2handler;
    act2.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR2, &act2, NULL);


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

