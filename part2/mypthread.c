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
// Type your globals...

// Type your own functions (void)
// e.g., free up sets of data structures created in your library
int threadCount=1;
int numThreadNodes=0;
int excutingThread=1;

#define MEM_STACK 1024*4

void add_node();
void add_to_queue(mypthread_t* threadname);

mypthread_t* find_thread(int findid);

// must pass current id to work
mypthread_t* find_next_active(int currentid);

//nodes for linked list
struct threadNode* head; //head node
struct threadNode* tail; //tail node
struct threadNode* temp; //temp node

    
//  Write your thread create function here...
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	//printf("Creating\n");
	if (numThreadNodes == 0) // first thread being created
	{
		//this is creating the parent thread
		mypthread_t* parentthread= (mypthread_t *)malloc(sizeof(mypthread_t));
		//set thread id to thread count and increase thread count
		parentthread->tid=threadCount++;
		//set up the context of parent thread
		ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
		parentthread->ucp = context;
		//allocate space and stack pointer
		parentthread->ucp->uc_stack.ss_sp = malloc(MEM_STACK);
		parentthread->ucp->uc_stack.ss_size = MEM_STACK;
		//set to active state
		parentthread->state = 0;
		//add parent thread to to thread queue
		add_to_queue(parentthread);
	}
	//set up context of thread
	ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
	//give thread context
	thread->ucp = context;
	getcontext(thread->ucp);
	//allocate space and stack pointer
	thread->ucp->uc_stack.ss_sp = malloc(MEM_STACK);
	thread->ucp->uc_stack.ss_size = MEM_STACK;
	//set thread id to thread count and increase thread count
	thread->tid = threadCount++;
	//set state to active
	thread->state = 0;
	//make context with given function
	makecontext(thread->ucp, (void(*)()) start_routine, 1, arg);
	//add function to the queue
	add_to_queue(thread);
	return 0;
} 

void mypthread_exit (void *retval){
	//printf("Exiting\n");
	//find the thread
	mypthread_t* exec_thread = find_thread(excutingThread);

	exec_thread->state = 2; //set thread state to dead
	free(exec_thread->ucp); //since thread exiting, give memory back
	if (exec_thread->jointid != 0){
		//find thread
		mypthread_t* joining_thread = find_thread(exec_thread->jointid);

		joining_thread->state = 0;//set state to active
	}
	//find next active thread
	mypthread_t* upnext =find_next_active(exec_thread->tid);
	//if the next active thread found is this thread, return and stay running
	if (excutingThread == upnext->tid){
		return;
	}
	//thread id of executing thread is the next thread's id
	excutingThread = upnext->tid;
	//set context to next thread
	setcontext(upnext->ucp);
}
  
int mypthread_yield (void){
	//printf("Yielding\n");
	//get the executing thread
	mypthread_t* exec_thread = find_thread(excutingThread);
	//find the thread to yield to, which is next active thread on the queue
	mypthread_t* upnext =find_next_active(exec_thread->tid);
	//if the next active is same thread just return
	if (excutingThread==upnext->tid){
		return 0;
	}
	//make next thread id the executing thread
	excutingThread = upnext->tid;
	//swap the context between the current thread context and the next context thread to yield
	swapcontext(exec_thread->ucp,upnext->ucp);
	return 0;
}


 int mypthread_join (mypthread_t thread, void **retval){
 	//printf("Join\n");
 	//get the thread id of thread
 	int t = thread.tid;
 	//find currently executing thread
 	mypthread_t* exec_thread = find_thread(excutingThread);
 	
 	//find thread 
 	mypthread_t* t_thread=find_thread(thread.tid);
 	if (t_thread->state != 0)
 	{
 		return 0;  //if the thread is not active return
 	}
 	else
 	{
 		//if it is active set the executing thread state to blocked
 		exec_thread->state=1;

 		t_thread->jointid = excutingThread;
 		//make thread t the executing thread
 		excutingThread=t;
 		//swap the context and make thread the executing thread
 		swapcontext(exec_thread->ucp,t_thread->ucp);
 	}
 	return 0;
 }
 
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
		add_node();	//add this new node
		head=temp; // set head to temp
		tail=temp; // set tail to temp
	}
	else
	{
		add_node();	//add this new node
		tail->next = temp;//set the next node to tail to temp node
		tail=temp;//make themp node to tail
		tail->next= head; //set next of tail to head to make it a circle
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
