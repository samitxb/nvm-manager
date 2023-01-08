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
	/*	NVMRecord record1;
	unsigned char data1[6] = { 1, 2, 3, 4, 5, 6 };
	memcpy(record1.data, data1, sizeof(data1)); //wird dann komischer weise 2x geschrieben
	printf("Size of Record Data %d\n", sizeof(data1));
	printf("Size of Record Data %zd\n", record1.data);*/

	NVMRecord record;
	unsigned char data[] = { 1, 2, 3, 4, 5, 6, 7 };
	record.header.id = 0;
	record.header.length = sizeof(data);
	memcpy(record.data, data, sizeof(data));  // Wird iwie 2mal in den Record geschrieben. Ka wieso.

	// To ADDRecord manger record & readonly & redundant
	int id = NVM_AddNewRecord(&manager, &record, 0 , 0);

	//Checksum berechnung
	//unsigned char e = calc_lrc(data, sizeof(data));
	//unsigned char e = calc_lrc(record.data, record.header.length);
	//printf("%d\n", e);


	//Schreibe Record
	//NVM_SyncWriteRecord(&manager, id, &record);

	NVM_SyncWriteRecord2(&manager, id, data);


	// Lösche Record
    //NVM_DeleteRecord(&manager, id);


	return 0;
}