#pragma once

#include "typedef.h"

unsigned char calc_lrc(unsigned char* data, int data_size);

void NVM_SyncWriteRecord2(NVMManager* manager, int id, unsigned char* data);

int NVM_SyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record);