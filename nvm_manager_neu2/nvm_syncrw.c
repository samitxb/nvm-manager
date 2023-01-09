#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Berechnet die LRC (byteweise XOR Verknüpfung aller Datenbytes) eines Records
unsigned char calc_lrc(unsigned char* data, int data_size) {
    unsigned char lrc = 0;
    for (int i = 0; i < data_size; i++) {
        lrc ^= data[i];
    }
    return lrc;
}


// Synchrones Schreiben eines Records
void NVM_SyncWriteRecord2(NVMManager* manager, int id, unsigned char* data) {

    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert und schreibbar ist
    if (id < 0 || id >= NVM_SIZE || !info->used || info->readonly) {
        printf("Ungültige ID oder Record ist schreibgeschützt\n");
        return;
    }
    // Berechne Checksumme
    unsigned char lrc1 = calc_lrc(data, info->length);
    printf("CHecksum Write2= %d\n", lrc1);

    // Schreibe den Record im NVM-Speicher
    int start = info->start;
    for (int i = 0; i < info->length; i++) {
        manager->nvm_data[start + i] = data[i];
    }

    // Wenn der Record redundant gespeichert werden soll, speichere ihn noch einmal hintereinander ab
    if (info->redundant) {
        unsigned char lrc2 = calc_lrc(data, info->length);
        printf("CHecksum Write2 Redundant = %d\n", lrc1);
        if (lrc1 != lrc2) {
            printf("Unterschiedliche LRCs bei Berechnung");
            info->valid = 0;
            return -1;
        }
        start = info->redundancy_start;
        for (int i = 0; i < info->length; i++) {
            manager->nvm_data[start + i] = data[i];
        }
    }

    // Setze das Validitätsflag und die Checksum
    info->valid = 1;
    info->checksum = lrc1;
}

// Synchrones Lesen eines Records
int NVM_SyncReadRecord(NVMManager* manager, int id, unsigned char* data, NVMRecord* record) {
    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert
    if (id < 0 || id >= ALLOC_TABLE_SIZE || !info->used) {
        printf("Ungültige ID oder Record existiert nicht\n");
        return -1;
    }

    //Lese Record aus NVM_Data & schreibe in record.data
    memcpy(record->data, &manager->nvm_data[info->start], info->length);

    unsigned char lrc1 = calc_lrc(record->data, info->length);
    printf("Checksum Read2 %d\n", lrc1);

    if (lrc1 != info->checksum) {
        printf("LRC-Fehler\n");
        return -1;
    }

    if (info->redundant) {
        // Lese redundanten Record
        NVMRecord redundantRecord;
        memcpy(&redundantRecord.data, &manager->nvm_data[info->redundancy_start], info->length);

        unsigned char lrc2 = calc_lrc(redundantRecord.data, info->length);
        printf("Checksum Read2 Redundant %d\n", lrc2);
        // Vergleiche Checksummen
        if (lrc1 != lrc2) {
            // Checksummen sind unterschiedlich
            printf("LRCs im redundant gespeicherten Record unterschiedlich\n");
            return -1;
        }
    }

    return 0;
}


