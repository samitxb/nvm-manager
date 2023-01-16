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



int main() {
	printf("\033[0;37m");

	// Initialisiere NVM Manager
	NVMManager manager;
	NVM_Init(&manager);

	// Neue Record Daten
	unsigned char data0[] = { 1, 2, 3, 4, 5 };
	unsigned char data1[] = { 1, 2, 3, 4, 5, 6 , 7 };
	unsigned char data2[] = { 1, 2, 3, 4, 5 };
	unsigned char data3[] = { 1, 2, 3, 4, 5 };
	// Erstellung der Records
	NVMRecord record0 = { .header.length = sizeof(data0) };
	NVMRecord record1 = { .header.length = sizeof(data1) };
	NVMRecord record2 = { .header.length = sizeof(data2) };
	NVMRecord record3 = { .header.length = sizeof(data3) };
	// Memcopy Daten in Record
	memcpy(record0.data, data0, sizeof(data0));
	memcpy(record1.data, data1, sizeof(data1));
	memcpy(record2.data, data2, sizeof(data2));
	memcpy(record3.data, data3, sizeof(data3));
	// To ADDRecord manger record & readonly & redundant
	int id0 = NVM_AddNewRecord(&manager, &record0, 0, 0);
	int id1 = NVM_AddNewRecord(&manager, &record1, 0, 0);
	//Schreibe Record
	NVM_SyncWriteRecord(&manager, &record0);
	NVM_SyncWriteRecord(&manager, &record1);
	//Read Record
	NVM_SyncReadRecord(&manager, &record0);
	NVM_SyncReadRecord(&manager, &record1);

	//Redundanz test
	int id2 = NVM_AddNewRecord(&manager, &record2, 0, 1);
	NVM_SyncWriteRecord(&manager, &record2);
	NVM_SyncReadRecord(&manager, &record2);
	//Readonly test
	int id3 = NVM_AddNewRecord(&manager, &record3, 1, 0);
	NVM_SyncWriteRecord(&manager, &record3);
	NVM_SyncReadRecord(&manager, &record3);

	for (int i = 0; i < 11; i++)
	{
		NVM_Handler(&manager, i, 0);
		int breakpoint2 = 0;

	}
	 
	/*	//Testsequenz für reorg funktion
	printf("\n\n\033[0;36m Testsequenz für resizeAllocTable(): \n\033[0;37m");
	NVM_DeleteRecord(&manager, id2);
	NVM_ReorganizeRecords(&manager);
	//int id5 = NVM_AddNewRecord(&manager, &record0, 0, 0);
	NVM_ReorganizeRecords(&manager);*/

	// Lösche Records
	NVM_DeleteRecord(&manager, id0);
	NVM_DeleteRecord(&manager, id1);
	NVM_DeleteRecord(&manager, id2);
	NVM_DeleteRecord(&manager, id3);

	while (!_kbhit())
	{
		Sleep(1);
	}
	return 0;
}
