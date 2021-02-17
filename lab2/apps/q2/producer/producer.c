#include "usertraps.h"
#include "lab2-api.h"
#include "misc.h"
#include "circ_buffer.h"

//code to implement the producer process
void main (int argc, char *argv[]) {

	char msg[] = "Hello World";	//message
	int msgLength = dstrlen(msg);	//length of the message
	int msgIndex = 0;			//index of the message
	
	circ_buffer *msgBuffer;		//buffer for the message
	uint32 h_mem;			//shared memory for producer and consumer
	sem_t s_procs_completed;	//semaphore like example
	lock_t bufferLock;		//lock to implement producer consumer

	//the producer create process will be called using the file name,
	//shared memory space, buffer lock, and semaphore for process completion

	//check for correct number of arguments
	if (argc != 4) {
		Printf("Incorrect number of arguments\n");
		Exit();
	}

	//convert arguments to integers for handles
	h_mem = dstrtol(argv[1], NULL, 10);
	s_procs_completed = dstrtol(argv[2], NULL, 10);
	bufferLock = dstrtol(argv[3], NULL, 10);

	//map shared memory to process memory
	if ((msgBuffer = (circ_buffer *)shmat(h_mem)) == NULL) {
		Printf("Could not map memory addreses\n");
		Exit();
	}

	//main insertion process to buffer
	
	while (msgIndex < msgLength)
	{
		//busy waiting?
		
		//acquire lock for process
		if (lock_acquire(bufferLock) != SYNC_SUCCESS) {
			Printf("Failed to acquire lock\n");
			Exit();
		}

		//check for buffer being full
		if ((msgBuffer -> head + 1) % 11 == msgBuffer->tail)
		{
		}
		else 
		{
			msgBuffer -> buffer[msgBuffer->head] = msg[msgIndex];
			msgBuffer -> head = (msgBuffer -> head + 1) % 11;
			//Print the message indicating that the producer is doing things
			Printf("Producer %d inserted: %c\n", getpid(), msg[msgIndex]);
			msgIndex = msgIndex + 1;
		}

		if (lock_release(bufferLock) != SYNC_SUCCESS) {
			Printf("Failed to release lock\n");
			Exit();
		}
	}

	//use semaphore to tell the original process that we're done
	if (sem_signal(s_procs_completed) != SYNC_SUCCESS) {
		Printf("Semaphore failed to update\n");
		Exit();
	} 
	
}



