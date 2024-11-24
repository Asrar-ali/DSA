#include <stdio.h>          // Includes the standard input/output library
#include <stdlib.h>        // Includes the standard input/output library


struct Queue {
	char *data;     // Character pointer that store the actual data in the queue.
	int head;       // Represents the index of the start(Head) of the queue.
	int tail;       // Represents the index of the end(Tail) of the queue.
	int size;       // Represents the current number of elements in queue.
	int capacity;   // Represents the maximum number of elements that the  queue can hold.
};


void build(struct Queue **q, unsigned int length) {        // Build function initializes a queue with a given length for 2 parameters.
	*q = (struct Queue *)malloc(sizeof(struct Queue));    // Allocates memory dynamically and malloc() returns a pointer to a block of memory.
	if (*q == NULL) {                                     // Checks if memory allocation was successfull or not.
		printf("build()>> Memory allocation failed for Queue structure\n");    //If memory allocation fails, it prints an error message and exits the Build function.
		return;
	}

	(*q)->data = (char *)malloc(length * sizeof(char));  // Allocates memory for the array which store's the actual elements of the queue.
	if ((*q)->data == NULL) {
		printf("build()>> Memory allocation failed for queue data\n");   //If memory allocation for Data fails, the program prints an error message.
		free(*q);
		*q = NULL;
		return;      //It exits the function to prevent using an unallocated queue.
	}

	(*q)->head = -1;  // Initializing the head index as -1.
	(*q)->tail = -1;  // Initializing  the teal index to -1.
	(*q)->size = 0;   //Initializing  the size of the queue as 0.
	(*q)->capacity = length;  //Sets the maximum capacity of the queue to the specified length by the user.


	for (int i = 0; i < length; i++) {   // Initializing all elements to -128 withnthe help of for loop.
		(*q)->data[i] = -128;            // The value(-128) is used as a marker for uninitialized elements in the queue.

	}

	printf("build()>> Building a queue of length %d\n", length);
	printf("build()>> Allocated memory for the structure at the address %p\n", (void *)*q);
	printf("build()>> Initialize all elements of data to -128\n");
}



void enqueue(struct Queue *q, char x)  // This Enqueue function takes two parameters and does not return any value.
{    //Null Check for the Queue.
	if (q == NULL || q->data == NULL) {   // This condition checks if the data array inside the queue (q->data) is uninitialized or NULL.
		printf("enqueue()>> Queue is not initialized\n");   // if Null then prints the error message.
		return;
	}
       //Check for Queue Overflow.
	if (q->size == q->capacity - 1 ) {   //This condition checks if the queue is full or not. It considers full if the current size (q->size) is one less than the capacity (q->capacity - 1).
		printf("enqueue()>> Attempt to overflow the queue at %p was prevented.\n", (void *)q);  // If the queue is full then it prints message.
		return;
	}

	int tailIndex = (q->tail + 1) % q->capacity;
	if (q->size == 0) {       //Checks if the Queue is Empty or not.
		q->head = 0;          
		q->tail = 0;
	} else if (tailIndex == q->head) {// Checks for Circular Overflow.
		printf("enqueue()>> Attempt to overflow the queue at %p was prevented.\n", (void *)q);  // If the queue is full then it prints message.
		return;
	}

	q->data[tailIndex] = x;  // Insert the Element x
	q->tail = tailIndex;  // Update the tail Pointer
	q->size++;            // Update the size. 
}


char dequeue(struct Queue *q) {  // This dequeue function returns a character (char) that is dequeued from the queue.
	if (q == NULL || q->data == NULL) {   // Null Check for the Queue.
		printf("dequeue()>> Queue is not initialized\n"); // if the condition is null then print the error message and return -127.
		return -127;
	}
         // Check for Queue Underflow.
	if (q->size == 0) {      // Checking if the queue is empty.
		printf("dequeue()>> Attempt to underflow the queue at %p was prevented.\n", (void *)q);  //If the queue is empty then it prints an error message and return -127.
		return -127;
	}

	char x = q->data[q->head];  // Stores the element at the current head position of the queue  in a variable x.
	q->data[q->head] = -128;   // Clear the dequeued element by overwriting the value at the head position of the queue with -128.
       
         // Calculate the New Head Position
	int headIndex = (q->head + 1) % q->capacity;  // The modulo operator wraps the head index to the beginning of the array if it exceeds the queue's capacity.
	q->head = headIndex;  // Updating the head Pointer the new headindex.
	q->size--;  // Decreasing the Queue Size.
	
	if (q->size == 0) {   // If queue becomes empty after the dequeuing operation, both the head and tail are set to -1.
		q->head = -1;
		q->tail = -1;
	}

	return x;      // Returns Dequeued Element.
}


int main(int argc, const char * argv[])
{
	struct Queue *qptr = NULL;

	build(&qptr, 5);
	// ====== Beginn Test Case 1
	enqueue(qptr, 7);
	enqueue(qptr, 8);
	enqueue(qptr, 9);
	enqueue(qptr, 10);
	enqueue(qptr, 11);
	enqueue(qptr, 12);
	enqueue(qptr, 99);

	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));
	printf("Dequeue() %d \n", dequeue(qptr));

	// ====== End Test Case 1
	// Freeing anything that was created with malloc!
	if (qptr->data) free(qptr->data);
	if (qptr) free(qptr);

	printf("\n");
	return 0;
}
