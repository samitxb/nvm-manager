#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


void NVM_ReorganizeRecords(NVMManager* manager) {
    int index = 0;
    //Anlegen eines AllocTables mit der gleichen Größe von nvm_data
    NVMRecordInfo biggerAllocTable[NVM_SIZE];

    for (int i = 0; i < NVM_SIZE; i++) {
        if (&manager->nvm_data[i] != 0xff) {
            manager->nvm_data[index] = manager->nvm_data[i];
            /*
            //Aktualisieren der Daten im AllocTable
            manager->allocTable[index].id = biggerAllocTable[i].id;                 //&manager->biggerAllocTable[i].id;
            manager->allocTable[index].start = biggerAllocTable[i].start;           //i;
            manager->allocTable[index].length = biggerAllocTable[i].length;         //&manager->allocTable[i].length;
            */



            index++;
        }
    }
    for (int i = index; i < NVM_SIZE; i++) {
        manager->nvm_data[i] = 0x00;
        manager->allocTable[i].id = 0;
    }
}

