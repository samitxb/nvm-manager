#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Berechnet die LRC (byteweise XOR Verknüpfung aller Datenbytes) eines Records
unsigned char NVM_CalculateChecksum(unsigned char* data, int data_size) {
    unsigned char checksum = 0;
    for (int i = 0; i < data_size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}


// Synchrones Schreiben eines Records
int NVM_SyncWriteRecord(NVMManager* manager, NVMRecord* record, unsigned char* data) {

    int id = record->header.id;
    NVMRecordInfo* info = &manager->allocTable[id];

    // Prüfe, ob der Record existiert und schreibbar ist
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !info->used || info->readonly) {
        printf("Ungueltige ID oder Record ist schreibgeschuetzt\n");
        return -1;
    }
    // Berechne Checksumme
    unsigned char checksum1 = NVM_CalculateChecksum(data, info->length);
    record->checksum = checksum1;

    // Schreibe den Record im NVM-Speicher
    int start = info->start;
    for (int i = 0; i < info->length; i++) {
        manager->nvmData[start + i] = data[i];
    }

    // Wenn der Record redundant gespeichert werden soll, speichere ihn noch einmal hintereinander ab
    if (info->redundant) {
        unsigned char checksum2 = NVM_CalculateChecksum(data, info->length);
        if (checksum1 != checksum2) {
            printf("Unterschiedliche LRCs bei Berechnung!\n");
            info->valid = 0;
            return -1;
        }
        start = info->redundancyStart;
        for (int i = 0; i < info->length; i++) {
            manager->nvmData[start + i] = data[i];
        }
    }

    // Setze das Validitätsflag und die Checksum
    info->valid = 1;
    info->checksum = checksum1;

    //Setzt falls nötig das endgültige readonly flag
    info->readonly = info->readonlyFirst;

    printf("Record & Daten mit der ID: %d erfolgreich in den NVM-Speicher geschrieben!\n", id);
    return 0;
}

// Synchrones Lesen eines Records
int NVM_SyncReadRecord(NVMManager* manager, NVMRecord* record) {
    int id = record->header.id;
    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !info->used) {
        printf("Ungültige ID oder Record existiert nicht\n");
        return -1;
    }

    //Lese Record aus NVM_Data & schreibe in record.data
    memcpy(record->data, &manager->nvmData[info->start], info->length);
    unsigned char checksum1 = NVM_CalculateChecksum(record->data, info->length);

    if (checksum1 != info->checksum) {
        printf("Checksummen-Fehler\n");
        return -1;
    }

    if (info->redundant) {
        // Lese redundanten Record
        NVMRecord redundantRecord = {
            .header.id = 0,
            .header.length = 0
        };
        memcpy(&redundantRecord.data, &manager->nvmData[info->redundancyStart], info->length);

        unsigned char checksum2 = NVM_CalculateChecksum(redundantRecord.data, info->length);
        // Vergleiche Checksummen
        if (checksum1 != checksum2) {
            // Checksummen sind unterschiedlich
            printf("Checksummen im redundant gespeicherten Record unterschiedlich\n");
            return -1;
        }
    }

    printf("Gelesene Daten von Record %d:\n", id);
    for (int i = 0; i < info->length; i++) {
        printf("%d ", record->data[i]);
    }
    printf("\n");
    return 0;
}



