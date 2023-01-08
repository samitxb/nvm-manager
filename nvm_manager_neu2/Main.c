#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"
#include "nvm_del.h"
#include "nvm_syncrw.h"



int main() {
	// Initialisiere NVM Manager
	NVMManager manager;
	NVM_Init(&manager);
	

	//Neuer Record
	unsigned char data1[] = { 1, 2, 3, 4, 5 };
	NVMRecord record1 = {
		.header.id = 0,
		.header.length = sizeof(data1)
	};
	memcpy(record1.data, data1, sizeof(data1)); 
	unsigned char data2[] = { 1, 2, 3, 4, 5, 6 };
	NVMRecord record2 = {
		.header.id = 1,
		.header.length = sizeof(data2)
	};
	memcpy(record2.data, data2, sizeof(data2));


	// To ADDRecord manger record & readonly & redundant
	int id = NVM_AddNewRecord(&manager, &record1, 0 , 0);

	//Schreibe Record
	//NVM_SyncWriteRecord(&manager, id, &record);
	NVM_SyncWriteRecord2(&manager, id, data1);


	// Lösche Record
	NVM_DeleteRecord(&manager, id);



	/*	NVMRecord record1;
	unsigned char data1[6] = { 1, 2, 3, 4, 5, 6 };
	memcpy(record1.data, data1, sizeof(data1)); //wird dann komischer weise 2x geschrieben
	printf("Size of Record Data %d\n", sizeof(data1));
	printf("Size of Record Data %zd\n", record1.data);*/
	//record1.header.id = 0;
	//record1.header.length = sizeof(data);


	return 0;
}