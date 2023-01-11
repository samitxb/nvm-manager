#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Asynchron schreibt einen Record in die Warteschlange
int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record) {
    if (manager->queueCount == QUEUE_SIZE) {
        // Warteschlange ist voll
        return -1;
    }

    // Füge Record in Warteschlange ein
    manager->queue[manager->queueEnd] = id;
    manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
    manager->queueCount++;

    // Kopiere Record
    NVMRecord* queuedRecord = malloc(sizeof(NVMRecordHeader) + record->header.length);
    memcpy(queuedRecord, record, sizeof(NVMRecordHeader) + record->header.length);

    return 0;
}

// Asynchron liest einen Record in die Warteschlange
int NVM_AsyncReadRecord(NVMManager* manager, int id, NVMRecord* record) {
    if (manager->queueCount == QUEUE_SIZE) {
        // Warteschlange ist voll
        return -1;
    }

    // Füge Record in Warteschlange ein
    manager->queue[manager->queueEnd] = -id; // Negative ID zum Unterscheiden von Schreiboperationen
    manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
    manager->queueCount++;

    return 0;
}

// Verarbeitet einen Record aus der Warteschlange
int NVM_ProcessQueue(NVMManager* manager) {
    NVMRecord* record;
    if (manager->queueCount == 0) {
        // Warteschlange ist leer
        return 0;
    }

    // Hole Record aus Warteschlange
    int id = manager->queue[manager->queueStart];
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount--;
  
    printf(NVM_ProcessQueue);

    if (id < 0) {
        // Lesebefehl
        id = -id;
        NVMRecordInfo* info = &manager->allocTable[id];
        if (!info->used) {
            return -1;
        }

        // Lese Record



        memcpy(&record, &manager->nvm_data[info->start], info->length);

        // Prüfe Checksumme
        if (record->checksum != NVM_CalculateChecksum(record->data, info->length)) {
            // Checksumme ist falsch
            return -1;
        }

        if (info->redundant) {
            // Lese redundanten Record
            NVMRecord redundantRecord;
            memcpy(&redundantRecord, &manager->nvm_data[info->redundant], info->length);

            // Vergleiche Checksummen
            if (record->checksum != redundantRecord.checksum) {
                // Checksummen sind unterschiedlich
                return -1;
            }
        }
    }
    else {
        // Schreibbefehl
        NVMRecord* queuedRecord = (NVMRecord*)&manager->queueRecords[id];

        // Schreibe Record
      //  unsigned int result = NVM_SyncWriteRecord(&manager, id, queuedRecord);

        // Speicher freigeben
        free(queuedRecord);
    }

    return 1;
}
