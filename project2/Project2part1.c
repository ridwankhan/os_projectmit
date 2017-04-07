#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>

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
    int maxarray[2];
    int minarray[2];
    int sumarray[2];
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
               {pid=fork();
                 if (pid==0)
                {
                 child[5]=getpid();
                }    

                }
          }
      }

        //child 1
     if (getpid()==child[0])
    { 
        int max1,min1,sum1;
        int child1max,child1min,child1sum;
        int child1totalmax, child1totalmin, child1totalsum;

        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 1\n");
        maxd = max(array,beginning,gap);
        mind = min(array,beginning,gap);
        sumd = sum(array,beginning,gap);
        int status;
        wait(&status);
        wait(&status);
        exit(0);

    }
    //child 2
    else if(getpid()==child[1])
    {
        int max2,min2,sum2;
        int child2max,child2min,child2sum;
        int child2totalmax, child2totalmin, child2totalsum;
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 2\n");
        maxd = max(array,gap+1,2*gap);
        mind = min(array,gap+1,2*gap);
        sumd = sum(array,gap+1,2*gap);
        int status;
       wait(&status);
       wait(&status);
        exit(0);
    }
    //child 3
    else if (getpid()==child[2])
    {
        int max3,min3,sum3;
        int child3max,child3min,child3sum;
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
        printf("child 3\n");
        maxd = max(array,2*gap+1, 3*gap);
        mind = min(array,2*gap+1, 3*gap);
        sumd = sum(array,2*gap+1, 3*gap);
       
        exit(0);
    }
    //child 4
    else if (getpid()==child[3])
    {
        int max4,min4,sum4;
        int child4max,child4min,child4sum;
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
         printf("child 4\n");
        maxd = max(array,3*gap+1,4*gap);
        mind = min(array,3*gap+1,4*gap);
        sumd = sum(array,3*gap+1,4*gap);
       
        exit(0);
    }
    //child 5
    else if (getpid()==child[4])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
         printf("child 5\n");
        maxd = max(array,4*gap+1,5*gap);
        mind = min(array,4*gap+1,5*gap);
        sumd = sum(array,4*gap+1,5*gap);
      
        exit(0);
    }
    //child 6
    else if (getpid()==child[5])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
         printf("child 6\n");
        maxd = max(array,5*gap+1,end);
        mind = min(array,5*gap+1,end);
        sumd = sum(array,5*gap+1,end);
   
        exit(0);
    }
    //Parent
    else
    {
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