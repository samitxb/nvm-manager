#pragma once

#include "typedef.h"

unsigned char NVM_CalculateChecksum(unsigned char* data, int data_size);

int NVM_SyncWriteRecord(NVMManager* manager, NVMRecord* record);

int NVM_SyncReadRecord(NVMManager* manager, NVMRecord* record);

