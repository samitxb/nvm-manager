#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "typedef.h"



void initqueue() {
	NVM_QueueEntry nvm_write_queue[NVM_QUEUE_SIZE];
	int nvm_write_queue_start = 0;
	int nvm_write_queue_end = 0;

	NVM_QueueEntry nvm_read_queue[NVM_QUEUE_SIZE];
	int nvm_read_queue_start = 0;
	int nvm_read_queue_end = 0;
}

// Initialisierung des NVM Managers
void NVM_Init() {
	memset(nvm_records, 0, sizeof(nvm_records));
	memset(nvm_data, 0xff, sizeof(nvm_data));
}