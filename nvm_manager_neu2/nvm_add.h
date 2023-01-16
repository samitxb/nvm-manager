#pragma once

#include "typedef.h"

int NVM_AddNewRecord(NVMManager* manager, NVMRecord* record, bool readonly, bool redundant);

//int NVM_AddNewRecord(NVMRecordManager* nvm_alloctable, NVMRecord* record, unsigned char nvmData[], int length, bool readonly, bool redundant);