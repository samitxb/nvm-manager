#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


/*void initqueue() {
	NVM_QueueEntry nvm_write_queue[NVM_QUEUE_SIZE];
	int nvm_write_queue_start = 0;
	int nvm_write_queue_end = 0;

	NVM_QueueEntry nvm_read_queue[NVM_QUEUE_SIZE];
	int nvm_read_queue_start = 0;
	int nvm_read_queue_end = 0;
}*/

/*// Initialisierung des NVM Managers
void NVM_Init(NVMRecordManager* nvm_alloctable, unsigned char nvm_data[]) {
	memset(nvm_alloctable, 0, sizeof(nvm_alloctable));
	memset(nvm_data, 0xff, sizeof(nvm_data));
}*/




// Initialisiert den NVM-Manager
int NVM_Init(NVMManager* manager) {
	memset(manager, 0, sizeof(NVMManager));

	// Initialisiere die Warteschlange
	manager->queueStart = 0;
	manager->queueEnd = 0;
	manager->queueCount = 0;



	return 0;
}
