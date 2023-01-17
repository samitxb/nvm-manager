#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"
#include "nvm_del.h"
#include "nvm_syncrw.h"
#include "nvm_asyncrw.h"
#include "nvm_reorganize.h"

int NVM_demoapplikation() {
    NVMManager manager;
    int id;
    int ret;    //Return der Funktionen gekürzt auf "ret" weil "return" reserviert ist
    unsigned char data[] = { 1, 2, 3, 4, 5 };
    NVMRecord record = { .header.length = sizeof(data) };

    printf("\n\n\033[0;36m Start der Demoapplikation \n\033[0;37m");
    printf("\033[0;37m");

    // Initialisiere den NVM-Manager
    ret = NVM_Init(&manager);
    assert(ret == 0);

    // Teste das Hinzufügen eines neuen Records
    id = NVM_AddNewRecord(&manager, &record, false, false);
    assert(id != -1);

    // Teste das schreiben in den Record durch SyncWriteRecord
    memcpy(record.data, data, sizeof(data));
    ret = NVM_SyncWriteRecord(&manager, &record);
    assert(ret == 0);

    // Teste das lesen aus dem Record durch SyncReadRecord
    ret = NVM_SyncReadRecord(&manager, &record);
    assert(ret == 0);

    // Teste das Löschen eines Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == 0);

    // Teste das Hinzufügen eines schreibgeschützten Records
    id = NVM_AddNewRecord(&manager, &record, true, false);
    memcpy(record.data, data, sizeof(data));
    NVM_SyncWriteRecord(&manager, &record);
    assert(id != -1);

    // Teste das Löschen eines schreibgeschützten Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == -1);

    // Teste das Überschreiben eines schreibgeschützen Records
    memcpy(record.data, data, sizeof(data));
    ret = NVM_SyncWriteRecord(&manager, &record);
    assert(ret == -1);

    // Teste das Hinzufügen eines redundanten Records
    id = NVM_AddNewRecord(&manager, &record, false, true);
    memcpy(record.data, data, sizeof(data));
    NVM_SyncWriteRecord(&manager, &record);
    assert(id != -1);

    // Teste das Löschen eines redundanten Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == 0);

    // Teste das Reorganisieren der Records
    ret = NVM_ReorganizeRecords(&manager);
    assert(ret == 0);

    // Teste das Hinzufügen von Records, bis kein Platz mehr im NVM-Speicher ist
    int i;
    for (i = 1; i < ALLOC_TABLE_SIZE; i++) {
        id = NVM_AddNewRecord(&manager, &record, false, false);
        if (id == -1) {
            break;
        }
    }
    assert(i == ALLOC_TABLE_SIZE);

    return 0;
}