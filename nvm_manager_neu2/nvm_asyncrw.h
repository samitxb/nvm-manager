#pragma once

#include "typedef.h"

void NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record);

int NVM_AsyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record);

int NVM_ProcessQueue(NVMManager* manager);