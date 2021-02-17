#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"
#include "circ_buffer.h"

//main process to create the processes

void main (int argc, char *argv[])
{
	int numprocs = 0;	//used to store number of processes to create
	int i;			//loop index variable
	circ_buffer *buff;	//used to get address of shared memory page
	uint32 h_mem;		//used to hold handle to shared memory page
	lock_t buffer_lock; 	//buffer lock
	sem_t s_procs_completed;//semaphore used to wait until all spawned processes have completed
	char h_mem_str[10];	//used as a command line argument to pass mem_handle to new processes
	char s_procs_completed_str[10];	//used as a command line argument to pass page mapped handle to new processes
	char buffer_lock_str[10]; //used as a command line arg for the lock	

	if (argc != 2) 
	{
		Printf("Usage: ");
		Printf(argv[0]);
		Printf(" <number of processes to create>\n");
		Exit();
	}

	//Convert string from ascii command line argument to integer number
	numprocs = dstrtol(argv[1], NULL, 10);
	Printf("Creating %d processes\n", numprocs);


	//Allocate space for a shared memory page
	//64 somethings
	if ((h_mem = shmget()) == 0) {
		Printf("Error: could not allocate shared memory page in ");
		Printf(argv[0]);
		Printf(", exiting...\n");
		Exit();
	}

	//map shared memory page into this process's memory space
	if ((buff = (circ_buffer *)shmat(h_mem)) == NULL) {
		Printf("Could not map the shared page to virtual address in ");
		Printf(argv[0]);
		Printf(", exiting...\n");
		Exit();
	}

	
	//head and tails need to be zero
	buff -> head = 0;
	buff -> tail = 0;

	//need lock and semaphore

	if ((buffer_lock = lock_create()) == SYNC_SUCCESS) {
		Printf("Lock creation unsuccessful");
		Exit();
	}

	if ((s_procs_completed = sem_create(-(numprocs-1))) == SYNC_FAIL) {
		Printf("Semaphore creation unsuccessful");
		Exit();
	}

	//convert the handlers from strings to ints to pass them into the process creations
	ditoa(h_mem, h_mem_str);
	ditoa(s_procs_completed, s_procs_completed_str);
	ditoa(buffer_lock, buffer_lock_str);

	
	//start producing and consuming? jk creating processes to produce and consume
	for (i = 0; i < numprocs; i++) {
		process_create("producer.dlx.obj", h_mem_str, s_procs_completed_str, buffer_lock_str, NULL);
		process_create("consumer.dlx.obj", h_mem_str, s_procs_completed_str, buffer_lock_str, NULL);
	}

	//wait until all processes have finished?
	if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
		Printf("somethingwent rong here\n");
		Exit();
	}	
}
