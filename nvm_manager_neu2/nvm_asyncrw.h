#pragma once

#include "typedef.h"

int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback);

int NVM_AsyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback);

int NVM_Handler(NVMManager* manager, unsigned char* data, NVMRecord* record, NVMWriteCallback callback);