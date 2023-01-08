#pragma once

#include "typedef.h"

unsigned char calc_lrc(unsigned char* data, int data_size);

//char calculateLRC(unsigned char* data);

//unsigned char NVM_CalculateChecksum(NVMRecord* record);

//unsigned char calc_checksum(unsigned char* data);

//int NVM_SyncWriteRecord(NVMManager* manager, int id, NVMRecord* record);

void NVM_SyncWriteRecord2(NVMManager* manager, int id, unsigned char* data);