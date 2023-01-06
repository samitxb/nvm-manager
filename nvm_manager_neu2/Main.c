#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"



int main() {

	//Init NVM MANAGER

	// Verwaltungsblock für NVM Records
	NVMRecordManager nvm_alloctable[ALLOC_TABLE_SIZE];

	// NVM-Speicher
	unsigned char nvm_data[NVM_SIZE];


	NVM_Init(&nvm_alloctable, nvm_data);



	NVMRecord record1;
	unsigned char data[] = { 1, 2, 3, 4, 5 };

	//Neuer Record
	int id = NVM_AddNewRecord(&nvm_alloctable, &record1, nvm_data, 5, 0, 0);
	



	return 0;
}