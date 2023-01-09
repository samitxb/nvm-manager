#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


void NVM_IntegrityCheck(NVMManager* manager) {
    for (int i = 0; i < ALLOC_TABLE_SIZE; i++) {
        NVMRecordInfo* info = &manager->allocTable[i];
        if (!info->used) {
            // Record existiert nicht
            continue;
        }

        // Lese Record
        NVMRecord record;
        memcpy(&record, &manager->nvm_data[info->start], info->length);

        // Prüfe Checksumme
        if (record.checksum != calc_lrc(&record.data, sizeof(&record.data))) {
            // Checksumme ist falsch
            info->valid = false;
            continue;
        }

        /*

        if (info->redundant) {
            // Lese redundanten Record
            NVMRecord redundantRecord;
            memcpy(&redundantRecord, &manager->nvm_data[info->redundancy_start], info->length);

            // Prüfe Checksumme des redundanten Records
            if (record.checksum != calc_lrc(calc_lrc(&record.data, sizeof(&record.data))) {    
                // Checksumme des redundanten Records ist falsch
                info->valid = false;
                continue;
            }
        }
        */

        // Record ist gültig
        info->valid = true;
    }
}

