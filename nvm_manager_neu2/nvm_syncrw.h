#pragma once

#include "typedef.h"

unsigned char NVM_CalculateChecksum(unsigned char* data, int data_size);

void NVM_SyncWriteRecord(NVMManager* manager, int id, unsigned char* data);

int NVM_SyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record);