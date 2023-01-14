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
	//memcpy(record1.data, data1, sizeof(data1));
	unsigned char data2[] = { 1, 2, 3, 4, 5, 6 , 7};
	NVMRecord record2 = {
		.header.id = 1,
		.header.length = sizeof(data2)
	};
	//memcpy(record2.data, data2, sizeof(data2));

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



	//Record für Redundanz
	unsigned char data3[] = { 1, 2, 3, 4, 5, 6 , 7 };
	NVMRecord record3 = {
		.header.id = 2,
		.header.length = sizeof(data3)
	};
	int id3 = NVM_AddNewRecord(&manager, &record3, 0, 1);
	NVM_SyncWriteRecord(&manager, &record3, data3);
	NVM_SyncReadRecord(&manager, &record3);
	printf("Daten vom Record3: \n ");
	for (int i = 0; i < sizeof(data3); i++) {
	printf("%d ", record3.data[i]);
	}
	printf("\n");
	

	while (1)
	{
		NVM_AsyncWriteRecord(&manager, id3, data3, &record3);
		NVM_AsyncReadRecord(&manager, id2, data2, &record2);

		if (manager.queueCount == 100)
		{
			printf("\nfinish!");
			break;
		}
	}


	//Testsequenz für resizeAllocTable
	biggerAllocTable* newAllocTable = resizeAllocTable(&manager);
	printf("\n\n \046[0;36m Einträge der resized AllocTable: \n");
	for (int i = 0; i < 30 /*eigentlich NVM_SIZE*/; i++) {
		printf("Eintrag %d: ID = %d, Start = %d, Länge = %d\n", i, newAllocTable[i].id, newAllocTable[i].start, newAllocTable[i].length);
	}
	


	//Testsequenz für reorg funktion
	NVM_DeleteRecord(&manager, id2);
	NVM_ReorganizeRecords(&manager);





	// Lösche Record
	NVM_DeleteRecord(&manager, id1);
	NVM_DeleteRecord(&manager, id2);
	NVM_DeleteRecord(&manager, id3);

	return 0;
}