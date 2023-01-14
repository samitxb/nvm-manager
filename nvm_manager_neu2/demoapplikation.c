#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

void demoapplikation(NVMManager* manager) {
    // Teste NVM_AddNewRecord
    NVMRecord record1;
    record1.header.id = 0;
    record1.header.length = 10;
    int id1 = NVM_AddNewRecord(manager, &record1, false, false);
    printf("Record hinzugefügt mit ID: %d\n", id1);

    // Teste NVM_AsyncWriteRecord
    NVMRecord record2;
    record2.header.length = 20;
    int id2 = NVM_AddNewRecord(manager, &record2, false, false);
    printf("Record hinzugefügt mit ID: %d\n", id2);
    NVM_AsyncWriteRecord(manager, id2, NULL, &record2, NULL);

    // Teste NVM_AsyncReadRecord
    NVMRecord readRecord;
    NVM_AsyncReadRecord(manager, id2, &readRecord);
    printf("Record gelesen mit ID: %d\n", id2);

    // Teste NVM_DeleteRecord
    NVM_DeleteRecord(manager, id1);
    printf("Record gelöscht mit ID: %d\n", id1);

    // Teste NVM_ReorganizeRecords
    NVM_ReorganizeRecords(manager);
    printf("Records reorganisiert\n");
}