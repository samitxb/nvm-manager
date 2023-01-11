#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Schreibt einen async-Record in die Warteschlange
int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {
    if (manager->queueCount == QUEUE_SIZE) {
        // Warteschlange ist voll
        callback(-1);
        return -1;
    }

    // Fügt Record in Warteschlange ein
    manager->queue[manager->queueEnd] = id;
    manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
    manager->queueCount++;

    // Kopiere Record
    NVMRecord* queuedRecord = malloc(sizeof(NVMRecordHeader) + record->header.length);
    memcpy(queuedRecord, record, sizeof(NVMRecordHeader) + record->header.length);

    printf("\nWrite Queue Count: %d mit ID=%i", manager->queueCount, id);

    return 0;
}


// Liest einen async-Record in der Warteschlange
int NVM_AsyncReadRecord(NVMManager* manager, int id, NVMRecord* record) {
    if (manager->queueCount == QUEUE_SIZE) {
        // Warteschlange ist voll
        printf("\nRead Queue failed");
        return -1;
    }

    // Füge Record in Warteschlange ein
    manager->queue[manager->queueEnd] = -id; // Negative ID zum Unterscheiden von Schreiboperationen
    manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
    manager->queueCount++;
    printf("\nRead Queue Count:  %d mit ID=%i", manager->queueCount, id);

    return 0;
}

// Verarbeitet einen Record aus der Warteschlange
int NVM_ProcessQueue(NVMManager* manager) {
    NVMRecord* record;
    NVMWriteCallback callback;
    if (manager->queueCount == 0) {
        // Warteschlange ist leer
        printf("\nLeer");
        return 0;
    }

    // Hole Record aus Warteschlange
    int id = manager->queue[manager->queueStart];
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount--;
  

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
        int status = NVM_SyncWriteRecord(&manager, id, queuedRecord);
        printf("%d", status);

        //Callback aufrufen
        NVMWriteCallback callback = manager->writeCallbacks[id];
        callback(status);

        // Speicher freigeben
        free(queuedRecord);
    }

    return 1;
}

void NVM_Handler(NVMManager* manager, int id, NVMWriteCallback* callback) {
    NVMRecord* record;
    while (manager->queueCount > 0) {
        record = manager->queue[manager->queueStart];
        manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
        manager->queueCount--;

        if (record->data!= NULL) {
            // Perform async write to NVM
            // ...
            // Call callback function when write is complete
            NVM_AsyncWriteRecord(&manager, id, record->data, &record, &callback);
        } 
        else {
            // Perform async read from NVM
            // ...
            // Call callback function when read is complete
            unsigned char* data = NVM_AsyncReadRecord(&manager, id, &record);
        }

        free(record);
    }
}