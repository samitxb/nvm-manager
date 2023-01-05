#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"



int main() {

	//Init NVM MANAGER
	initqueue();
	NVM_Init();

	//Neuer Record
	int id = NVM_AddNewRecord(5, 0, 0);



	return 0;
}