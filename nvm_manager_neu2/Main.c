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
#include "nvm_demoapplikation.h"



int main() {
	// Initialisiere NVM Manager
	//NVMManager manager;
	//NVM_demoapplikation(&manager);

    NVMManager manager;
    NVMRecord record;
    int id;
    int i;
    int ret;

    // Initialisiere den NVM-Manager
    ret = NVM_Init(&manager);
    assert(ret == 0);

    // Teste das Hinzufügen eines neuen Datensatzes
    record.header.length = 2;
    id = NVM_AddNewRecord(&manager, &record, false, false);
    assert(id == 0);

    // Teste das Löschen eines Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    assert(ret == 0);

    // Teste das Hinzufügen eines schreibgeschützten Datensatzes
    record.header.length = 2;
    id = NVM_AddNewRecord(&manager, &record, true, false);

    assert(id == 0);

    // Teste das Löschen eines schreibgeschützten Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    assert(ret == 0);
    printf("\n%d", ret);

    // Teste das Hinzufügen eines redundanten Datensatzes
    record.header.length = 3;
    id = NVM_AddNewRecord(&manager, &record, false, true);
    assert(id == 0);

    // Teste das Löschen eines redundanten Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    assert(ret == 0);

    // Teste das Hinzufügen von Datensätzen, bis kein Platz mehr im NVM-Speicher ist
    //for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
    //    record.header.length = 10;
    //    id = NVM_AddNewRecord(&manager, &record, false, false);
    //    if (id == -1) {
    //        break;
    //    }
    //}
    //assert(i == ALLOC_TABLE_SIZE - 1);

    return 0;
}

