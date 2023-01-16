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

int NVM_demoapplikation() {
    NVMManager manager;
    NVMRecord record;
    int id;
    int ret;    //Return der Funktionen gek�rzt auf "ret" weil "return" reserviert ist
    unsigned char data[] = { 1, 2, 3, 4, 5 };

    // Initialisiere den NVM-Manager
    ret = NVM_Init(&manager);
    assert(ret == 0);

    // Teste das Hinzuf�gen eines neuen Datensatzes
    id = NVM_AddNewRecord(&manager, &record, false, false);
    memcpy(record.data, data, sizeof(data));
    assert(id != -1);

    // Teste das L�schen eines Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    assert(ret == 0);

    // Teste das Hinzuf�gen eines schreibgesch�tzten Datensatzes
    id = NVM_AddNewRecord(&manager, &record, true, false);
    memcpy(record.data, data, sizeof(data));
    assert(id != -1);

    // Teste das L�schen eines schreibgesch�tzten Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    //assert(ret == -1);

    // Teste das Hinzuf�gen eines redundanten Datensatzes
    id = NVM_AddNewRecord(&manager, &record, false, true);
    memcpy(record.data, data, sizeof(data));
    assert(id != -1);

    // Teste das L�schen eines redundanten Datensatzes
    ret = NVM_DeleteRecord(&manager, id);
    assert(ret == 0);

    // Teste das Reorganisieren der Records
    ret = NVM_ReorganizeRecords(&manager);
    assert(ret == 0);

    // Teste das Hinzuf�gen von Datens�tzen, bis kein Platz mehr im NVM-Speicher ist
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