#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Löschen eines Records
int NVM_DeleteRecord(NVMManager* manager, NVMRecord* record) {
    int id = record->header.id;
    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert und nicht schreibgeschützt ist
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !info->used || info->readonly) {
        printf("Ungueltige ID oder Record ist schreibgeschuetzt! ID: %d\n", id);
        return -1;
    }

    // Setze den Speicherbereich des Records auf FF und markiere den Record als unbenutzt
    for (int i = info->start; i < info->start + info->length; i++) {
        manager->nvmData[i] = 0xff;
    }
    if (info->redundant) {
        for (int i = info->redundancyStart; i < info->redundancyStart + info->length; i++) {
            manager->nvmData[i] = 0xff;
        }
    }
    info->used = 0;
    info->readonly = 0;
    info->redundant = 0;
    info->redundancyStart = 0;
    info->valid = 0;
    info->id = 0;
    info->length = 0;
    info->start = 0;
    info->checksum = 0;

    printf("Record mit der ID: %d erfolgreich geloescht!\n", id);
    return 0;
}

