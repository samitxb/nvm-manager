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

	//Neuer Record
	unsigned char data1[] = { 1, 2, 3, 4, 5 };
	NVMRecord record1 = {
		//.header.id = 0,
		.header.length = sizeof(data1)
	};	
	unsigned char data2[] = { 1, 2, 3, 4, 5, 6 , 7};
	NVMRecord record2 = {
		//.header.id = 1,
		.header.length = sizeof(data2)
	};
	unsigned char data3[] = { 1, 2, 3, 4, 5 };
	NVMRecord record3 = {
		//.header.id = 2,
		.header.length = sizeof(data3)
	};
	unsigned char data4[] = { 1, 2, 3, 4, 5 };
	NVMRecord record4 = {
		//.header.id = 2,
		.header.length = sizeof(data4)
	};

	// To ADDRecord manger record & readonly & redundant
	int id1 = NVM_AddNewRecord(&manager, &record1, 0, 0);
	int id2 = NVM_AddNewRecord(&manager, &record2, 0, 0);

	//Schreibe Record
	NVM_SyncWriteRecord(&manager, &record1, data1);
	NVM_SyncWriteRecord(&manager, &record2, data2);

	//Read Record
	NVM_SyncReadRecord(&manager, &record1);
	NVM_SyncReadRecord(&manager, &record2);
	printf("Daten vom Record1:\n");
	for (int i = 0; i < sizeof(data1); i++) {
		printf("%d ", record1.data[i]);
	}
	printf("\n");
	printf("Daten vom Record2:\n");
	for (int i = 0; i < sizeof(data2); i++) {
		printf("%d ", record2.data[i]);
	}
	printf("\n");

	//Redundanz test
	int id3 = NVM_AddNewRecord(&manager, &record3, 0, 1);
	NVM_SyncWriteRecord(&manager, &record3, data3);
	NVM_SyncReadRecord(&manager, &record3);
	printf("Daten vom Record3: \n ");
	for (int i = 0; i < sizeof(data3); i++) {
	printf("%d ", record3.data[i]);
	}
	printf("\n");

	//Readonly test
	int id4 = NVM_AddNewRecord(&manager, &record4, 1, 0);
	NVM_SyncWriteRecord(&manager, &record4, data4);
	NVM_SyncReadRecord(&manager, &record4);
	printf("Daten vom Record4: \n ");
	for (int i = 0; i < sizeof(data4); i++) {
		printf("%d ", record4.data[i]);
	}
	printf("\n");

	for (int i = 0; i < 11; i++)
	{
		NVM_Handler(&manager, i, 0);
		int breakpoint2 = 0;

	}
	 
	//Testsequenz f�r reorg funktion
	printf("\n\n\033[0;36m Testsequenz f�r resizeAllocTable(): \n\033[0;37m");

	NVM_DeleteRecord(&manager, id2);
	NVM_ReorganizeRecords(&manager);

	int id5 = NVM_AddNewRecord(&manager, &record1, 0, 0);

	NVM_ReorganizeRecords(&manager);





	// L�sche Record
	NVM_DeleteRecord(&manager, id1);
	NVM_DeleteRecord(&manager, id2);
	NVM_DeleteRecord(&manager, id3);


	return 0;
}