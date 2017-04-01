/*Implement a synchronization scheme that simulates the behavior of the customers of a bank as follows: 
The bank acquires K seats in the waiting room and a customer service desk. The customers can see from the window if there is free seating. 
If there is not, they go for a walk (take_a_walk ()) and retry later.If there is seating available, they enter the waiting room and 
attempt to be served one at a time in the customer service desk. The client is served by calling make_transaction (). 
Use shared variables and semaphores for your solution. Consider making any changes necessary at the points indicated with.  ... in 
the code segment that follows:
*/

#include <smeap
...

int k =10;
void make_transaction()
{
	// makes the transaction
	fprintf("Transaction Made! \n");

}

void take_a_walk()
{
	// makes the customer come back and try again 
	fprintf("Customer took a walk! \n");
}
void return_home()
{
	//removes the customer
}
void bank_client()
{
	while(1)
	{
		...
		if (...) 
		{/* if seats avaliable*/
			// take a seat 
			// make a queue
			...
		make_transaction();
		...
		break;
		}
		else{
		
			...
			take_a_walk();
		}
	}
	return_home();
}

int main(int argc, char **argv)
{
	queue waiting;	
	void bank_client()

}
