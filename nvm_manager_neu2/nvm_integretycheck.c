#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "nvm_syncrw.h"
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
        memcpy(&record.data, &manager->nvmData[info->start], info->length);

        // Prüfe Checksumme
        if (info->checksum != NVM_CalculateChecksum(&record.data, info->length)) {
            // Checksumme ist falsch
            info->valid = false;
            printf("\e[0m\033[0; 37Fehler: Integritätsprüfung fehlgeschlagen bei Record mit ID %d\e[0m\n", i);
            continue;
        }
        // Record ist gültig
        info->valid = true;
    }
}