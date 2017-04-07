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
    // int fd1[2];
    // int fd2[2];
    // int fd3[2];
    // int fd4[2];
    // int fd5[2];
    // int fd6[2];
    pid_t pid=getpid();
    // pipe(fd1);
    // pipe(fd2);
    // pipe(fd3);
    // pipe(fd4);
    // pipe(fd5);
    // pipe(fd6);
    int maxd;
    int mind;
    int sumd;
    int maxarray[2];
    int minarray[2];
    int sumarray[2];
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
        wait(NULL);
        // read(fd1[0],&max1,sizeof(max1));
        // read(fd2[0],&min1,sizeof(min1));
        // read(fd3[0],&sum1,sizeof(sum1));
        if(max1>maxd)
            child1max = max1;
        else 
            child1max = maxd;
        if(min1<mind)
            child1min = min1;
        else 
            child1min = mind;
        child1sum += (sum1 + sumd);
        // read(fd1[0],&maxd,sizeof(maxd));
        // read(fd2[0],&mind,sizeof(mind));
        // read(fd3[0],&sumd,sizeof(sumd));
        if(child1max>maxd)
            child1totalmax = child1max;
        else
            child1totalmax = maxd;
        if(child1min<mind)
            child1totalmin = child1min;
        else
            child1totalmin = mind;
        child1totalsum += child1sum+sumd;
        // write(fd4[1],&child1totalmax,sizeof(child1totalmax));
        // write(fd5[1],&child1totalmin,sizeof(child1totalmin));
        // write(fd6[1],&child1totalsum,sizeof(child1totalsum));

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
        // read(fd1[0],&max2,sizeof(max2));
        // read(fd2[0],&min2,sizeof(min2));
        // read(fd3[0],&sum2,sizeof(sum2));
        if(max2>maxd)
            child2max = max2;
        else 
            child2max = maxd;
        if(min2<mind)
            child2min = min2;
        else 
            child2min = mind;
        child2sum += (sum2 + sumd);
        // read(fd1[0],&maxd,sizeof(maxd));
        // read(fd2[0],&mind,sizeof(mind));
        // read(fd3[0],&sumd,sizeof(sumd));
        if(child2max>maxd)
            child2totalmax = child2max;
        else
            child2totalmax = maxd;
        if(child2min<mind)
            child2totalmin = child2min;
        else
            child2totalmin = mind;
        child2totalsum += child2sum+sumd;
        // write(fd4[1],&child2totalmax,sizeof(child2totalmax));
        // write(fd5[1],&child2totalmin,sizeof(child2totalmin));
        // write(fd6[1],&child2totalsum,sizeof(child2totalsum));
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
        // write(fd1[1],&maxd,sizeof(maxd));
        // write(fd2[1],&mind,sizeof(mind));
        // write(fd3[1],&sumd,sizeof(sumd));
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
        // write(fd1[1],&maxd,sizeof(maxd));
        // write(fd2[1],&mind,sizeof(mind));
        // write(fd3[1],&sumd,sizeof(sumd));
    }
    //child 5
    else if (getpid()==child[4])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
         printf("child 5\n");
        maxd = max(array,4*gap+1,5*gap);
        mind = min(array,4*gap+1,5*gap);
        sumd = sum(array,4*gap+1,5*gap);
        // write(fd1[1],&maxd,sizeof(maxd));
        // write(fd2[1],&mind,sizeof(mind));
        // write(fd3[1],&sumd,sizeof(sumd));
    }
    //child 6
    else if (getpid()==child[5])
    {
        printf("\nHi I'm process %d and my parent is %d\n",getpid(),getppid());
         printf("child 6\n");
        maxd = max(array,5*gap+1,end);
        mind = min(array,5*gap+1,end);
        sumd = sum(array,5*gap+1,end);
        // write(fd1[1],&maxd,sizeof(maxd));
        // write(fd2[1],&mind,sizeof(mind));
        // write(fd3[1],&sumd,sizeof(sumd));
    }
    //Parent
    else
    {

    for (int child=0; child<4;child++)
    {
        int status;
        pid_t pid=wait(&status);
    }   

int max1;
int max2;
int totalmax;
// read(fd4[0],&max1,sizeof(max1));
// read(fd4[0],&max2,sizeof(max2));
maxarray[0] = max1;
maxarray[1] = max2;
if(max1>max2)
totalmax = max1;
else 
totalmax = max2;
printf("Max = %d\n",totalmax);

int min1;
int min2;
int totalmin;
// read(fd5[0],&min1,sizeof(min1));
// read(fd5[0],&min2,sizeof(min2));
minarray[0] = min1;
minarray[1] = min2;
if(min1<min2)
totalmin = min1;
else 
totalmin = min2;
printf("Min = %d\n",totalmin);

int sum1;
int sum2;
int totalsum;
// read(fd6[0],&sum1,sizeof(sum1));
// read(fd6[0],&sum2,sizeof(sum2));
sumarray[0] = sum1;
sumarray[1] = sum2;
totalsum = sum1+sum2;
printf("Sum = %d\n",totalsum);

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