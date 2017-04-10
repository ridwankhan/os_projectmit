//mypthread.c

#include "mypthread.h"

/* threadNode is in the .h file 
typedef struct threadNode{
    
// Type your commands
	int id;
	//int pid;

	ucontext_t ucp;

}Node;
*/


//Global
int threadCount=1;
int numThreadNodes=0;
int excutingThread=1;

#define MEM_STACK 1024*4

void add_node(){
	mypthread_t threadname; //create instance of thread
	temp= (struct threadNode *) malloc(sizeof(struct threadNode)); // initialize temp
	temp->next=NULL; // set next temp
	numThreadNodes++; // increment node count
}
void add_to_queue(mypthread_t* threadname){
	if (head==NULL)
	{
		// first node in list
		add_node();
		head=temp; // set head to temp
		tail=temp; // set tail to temp
	}
	else
	{
		add_node();
		tail->next = temp;
		tail=temp;
		tail->next= head;
	}
	temp->ptr= threadname;
}

mypthread_t* find_thread(int findid)
{
	temp= head; // set temp to head
	do   // search all tids stored in the linked list
	{
		if (temp->ptr->tid == findid) // if you find the tid
		{
			return (temp->ptr);  // return pointer to it
		}
		else  
		{
			temp=temp->next; // iterate to next
		}
	}while (temp!=head); // if you back at head the id doesnt exist
printf("tid %d not found\n",findid);
}

// must pass current id to work
mypthread_t* find_next_active(int currentid)
{
	// 
	temp= head; // set temp to head
	do   // search all tids stored in the linked list
	{
		if (temp->ptr->tid == currentid) // if you find the tid
		{
			break;  // break out of loop
		}
		else  
		{
			temp=temp->next; // iterate to next
		}
	}while (temp!=head); // if you back at head the id doesnt exist


	temp=temp->next; // set temp to next  node
	do  // infinite loop
	{
		if (temp->ptr->state==0) // if next active
		{
			return (temp->ptr); // return next active answer
		}
		else
		{
			temp=temp->next; // increment
		}
	}while (1);
}


// Type your globals...

// Type your own functions (void)
// e.g., free up sets of data structures created in your library

    
//  Write your thread create function here...
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{

	if (numThreadNodes == 0) // first thread being created
	{
		mypthread_t*parentthread= (mypthread_t *)malloc(sizeof(mypthread_t));
		parentthread->tid=threadCount++;
		ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
		parentthread->ucp = context;
		parentthread->ucp->uc_stack.ss_sp = malloc(MEM_STACK);
		parentthread->ucp->uc_stack.ss_size = MEM_STACK;
		parentthread->state = 0;
		add_to_queue(parentthread);
	}
	ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
	thread->ucp = context;
	getcontext(thread->ucp);
	(*thread).ucp->uc_stack.ss_sp = malloc(MEM_STACK);
	(*thread).ucp->uc_stack.ss_size = 4096;
	(*thread).state = 0;
	thread->tid = threadCount++;
	makecontext(thread->ucp, (void(*)()) start_routine, 1, arg);
	add_to_queue(thread);
	return 0;

} 

void mypthread_exit (void *retval){

	mypthread_t* exec_thread = find_thread(excutingThread);
	exec_thread->state = 2;
	free(exec_thread->ucp); //since thread exiting, give memory back
	if (exec_thread->jointid != 0){
		mypthread_t* joining_thread = find_thread(exec_thread->jointid);
		joining_thread->state = 0;
	}
	mypthread_t* upnext =find_next_active(exec_thread->tid);
	if (excutingThread == upnext->tid){
		return;
	}
	excutingThread = upnext->tid;
	setcontext(upnext->ucp);
}
  
int mypthread_yield (void){
	mypthread_t* exec_thread = find_thread(excutingThread);
	mypthread_t* upnext =find_next_active(exec_thread->tid);
	if (excutingThread==upnext->tid){
		return 0;
	}
	excutingThread = upnext->tid;
	swapcontext(exec_thread->ucp,upnext->ucp);
	return 0;
}


 int mypthread_join (mypthread_t thread, void **retval){
 	int t = thread.tid;
 	mypthread_t* exec_thread = find_thread(excutingThread);
 	mypthread_t* t_thread=find_thread(thread.tid);
 	if (t_thread->state != 0)
 	{
 		return 0; 
 	}
 	else
 	{
 		exec_thread->state=1;
 		t_thread->jointid = excutingThread;
 		excutingThread=t;
 		swapcontext(exec_thread->ucp,t_thread->ucp);
 	}
 	return 0;
 }
    