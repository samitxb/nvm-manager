#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


int NVM_AsyncWriteRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record, NVMWriteCallback callback) {

    NVMRecordInfo* info = &manager->queueRecords[id];

    // Erstelle einen neuen Record
    record = (NVMRecord*)malloc(sizeof(NVMRecordHeader));
    if (record == NULL) {
        callback(-1);  // Rufe den callback mit einem Fehlercode auf
        return -1;
    }

    // Füge Record in Warteschlange ein
    manager->queue[manager->queueStart] = id; //  ID von Schreiboperation
    manager->queueStart = (manager->queueStart + 1) % QUEUE_SIZE;
    manager->queueCount++;



    printf("\nWrite Queue Count:  %d mit ID=%i", manager->queueCount, id);

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

    printf("\nRead Queue Count:   %d mit ID=%i", manager->queueCount, id);

    return 0;

}



void NVM_Handler(NVMManager* manager, int readOrWrite, NVMRecord* record, NVMWriteCallback* callback) {

    NVMRecordInfo* info;

    int getID[ALLOC_TABLE_SIZE];

    for (int i = 0; i < ALLOC_TABLE_SIZE; i++)
    {
        getID[i] = manager->allocTable[i].id;
        printf("%i", getID[i]);
    }

    int infoID = getID[1];
    printf("\n\n%i", infoID);

    // Überprüfe, ob die Warteschlange leer ist
    if (&manager->queueCount == 0) {
        printf("Warteschlange ist leer\n");
        return 0;
    }

    // Überprüfe, ob es sich um einen Lesebefehl handelt
    if (readOrWrite < 0) {
        readOrWrite = infoID;
        info = &manager->allocTable[infoID];
        printf("\n\n%i\n\n", info);

        // Überprüfe, ob der Record genutzt wird
        if (!info->used) {
            printf("Record wird nicht genutzt\n");
            return -1;
        }

        // Alloziere Speicher für den Record
        record = (NVMRecord*)malloc(info->length);
        if (record == NULL) {
            printf("Fehler beim Allozieren des Speichers\n");
            return -1;
        }

        // Lese den Record
        NVM_AsyncReadRecord(&manager, infoID, &record);

        // Überprüfe die Checksumme
        if (record->checksum != NVM_CalculateChecksum(record->data, info->length)) {
            printf("Checksumme stimmt nicht überein\n");
            free(record);
            return -1;
        }

        // Überprüfe, ob der Record redundant gespeichert ist
        if (info->redundant) {
            NVMRecord redundantRecord;
            memcpy(&redundantRecord, &manager->nvm_data[info->redundant], info->length);

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
        // Es handelt sich um einen Schreibbefehl
        NVMRecord* queuedRecord;
        queuedRecord = &manager->queueRecords[infoID];

        // Schreibe den Record
        int status = NVM_AsyncWriteRecord(manager, infoID, queuedRecord->header.id, queuedRecord->header.length, &queuedRecord, 0);
        printf("Schreibstatus: %d\n", status);

        // Rufe den Schreib-Callback auf
        NVMWriteCallback* callback = manager->writeCallbacks[0];








        /*        if (record->data != NULL) {
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
        */

    }
}