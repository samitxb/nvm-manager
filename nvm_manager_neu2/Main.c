#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"
#include "nvm_del.h"
#include "nvm_syncrw.h"
#include "nvm_asyncrw.h"
#include "nvm_reorganize.h"
#include "resizeAllocTable.h"
#include "updateAllocTable.h"


/*typedef struct createRecord(unsigned char data) {

	NVMRecord record = {
		//.header.id = 0,
		.header.length = sizeof(data)
	};

	return record;
};*/


int main() {
	printf("\033[0;37m");

	// Initialisiere NVM Manager
	NVMManager manager;
	NVM_Init(&manager);

	//Neuer Record
	unsigned char data1[] = { 1, 2, 3, 4, 5 };
	unsigned char data2[] = { 1, 2, 3, 4, 5, 6 , 7 };
	unsigned char data3[] = { 1, 2, 3, 4, 5 };
	unsigned char data4[] = { 1, 2, 3, 4, 5 };

	NVMRecord record1 = { .header.length = sizeof(data1) };	
	NVMRecord record2 = { .header.length = sizeof(data2) };
	NVMRecord record3 = { .header.length = sizeof(data3) };
	NVMRecord record4 = { .header.length = sizeof(data4) };

	// To ADDRecord manger record & readonly & redundant
	int id1 = NVM_AddNewRecord(&manager, &record1, 0, 0);
	int id2 = NVM_AddNewRecord(&manager, &record2, 0, 0);

	//Schreibe Record
	NVM_SyncWriteRecord(&manager, &record1, data1);
	NVM_SyncWriteRecord(&manager, &record2, data2);

	//Read Record
	NVM_SyncReadRecord(&manager, &record1);
	NVM_SyncReadRecord(&manager, &record2);

	//Redundanz test
	int id3 = NVM_AddNewRecord(&manager, &record3, 0, 1);
	NVM_SyncWriteRecord(&manager, &record3, data3);
	NVM_SyncReadRecord(&manager, &record3);

	//Readonly test
	int id4 = NVM_AddNewRecord(&manager, &record4, 1, 0);
	NVM_SyncWriteRecord(&manager, &record4, data4);
	NVM_SyncReadRecord(&manager, &record4);


	for (int i = 0; i < 11; i++)
	{
		NVM_Handler(&manager, i, 0);
		int breakpoint2 = 0;

	}
	 
	//Testsequenz für reorg funktion
	//printf("\n\n\033[0;36m Testsequenz für resizeAllocTable(): \n\033[0;37m");

	//NVM_DeleteRecord(&manager, id2);
	//NVM_ReorganizeRecords(&manager);

	//int id5 = NVM_AddNewRecord(&manager, &record1, 0, 0);

	//NVM_ReorganizeRecords(&manager);


	// Lösche Records
	NVM_DeleteRecord(&manager, id1);
	NVM_DeleteRecord(&manager, id2);
	NVM_DeleteRecord(&manager, id3);
	NVM_DeleteRecord(&manager, id4);

	while (!_kbhit())
	{
		Sleep(1);
	}
	return 0;
}
