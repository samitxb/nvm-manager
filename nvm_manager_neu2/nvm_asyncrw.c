#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "nvm_syncrw.h"
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

    manager->queue[manager->queueSchreiben] = id;
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

    id = manager->queue[manager->queueLesen];     // Für Readoperation
    manager->queueLesen = (manager->queueLesen + 1) % QUEUE_SIZE;

    if (manager->queueLesen != manager->queueSchreiben)
    {
        //Lese Record aus NVM_Data & schreibe in record.data    
        memcpy(record->data, &manager->nvmData[manager->allocTable[id].start], manager->allocTable[id].length);
        infoAlloc->id = manager->queue[manager->queueLesen];
    }

    return callback;
}



int NVM_Handler(NVMManager* manager, int id, NVMRecord* record) 
{
    NVMRecordInfo* queueId = &manager->queue[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];

    int callback = 0;



    int break2 = 0;
    // Überprüfe, ob es sich um einen Lesebefehl handelt
    if (manager->queueSchreiben == 0)
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

        for (int i = 0; i <= manager->queueLesen; i++) {

            if (manager->queue[i] == id) {
                manager->queue[i] = 0;
                manager->queueLesen--;
            }
        }

        printf("Gelesene Daten von Record %d:\n", id);
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
        
        for (int i = 0; i <= manager->queueSchreiben; i++) {
            
            if (manager->queue[i] == id) {
                manager->queue[i] = 0;
                manager->queueSchreiben--;
            }   
        }


        return callback;

        }
}


// NVMRecord* putRecordInQueue = &manager;
//putRecordInQueue = memcpy(putRecordInQueue, &getDataFromTable, sizeof(10));

// Lese den Record
// NVM_AsyncReadRecord(&manager, infoID, getDataFromTable, queuedRecord, 0);

/*
// Überprüfe die Checksumme
if (record->checksum != NVM_CalculateChecksum(record->data, infoHeaderForQueue->length)) {
    printf("Checksumme stimmt nicht überein\n");
    return -1;
}

// Überprüfe, ob der Record redundant gespeichert ist
if (infoHeaderForQueue->redundant) {
    NVMRecord* redundantRecord;
    memcpy(&redundantRecord, &manager->nvmData, infoHeaderForQueue->length);


    // Vergleiche Checksummen
    if (&record->checksum != &redundantRecord->checksum) {

        printf("Checksummen sind unterschiedlich\n");
        return -1;
    }
}
int break4 = 0;
*/
// Verarbeite den gelesenen Record
// Hier kann man z.B. eine Callback-Funktion aufrufen oder den Record weiterverarbeiten
// ...






/*int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {

    NVMRecordInfo* info = &manager->queue[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];

    // Erstelle einen neuen Record
    record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
    if (record == NULL) {
      //  callback(-1);  // Rufe den callback mit einem Fehlercode auf
        return -1; 
    }




    printf("\nWrite Queue Count:  %d mit ID: ", manager->queueCount);

    return 0;
}

// Liest einen async-Record in der Warteschlange
int NVM_AsyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {

    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecordInfo* infoQueue = &manager->queue[id];

    if (manager->queueCount == QUEUE_SIZE) {
        // Warteschlange ist voll
        printf("\nWarteschlange ist voll!");
        return -1;
    }

    /*
    // Prüfe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !infoAlloc->used) {
        printf("\nUngueltige ID oder Record existiert nicht in AllocTable\n");
        return -1;
    }


    //     int queueDiff = manager->queueEnd - manager->queueStart;



printf("\nRead Queue Count:   %d mit ID: ", manager->queueCount);

return 0;

}



int NVM_Handler(NVMManager* manager, int id, NVMRecord* record, NVMWriteCallback* callback) {

    NVMRecordInfo* info = &manager->queue[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecord* queuedRecord;

    int test = infoAlloc;
    int getID[ALLOC_TABLE_SIZE];


    int infoID = &getID[id];
    printf("%i", id);



    // Überprüfe, ob es sich um einen Lesebefehl handelt
    if (id < 0 || id >= NVM_SIZE || infoAlloc->used)
    {
        infoID = id;
        info = &manager->allocTable[infoID];



        // Überprüfe, ob der Record genutzt wird
        if (!infoAlloc->used) {
            printf("Record wird nicht genutzt\n");
            return -1;
        }

        NVMRecord* queuedRecord;
        queuedRecord = &manager->queueRecords[infoID];

#
        // Record in Warteschlange einfügen
        manager->queue[manager->queueEnd] = -id;     // Für Unterscheidung ob Write oder Read
        manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
        manager->queueCount++;


        // Lese den Record
        NVM_AsyncReadRecord(&manager, infoID, queuedRecord->data, queuedRecord, 0);


         // Überprüfe die Checksumme
         if (record->checksum != NVM_CalculateChecksum(record->data, infoAlloc->length)) {
             printf("Checksumme stimmt nicht überein\n");
             return -1;
         }

         // Überprüfe, ob der Record redundant gespeichert ist
         if (infoAlloc->redundant) {
             NVMRecord redundantRecord;
             memcpy(&redundantRecord, &manager->nvmData[infoAlloc->redundant], infoAlloc->length);

             // Vergleiche Checksummen
             if (record->checksum != redundantRecord.checksum) {

                 printf("Checksummen sind unterschiedlich\n");
                 free(record);
                 return -1;
             }
         }

         // Verarbeite den gelesenen Record
         // Hier kann man z.B. eine Callback-Funktion aufrufen oder den Record weiterverarbeiten
         // ...
         NVMWriteCallback* callback = manager->writeCallbacks[infoID];
    }

    else
    {
        NVMRecordInfo* info = &manager->queue[id];
        NVMRecordInfo* infoAlloc = &manager->allocTable[id];




        // Erstelle einen neuen Record
        record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
        if (record == NULL) {
            //  callback(-1);  // Rufe den callback mit einem Fehlercode auf
            return -1;
        }

        // Füge Record in Warteschlange ein
        manager->queue[manager->queueStart] = id; //  ID von Schreiboperation
        manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
        manager->queueCount++;

        // Es handelt sich um einen Schreibbefehl
        NVMRecord* queuedRecord;
        queuedRecord = &manager->queueRecords[infoID];

        // Schreibe den Record
        NVM_AsyncWriteRecord(&manager, infoID, queuedRecord->data, queuedRecord, 0);

        // Rufe den Schreib-Callback auf
        NVMWriteCallback* callback = manager->writeCallbacks[0];








                if (record->data != NULL) {
            // Perform async write to NVM
            // ...
            // Call callback function when write is complete
            NVM_AsyncWriteRecord(&manager, &record->header.id, &record, &record->header.length, 0, callback);
        }
        else {
            // Perform async read from NVM
            // ...
            // Call callback function when read is complete
            unsigned char* data = NVM_AsyncReadRecord(&manager, &record->header.id, &record);
            return data;
        }


    }
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"*/



