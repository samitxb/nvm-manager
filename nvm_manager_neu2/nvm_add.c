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
            if (manager->nvmData[j] != 0) {  
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
    info->readonlyFirst = readonly;
    info->redundant = redundant;
    if (redundant) {
        info->redundancyStart = start + length;
    }
    // Valid und Checksum auf 0
    info->valid = 0;
    info->checksum = 0;

    // Reserviert Speicherbereich
    for (int i = start; i < start + length; i++) {
        manager->nvmData[i] = 0xff;
    }
    // Reserviert redundanten Speicherbereich falls redundant
    if (info->redundant) {
        for (int i = start; i < info->redundancyStart + length; i++) {
            manager->nvmData[i] = 0xff;
        }
    }

    printf("Record mit der ID: %d erfolgreich erstellt!\n", id);

    // Schreibe die id in den Record
    record->header.id = id;
    return id;
}
