#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "circ_buffer.h"

//code to implement consumer process
void main (int argc, char *argv[])
{
	int msgLength = 11;
	int msgIndex = 0;

	circ_buffer *msgBuffer;
	uint32 h_mem;
	sem_t s_procs_completed;
	lock_t bufferLock;

	//check for the correct number of arguments
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
		Printf("Could not map memory addresses\n");
		Exit();
	}

	//main consumption process from buffer
	while (msgIndex < msgLength) {
		//check for correct lock acquisition
		if (lock_acquire(bufferLock) != SYNC_SUCCESS) {
			Printf("Lock not acquired successfully\n");
			Exit();
		}
		
		//check that there is something in the buffer
		if (msgBuffer -> head != msgBuffer -> tail) 
		{
			Printf("Consumer %d removed: %c\n", getpid(), msgBuffer -> buffer[msgBuffer -> tail]);
			msgBuffer -> tail = (msgBuffer -> tail + 1) % 11;
			msgIndex = msgIndex + 1;
		}

		//release lock and check for success
		if (lock_release(bufferLock) != SYNC_SUCCESS) {
			Printf("Lock not released successfully\n");
			Exit();
		}
	}

	//use semaphore to signal process completion
	if (sem_signal(s_procs_completed) != SYNC_SUCCESS) {
		Printf("Semaphore not incrememted successfully\n");
		Exit();
	}
	
}
