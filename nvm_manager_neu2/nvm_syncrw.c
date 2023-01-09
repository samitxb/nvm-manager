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


/*
int NVM_SyncWriteRecord(NVMManager* manager, int id, NVMRecord* record) {

    NVMRecordInfo* info = &manager->allocTable[id];
    if (!info->used || info->readonly) {
        return -1;
    }

    // Berechne Checksumme
    record->checksum = calc_lrc(record->data, record->header.length);
    printf("CHecksum Write= %d\n", record->checksum);

    // Schreibe Record
    if (info->redundant) {
        // Schreibe Record zweimal hintereinander
        memcpy(&manager->nvm_data[info->start], record, info->length);
        memcpy(&manager->nvm_data[info->redundancy_start], record, info->length);

        // Vergleiche Checksummen
        NVMRecord record1;
        memcpy(&record1, &manager->nvm_data[info->start], info->length);
        NVMRecord record2;
        memcpy(&record2, &manager->nvm_data[info->redundancy_start], info->length);
        if (record1.checksum != record2.checksum) {
            // Checksummen sind unterschiedlich
            return -1;
        }
    }
    else {
        // Schreibe Record einmal
        memcpy(&manager->nvm_data[info->start], record, info->length);
    }

    // Markiere Record als valide
    info->valid = 1;
    //NVMRecordInfo.valid = 1;

    return 0;
}
*/


// Synchrones Schreiben eines Records
void NVM_SyncWriteRecord2(NVMManager* manager, int id, unsigned char* data) {

    NVMRecordInfo* info = &manager->allocTable[id];
    // Prüfe, ob der Record existiert und schreibbar ist
    if (id < 0 || id >= NVM_SIZE || !info->used || info->readonly) {
        printf("Ungültige ID oder Record ist schreibgeschützt\n");
        return;
    }

    // Berechne Checksumme
    int lrc = calc_lrc(data, info->length);
    printf("CHecksum Write2= %d\n", lrc);

    // Schreibe den Record im NVM-Speicher
    int start = info->start;
    //manager->nvm_data[start] = id;
    //manager->nvm_data[start + 1] = info->length;
    for (int i = 0; i < info->length; i++) {
        manager->nvm_data[start /*+ NVM_HEADER_SIZE*/  + i] = data[i];
    }
    //manager->nvm_data[start /*+ NVM_HEADER_SIZE*/ + info->length] = lrc;


    // Wenn der Record redundant gespeichert werden soll, speichere ihn noch einmal hintereinander ab
    if (info->redundant) {
        start = info->start + info->length + NVM_HEADER_SIZE + 1;
        manager->nvm_data[start] = id;
        manager->nvm_data[start + 1] = info->length;
        for (int i = 0; i < info->length; i++) {
            manager->nvm_data[start + NVM_HEADER_SIZE + i] = data[i];
        }
        manager->nvm_data[start + NVM_HEADER_SIZE + info->length] = lrc;
    }

    // Setze das Redundanzstart-Flag und das Validitätsflag
    info->redundancy_start = start;
    info->valid = 1;
    info->checksum = lrc;
}


/*// Schreibt einen Record synchron
void NVM_SyncWriteRecord(int id, unsigned char* data) {

    if (id < 0 || id >= NVM_NUM_RECORDS || !nvm_records[id].used || !nvm_records[id].rw) {
        printf("Error: Invalid record ID or record is not writable\n");
        return;
    }

    // LRC berechnen
    unsigned char lrc = calc_lrc(data, nvm_records[id].length);

    // Record schreiben
    memcpy(&nvm_storage[nvm_records[id].start_pos], data, nvm_records[id].length);
    if (nvm_records[id].redundant) {
        memcpy(&nvm_storage[nvm_records[id].redundancy_start_pos], data, nvm_records[id].length);
    }
    // Vergleiche LRCs
    if (nvm_records[id].redundant) {
        unsigned char lrc1 = calc_lrc(data, nvm_records[id].length);
        unsigned char lrc2 = calc_lrc(&nvm_storage[nvm_records[id].redundancy_start_pos], nvm_records[id].length);
        if (lrc1 != lrc2) {
            printf("Error: LRC check faailed\n");
            nvm_records[id].valid = 0;
            return;
        }
    }

    nvm_records[id].valid = 1;
}
*/


/*
char calculateLRC(unsigned char* data) {
    int length = sizeof(&data) / sizeof(&data[0]);
    printf("Length: %d\n", length);


    // Der Wert der LRC
    char lrc = 0;

    // Berechne die LRC durch XOR aller Zahlen im Array
    for (int i = 0; i < length; i++) {
        lrc ^= data[i];
    }

    return lrc;
}
*/


/*// Berechnet die Checksumme eines Records (LRC)
unsigned char NVM_CalculateChecksum(NVMRecord* record) {
    unsigned char checksum = 0;
    int i;
    for (i = 0; i < record->header.length; i++) {
        checksum ^= record->data[i];
    }
    return checksum;
}

    // Berechne das LRC über die Daten

unsigned char calc_checksum(unsigned char* data) {
    unsigned char lrc = 0;
    for (int i = 0; i < sizeof(data); i++) {
        lrc ^= data[i];
    }

    return lrc;
}*/



