#ifndef __USERPROG__
#define __USERPROG__

//Defines circular buffer to be used in producer consumer problem

#define SIZE_OF_BUFFER 11

typedef struct {
	char buffer[SIZE_OF_BUFFER]; 	//length of hello world string
	int head;
	int tail;
	
} circ_buffer;

#endif

