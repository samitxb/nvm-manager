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

    printf("\n\n\033[0;36mStart der Demoapplikation \n\033[0;37m\n");
    printf("\033[0;37m");


    // Initialisiere den NVM-Manager
    ret = NVM_Init(&manager);
    assert(ret == 0);
    printf("\033[0;32mInitialisiere den NVM-Manager: CHECK!\033[0;37m\n\n");

    // Teste das Hinzufügen eines neuen Records
    id = NVM_AddNewRecord(&manager, &record, false, false);
    assert(id != -1);
    printf("\033[0;32mHinzufügen eines neuen Records: CHECK!\033[0;37m\n\n");

    // Teste asynchrones Schreiben
    ret = NVM_AsyncWriteRecord(&manager, id, &data, &record, 0);
    assert(ret == 0);
    printf("\033[0;32masynchrones Schreiben: CHECK!\033[0;37m\n\n");

    // Teste asynchrones Lesen
    ret = NVM_AsyncReadRecord(&manager, id, &data, &record, 0);
    assert(ret == 0);
    printf("\033[0;32masynchrones Lesen: CHECK!\033[0;37m\n\n");

    // Teste das synchrone lesen aus dem unbeschriebenen Record
    id = NVM_AddNewRecord(&manager, &record, false, false);
    ret = NVM_SyncReadRecord(&manager, &record);
    assert(ret == -1);
    printf("\033[0;32mlesen aus dem unbeschriebenen Record: CHECK!\033[0;37m\n\n");

    // Teste das synchrone schreiben in den Record
    memcpy(record.data, data, sizeof(data));
    ret = NVM_SyncWriteRecord(&manager, &record);
    assert(ret == 0);
    printf("\033[0;32mschreiben in den Record durch SyncWriteRecord: CHECK!\033[0;37m\n\n");

    // Teste das synchrone lesen aus dem Record
    ret = NVM_SyncReadRecord(&manager, &record);
    assert(ret == 0);
    printf("\033[0;32mlesen aus dem Record durch SyncReadRecord: CHECK!\033[0;37m\n\n");

    // Teste das Löschen eines Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == 0);
    printf("\033[0;32mLöschen eines Records: CHECK!\033[0;37m\n\n");

    // Teste das Hinzufügen eines schreibgeschützten Records
    id = NVM_AddNewRecord(&manager, &record, true, false);
    memcpy(record.data, data, sizeof(data));
    NVM_SyncWriteRecord(&manager, &record);
    assert(id != -1);
    printf("\033[0;32mHinzufügen eines schreibgeschützten Records: CHECK!\033[0;37m\n\n");

    // Teste das Überschreiben eines schreibgeschützen Records
    memcpy(record.data, data, sizeof(data));
    ret = NVM_SyncWriteRecord(&manager, &record);
    assert(ret == -1);
    printf("\033[0;32mÜberschreiben eines schreibgeschützen Records: CHECK!\033[0;37m\n\n");

    // Teste das Löschen eines schreibgeschützten Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == -1);
    printf("\033[0;32mLöschen eines schreibgeschützten Records: CHECK!\033[0;37m\n\n");

    // Teste das Hinzufügen eines redundanten Records
    id = NVM_AddNewRecord(&manager, &record, false, true);
    memcpy(record.data, data, sizeof(data));
    NVM_SyncWriteRecord(&manager, &record);
    assert(id != -1);
    printf("\033[0;32mHinzufügen eines redundanten Records: CHECK!\033[0;37m\n\n");

    // Teste das Löschen eines redundanten Records
    ret = NVM_DeleteRecord(&manager, &record);
    assert(ret == 0);
    printf("\033[0;32mLöschen eines redundanten Records: CHECK!\033[0;37m\n\n");

    // Teste das Reorganisieren der Records
    ret = NVM_ReorganizeRecords(&manager);
    assert(ret == 0);
    printf("\033[0;32mReorganisieren der Records: CHECK!\033[0;37m\n\n");

    // Teste das Hinzufügen von Records, bis kein Platz mehr ist
    int i;
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        id = NVM_AddNewRecord(&manager, &record, false, false);
        if (id == -1) {
            break;
        }
    }
    assert(i == ALLOC_TABLE_SIZE-2);
    printf("\033[0;32mHinzufügen von Records, bis kein Platz mehr ist: CHECK!\033[0;37m\n\n");



    //Teste Warteschlange voll

     return 0;
}