#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "nvm_reorganize.h"
#include "typedef.h"


// Liest einen async-Record in der Schreibeschlange
int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record) {

    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];

    int callback = 0;

    // Erstelle einen neuen Record
    record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
    if (record == NULL) {
        return -1;
    }

    manager->queue[manager->queueCount] = id;
    manager->queueSchreiben = (manager->queueSchreiben + 1) % QUEUE_SIZE;


    if (manager->queueSchreiben != manager->queueLesen)
    {
        queueRecord->id = id;
        queueRecord->length = infoAlloc->length;
        queueRecord->readonly = infoAlloc->readonly;
        queueRecord->readonlyFirst = infoAlloc->readonlyFirst;
        queueRecord->redundancyStart = infoAlloc->redundancyStart;
        queueRecord->redundant = infoAlloc->redundant;
        queueRecord->start = infoAlloc->start;
        queueRecord->used = infoAlloc->used;
        queueRecord->valid = infoAlloc->valid;
        queueRecord->checksum = infoAlloc->checksum;

    }


    return callback;
}

// Liest einen async-Record in der Warteschlange
int NVM_AsyncReadRecord(NVMManager* manager, int id,  unsigned char* data, NVMRecord* record) {

    int callback = 0;

    //int id = record->header.id;

    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecordInfo* infoQueue = &manager->queue[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];


    if (manager->queueLesen == QUEUE_SIZE) {
        // Warteschlange ist voll
        printf("\nWarteschlange ist voll!");
        callback -1;
    }

    // Prüfe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !infoAlloc->used) {
        printf("\nUngueltige ID oder Record existiert nicht in AllocTable\n");
        callback -1;
    }

    manager->queue[manager->queueCount] = id;     // Für Readoperation
    manager->queueLesen = (manager->queueLesen + 1) % QUEUE_SIZE;
    manager->queueCount++;


    /*
    if (manager->queueLesen != manager->queueSchreiben)
    {
        //Lese Record aus NVM_Data & schreibe in record.data    
        memcpy(record->data, &manager->nvmData[infoAlloc->start], infoAlloc->length);
        //infoAlloc->id = manager->queue[manager->queueLesen];
    }*/

    return callback;
}



int NVM_Handler(NVMManager* manager, int id, NVMRecord* record) 
{
    NVMRecordInfo* queueId = &manager->queue[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];

   



    int callback = 0;

    // Überprüfe, ob es sich um einen Lesebefehl handelt
    if (manager->queueSchreiben == 0)                                   //Fehlerquelle: Deswegen Bugs in Warteschlange
    {

        // Record zwischenzeitlich erstellen
        NVMRecord getDataFromTable =
        {
        .header.id = infoAlloc->id,
        .header.length = infoAlloc->length,
        .checksum = infoAlloc->checksum
        };

        // Überprüfe, ob der Record genutzt wird
        if (!infoAlloc->used) {
            printf("Record wird nicht genutzt\n");
            callback -1;
        }
        // Lese Record aus
        memcpy(&getDataFromTable.data, &manager->nvmData[infoAlloc->start], infoAlloc->length);


        queueRecord->id = 0;
        queueRecord->start = 0;
        queueRecord->checksum = 0;
        queueRecord->length = 0;
        queueRecord->readonly = 0;
        queueRecord->readonlyFirst = 0;
        queueRecord->redundancyStart = 0;
        queueRecord->redundant = 0;
        queueRecord->start = 0;
        queueRecord->valid = 0;
        queueRecord->used = 0;

        for (int i = 0; i < manager->queueCount; i++) {

            if (manager->queue[i] == id) {
                manager->queue[i] = 0;
                manager->queueCount--;
                manager->queueLesen--;
            }
        }

        printf("\nGelesene Daten von Record %d: ", id);
        for (int i = 0; i < infoAlloc->length; i++) {
            printf("%d ", getDataFromTable.data[i]);
        }
      
    }
    else
    {
        // Schreibe den Record im NVM-Speicher
        int start = infoAlloc->start;
        for (int i = 0; i < infoAlloc->length; i++) {
            manager->nvmData[start + i] = record->data[i];
        }

        printf("Record geschrieben mit der ID: %d\n", id);
        
        for (int i = 0; i <= manager->queueCount; i++) {
            
            if (manager->queue[i] == id) {
                manager->queue[i] = 0;
                manager->queueCount--;
                manager->queueSchreiben--;
            }
        }
    }

    return callback;
}
