#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Löschen eines Records
void NVM_DeleteRecord(NVMManager* manager, int id) {

    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert und nicht schreibgeschützt ist
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !info->used || info->readonly) {
        printf("Ungültige ID oder Record ist schreibgeschützt\n");
        return;
    }

    // Setze den Speicherbereich des Records auf FF und markiere den Record als unbenutzt
    for (int i = info->start; i < info->start + info->length; i++) {
        manager->nvm_data[i] = 0xff;
    }
    if (info->redundant) {
        for (int i = info->redundancy_start; i < info->redundancy_start + info->length; i++) {
            manager->nvm_data[i] = 0xff;
        }
    }
    info->used = 0;
    info->readonly = 0;
    info->redundant = 0;
    info->redundancy_start = 0;
    info->valid = 0;
    info->id = 0;
    info->length = 0;
    info->start = 0;
    info->checksum = 0;
}

/*// Löscht einen Record
void NVM_DeleteRecord(int id) {
    if (id < 0 || id >= NVM_NUM_RECORDS || !nvm_records[id].used) {
        printf("Error: Invalid record ID\n");
        return;
    }

    // Speicherbereich des Records auf FF setzen
    for (int i = nvm_records[id].start_pos; i < nvm_records[id].start_pos + nvm_records[id].length; i++) {
        nvm_storage[i] = 0xff;
    }
    if (nvm_records[id].redundant) {
        for (int i = nvm_records[id].redundancy_start_pos;
            i < nvm_records[id].redundancy_start_pos + nvm_records[id].length; i++) {
            nvm_storage[i] = 0xff;
        }
    }

    // Verwaltungsdaten des Records zurücksetzen
    memset(&nvm_records[id], 0, sizeof(nvm_record_t));
}*/


/*// Löscht einen Record
void NVM_DeleteRecord(NVMManager* manager, int id) {
    NVMRecordInfo* info = &manager->allocTable[id];
    if (!info->used) {
        return;
    }

    // Setze Recordbereich auf FF und markiere im Verwaltungsblock als nicht mehr existent
    memset(&manager->nvm_data[info->start], 0xFF, info->length);
    info->used = 0;
}*/
