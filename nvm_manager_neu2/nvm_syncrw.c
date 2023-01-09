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
    int lrc = calc_lrc(data, info->length);
    printf("CHecksum Write2= %d\n", lrc);

    // Schreibe den Record im NVM-Speicher
    int start = info->start;
    for (int i = 0; i < info->length; i++) {
        manager->nvm_data[start + i] = data[i];
    }

    // Wenn der Record redundant gespeichert werden soll, speichere ihn noch einmal hintereinander ab
    if (info->redundant) {
        start = info->start + info->length + 1;
        manager->nvm_data[start] = id;
        manager->nvm_data[start + 1] = info->length;
        for (int i = 0; i < info->length; i++) {
            manager->nvm_data[start + i] = data[i];
        }
        manager->nvm_data[start + info->length] = lrc;
    }

    // Setze das Redundanzstart-Flag und das Validitätsflag
    info->redundancy_start = start;
    info->valid = 1;
    info->checksum = lrc;
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

    // Wenn der Record redundant gespeichert wurde, prüfe auch das zweite Kopie
    if (info->redundant) {
        int start = info->redundancy_start;
        if (manager->nvm_data[start] != id || manager->nvm_data[start + 1] != info->length) {
            printf("Ungültiger redundant gespeicherter Record\n");
            return -1;
        }
        unsigned char lrc2 = 0;
        for (int i = 0; i < info->length; i++) {
            lrc2 ^= manager->nvm_data[start + i];
        }
        if (manager->nvm_data[start +  info->length] != lrc2) {
            printf("LRC-Fehler in redundant gespeichertem Record\n");
            return -1;
        }
        if (lrc1 != lrc2) {
            printf("LRCs im redundant gespeicherten Record unterschiedlich\n");
            return -1;
        }
    }

    return 0;
}

/*// Synchron liest einen Record
int NVM_SyncReadRecord(NVMManager* manager, int id, NVMRecord* record) {
    printf("Read ID: %d\n", id);
    NVMRecordInfo* info = &manager->allocTable[id];

    if (!info->used) {
        return -1;
    }

    // Lese Record  evtl & vor record setzen -> dann aber lesezugriffsverletzung
    memcpy(record, &manager->nvm_data[info->start], info->length);

    printf("Checksum Read = %d\n", record->checksum);
    printf("Neu Kalk Read = %d\n", NVM_CalculateChecksum(record));

    // Prüfe Checksumme
    if (record->checksum != NVM_CalculateChecksum(record)) {
        // Checksumme ist falsch
        return -1;

    }

    if (info->redundant) {
        // Lese redundanten Record
        NVMRecord redundantRecord;
        memcpy(&redundantRecord, &manager->nvm_data[info->redundancy_start], info->length);
        // Vergleiche Checksummen
        if (record->checksum != redundantRecord.checksum) {
            // Checksummen sind unterschiedlich
            return -1;
        }
    }

    return 0;
}
*/


/*// Liest einen Record synchron aus
void NVM_SyncReadRecord(int id, unsigned char* data) {
    if (id < 0 || id >= NVM_NUM_RECORDS || !nvm_records[id].used) {
        printf("Error: Invalid record ID\n");
        return;
    }

    // LRC prüfen
    unsigned char lrc = calc_lrc(data, nvm_records[id].length);
    if (lrc != calc_lrc(&nvm_storage[nvm_records[id].start_pos], nvm_records[id].length)) {
        printf("Error: LRC check faiiled\n");
        nvm_records[id].valid = 0;
        return;
    }

    // Vergleiche LRCs von Redundanzblocks
    if (nvm_records[id].redundant) {
        unsigned char lrc1 = calc_lrc(data, nvm_records[id].length);
        unsigned char lrc2 = calc_lrc(&nvm_storage[nvm_records[id].redundancy_start_pos], nvm_records[id].length);
        if (lrc1 != lrc2) {
            printf("Error: Redundancy LRC check failed\n");
            nvm_records[id].valid = 0;
            return;
        }
    }

    // Record auslesen
    memcpy(data, &nvm_storage[nvm_records[id].start_pos], nvm_records[id].length);
    nvm_records[id].valid = 1;
}
*/

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



