#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Fügt einen neuen Record hinzu und gibt die zugehörige ID zurück
int NVM_AddNewRecord(NVMManager* manager, NVMRecord* record, bool readonly, bool redundant) {

    //Suche nach einem freien Platz in der Allokationstabelle
    int id = -1;
    for (int i = 0; i < ALLOC_TABLE_SIZE; i++) {
        if (!manager->allocTable[i].used) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        printf("Error: Kein Platz mehr im Verwaltungsblock\n");
        return -1;
    }

    //Suche nach einem freien Platz im NVM-Speicher
    int start = -1;
    int length = record->header.length;
    for (int i = 0; i < NVM_SIZE - length; i++) {
        bool free = 1;
        for (int j = i; j < i + length; j++) {
            if (manager->nvm_data[j] != 0) {  
                free = 0;
                break;
            }
        }
        if (free) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        printf("Error: Kein Platz mehr im NVM-Speicher\n");
        return -1;
    }

    // Füge Record in die Allokationstabelle hinzu
    NVMRecordInfo* info = &manager->allocTable[id];
    info->id = id;
    info->start = start;
    info->length = length;
    info->used = 1;
    info->readonly_first = readonly;
    info->redundant = redundant;
    if (redundant) {
        info->redundancy_start = start + length;
    }
    info->valid = 1;
    info->checksum = 0;

    //Setzt Speicherbereich auf FF (reservierung?) 
    for (int i = start; i < start + length; i++) {
        manager->nvm_data[i] = 0xff;
    }
    //Setzt Speicherbereich auf FF (reservierung?) falls redundant
    if (info->redundant) {
        for (int i = start; i < info->redundancy_start + length; i++) {
            manager->nvm_data[i] = 0xff;
        }
    }

    printf("Record mit der ID: %d erfolgreich erstellt!\n", id);

    record->header.id = id;
    return id;
}
