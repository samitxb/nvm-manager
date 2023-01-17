#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Liest einen async-Record in der Schreibeschlange
int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {


    NVMRecordInfo* info = &manager->queue[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];

    // Erstelle einen neuen Record
    record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
    if (record == NULL) {
        callback(-1);  // Rufe den callback mit einem Fehlercode auf
        return -1;
    }

    int break7 = 0;

    // QueueStart = 1?
    manager->queue[manager->queueStart] = id; //  ID von Schreiboperation
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount++;
    printf("\nWrite Queue Count:  %d mit ID: ", manager->queueCount);


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

    /*
    NVMRecord queueRecords =
    {
    .header.id = id,
    .header.length = infoAlloc->length,
    .checksum = infoAlloc->checksum,
    };
    */

   

    int breaksss = 0;

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

    // Prüfe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !infoAlloc->used) {
        printf("\nUngueltige ID oder Record existiert nicht in AllocTable\n");
        return -1;
    }


    // QueueStart = 1?

    // Record in Warteschlange einfügen
    manager->queue[manager->queueStart] = -id;     // Für Unterscheidung ob Write oder Read wird id auf -id gesetzt
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount++;
    //  int queueDiff = manager->queueEnd - manager->queueStart;

    printf("\nRead Queue Count:   %d mit ID: \n", manager->queueStart);
    return 0;

}



int NVM_Handler(NVMManager* manager, int id, NVMRecord* record, NVMWriteCallback callback) 
{
    NVMRecordInfo* infoHeaderForQueue = &manager->allocTable[id];
    NVMRecordInfo* queueRecord = &manager->queueRecords[id];
    NVMRecordInfo* infoAlloc = &manager->allocTable[id];
    NVMRecord* dataW = &manager->nvmData[id];
    NVMRecordInfo* queueRecordId = &manager->queue[id];




    int break1 = 0;

    //Mit memcpy aus AllocTable auslesen
    NVMRecord getDataFromTable =
    {
    .header.id = infoHeaderForQueue->id,
    .header.length = infoHeaderForQueue->length,
    .checksum = infoHeaderForQueue->checksum
    };


    int break2 = 0;
    // Überprüfe, ob es sich um einen Lesebefehl handelt
    if (queueRecordId->id < 1)
    {
        // Überprüfe, ob der Record genutzt wird
        if (!infoHeaderForQueue->used) {
            printf("Record wird nicht genutzt\n");
            return -1;
        }


        // Lese Record aus
        memcpy(&getDataFromTable.data, dataW, infoHeaderForQueue->length);

        int break3 = 0;

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
  


        
        for (int i = 0; i < manager->queueCount; i++)
        {
            if (queueRecord->id == manager->queue[i])
            {
                queueRecord->id = 0;
                queueRecord->checksum = 0;
                queueRecord->length = 0;
                queueRecord->readonly = 0;
                queueRecord->readonlyFirst = 0;
                queueRecord->redundancyStart = 0;
                queueRecord->redundant = 0;
                queueRecord->start = 0;
                queueRecord->valid = 0;
                queueRecord->used = 0;
                manager->queue[-i] = 0;
            }
        }
        manager->queueStart = manager->queueStart--;
        manager->queueCount = manager->queueCount--;

        int break5 = 0;
        return callback = 1;
    }

    else
    {

        int break6 = 0;


        int break7 = 0;



        // Schreibe den Record im NVM-Speicher
        int start = queueRecord->start;
        for (int i = 0; i < queueRecord->length; i++) {
            manager->nvmData[start + i] = record->data[i];
        }


        for (int i = 0; i < manager->queueCount; i++)
        {
            if (queueRecord->id == manager->queue[i])
            {
                manager->queue[i] = 0;
            }
        }

        manager->queueStart--;
        manager->queueCount--;
        return callback = 1;

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



