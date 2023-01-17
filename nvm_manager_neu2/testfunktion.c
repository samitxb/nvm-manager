#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <conio.h>
#include <Windows.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"
#include "nvm_del.h"
#include "nvm_syncrw.h"
#include "nvm_asyncrw.h"
#include "nvm_reorganize.h"


int testfunktion() {
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


	//Test für Async
	unsigned char data4[] = { 1, 2, 3, 4, 5 };
	unsigned char data5[] = { 1, 2, 3, 4, 5, 6, 7 };
	NVMRecord record4 = { .header.length = sizeof(data4) };
	NVMRecord record5 = { .header.length = sizeof(data5) };
	memcpy(record4.data, data4, sizeof(data4));
	memcpy(record5.data, data5, sizeof(data5));
	int id4 = NVM_AddNewRecord(&manager, &record4, 0, 0);
	int id5 = NVM_AddNewRecord(&manager, &record5, 0, 0);

	for (int i = 0; i < 11; i++)
	{
		NVM_Handler(&manager, i, data4, &record4, 0);
		int breakpoint2 = 0;

	}

	/*	//Testsequenz für reorg funktion
	printf("\n\n\033[0;36m Testsequenz für resizeAllocTable(): \n\033[0;37m");
	NVM_DeleteRecord(&manager, id2);
	NVM_ReorganizeRecords(&manager);
	//int id5 = NVM_AddNewRecord(&manager, &record0, 0, 0);
	NVM_ReorganizeRecords(&manager);*/

	// Lösche Records
	NVM_DeleteRecord(&manager, &record0);
	NVM_DeleteRecord(&manager, &record1);
	NVM_DeleteRecord(&manager, &record2);
	NVM_DeleteRecord(&manager, &record3);




	/*
		// Initialisiere NVM Manager
	//NVMManager manager;
	//NVM_demoapplikation(&manager);

	NVMManager manager;
	NVMRecord record;
	int id;
	int i;
	int ret;

	// Initialisiere den NVM-Manager
	ret = NVM_Init(&manager);
	assert(ret == 0);

	// Teste das Hinzufügen eines neuen Datensatzes
	record.header.length = 2;
	id = NVM_AddNewRecord(&manager, &record, false, false);
	assert(id == 0);

	// Teste das Löschen eines Datensatzes
	ret = NVM_DeleteRecord(&manager, id);
	assert(ret == 0);

	// Teste das Hinzufügen eines schreibgeschützten Datensatzes
	record.header.length = 2;
	id = NVM_AddNewRecord(&manager, &record, true, false);

	assert(id == 0);

	// Teste das Löschen eines schreibgeschützten Datensatzes
	ret = NVM_DeleteRecord(&manager, id);
	assert(ret == 0);
	printf("\n%d", ret);

	// Teste das Hinzufügen eines redundanten Datensatzes
	record.header.length = 3;
	id = NVM_AddNewRecord(&manager, &record, false, true);
	assert(id == 0);

	// Teste das Löschen eines redundanten Datensatzes
	ret = NVM_DeleteRecord(&manager, id);
	assert(ret == 0);*/

	// Teste das Hinzufügen von Datensätzen, bis kein Platz mehr im NVM-Speicher ist
	//for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
	//    record.header.length = 10;
	//    id = NVM_AddNewRecord(&manager, &record, false, false);
	//    if (id == -1) {
	//        break;
	//    }
	//}
	//assert(i == ALLOC_TABLE_SIZE - 1);


	return 0;
}