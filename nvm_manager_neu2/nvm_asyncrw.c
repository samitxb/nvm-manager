#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {

    NVMRecordInfo* info = &manager->queue[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];

    // Erstelle einen neuen Record
   // record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
    if (record == NULL) {
        callback(-1);  // Rufe den callback mit einem Fehlercode auf
        return -1;
    }

    manager->queue[manager->queueStart] = id; //  ID von Schreiboperation
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount++;
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

    // Pr�fe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !infoAlloc->used) {
        printf("\nUngueltige ID oder Record existiert nicht in AllocTable\n");
        return -1;
    }

    //�berpr�fung ob NVM SPeicher voll ?

    // Record in Warteschlange einf�gen
    manager->queue[manager->queueStart] = -id;     // F�r Unterscheidung ob Write oder Read wird id auf -id gesetzt
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount++;
    //  int queueDiff = manager->queueEnd - manager->queueStart;

    printf("\nRead Queue Count:   %d mit ID: \n", manager->queueStart);
    return 0;

}



int NVM_Handler(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {

    NVMRecordInfo* infoHeaderForQueue = &manager->allocTable[id];
    NVMRecordInfo* infoNVMQueue = &record->data;
    NVMRecordInfo* queuedRecord = &manager->queueRecords;

    int getID[ALLOC_TABLE_SIZE];
    int infoID = id;


    // �berpr�fe, ob es sich um einen Lesebefehl handelt
    if (id < 0 || id >= ALLOC_TABLE_SIZE || infoHeaderForQueue->used)
    {
        infoHeaderForQueue = &manager->allocTable[id]; // die shaise geht blyat

        int queueStartReset = &manager->queueStart;
        //queueStartReset == 0;

        // �berpr�fe, ob der Record genutzt wird
        if (!infoHeaderForQueue->used) {
            printf("Record wird nicht genutzt\n");
            return -1;
        }

        //Mit memcpy aus AllocTable auslesen
        NVMRecord getDataFromTable =
        {
        .header.id = infoHeaderForQueue->id,
        .header.length = infoHeaderForQueue->length,
        .checksum = infoHeaderForQueue->checksum
        };

        memcpy(&getDataFromTable.data, data, infoHeaderForQueue->length);

        int breaks = 0;

        // NVMRecord* putRecordInQueue = &manager;
        //putRecordInQueue = memcpy(putRecordInQueue, &getDataFromTable, sizeof(10));

        // Lese den Record
        // NVM_AsyncReadRecord(&manager, infoID, getDataFromTable, queuedRecord, 0);


        // �berpr�fe die Checksumme
        if (record->checksum != NVM_CalculateChecksum(record->data, infoHeaderForQueue->length)) {
            printf("Checksumme stimmt nicht �berein\n");
            return -1;
        }

        // �berpr�fe, ob der Record redundant gespeichert ist
        if (infoHeaderForQueue->redundant) {
            NVMRecord* redundantRecord;
            memcpy(&redundantRecord, &manager->nvmData, infoHeaderForQueue->length);


            // Vergleiche Checksummen
            if (&record->checksum != &redundantRecord->checksum) {

                printf("Checksummen sind unterschiedlich\n");
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
        infoID = id;


        // Erstelle einen neuen Record
        record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
        if (record == NULL)
        {
            callback(-1); // Rufe den callback mit einem Fehlercode auf
            return -1;
        }

        // Es handelt sich um einen Schreibbefehl
        NVMRecord* queuedRecord;
        queuedRecord = &manager->queueRecords[infoID];

        // Schreibe den Record
        NVM_AsyncWriteRecord(manager, infoID, queuedRecord->data, queuedRecord, 0);

        // Rufe den Schreib-Callback auf
        NVMWriteCallback* callback = manager->writeCallbacks[0];



        if (record->data != NULL) {
            // Perform async write to NVM
            // ...
            // Call callback function when write is complete
            NVM_AsyncWriteRecord(&manager, id, &record, &record->header.length, 0, callback);
            return callback;
        }
        else {
            // Perform async read from NVM
            // ...
            // Call callback function when read is complete
            unsigned char* data = NVM_AsyncReadRecord(&manager, id, queuedRecord->data, &record, callback);
            return callback;
        }

    }
}


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
    // Pr�fe, ob der Record existiert
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



    // �berpr�fe, ob es sich um einen Lesebefehl handelt
    if (id < 0 || id >= NVM_SIZE || infoAlloc->used)
    {
        infoID = id;
        info = &manager->allocTable[infoID];



        // �berpr�fe, ob der Record genutzt wird
        if (!infoAlloc->used) {
            printf("Record wird nicht genutzt\n");
            return -1;
        }

        NVMRecord* queuedRecord;
        queuedRecord = &manager->queueRecords[infoID];

#
        // Record in Warteschlange einf�gen
        manager->queue[manager->queueEnd] = -id;     // F�r Unterscheidung ob Write oder Read
        manager->queueEnd = (manager->queueEnd + 1) % QUEUE_SIZE;
        manager->queueCount++;


        // Lese den Record
        NVM_AsyncReadRecord(&manager, infoID, queuedRecord->data, queuedRecord, 0);


         // �berpr�fe die Checksumme
         if (record->checksum != NVM_CalculateChecksum(record->data, infoAlloc->length)) {
             printf("Checksumme stimmt nicht �berein\n");
             return -1;
         }

         // �berpr�fe, ob der Record redundant gespeichert ist
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

        // F�ge Record in Warteschlange ein
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



