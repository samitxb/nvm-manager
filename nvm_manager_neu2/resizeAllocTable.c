#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

biggerAllocTable* resizeAllocTable(NVMManager* manager) {

    biggerAllocTable newAllocTable[NVM_SIZE];

    int newAllocTableIndex = 0;

    for (int i = 0; i < ALLOC_TABLE_SIZE; i++) {

        for (int j = 0; j < manager->allocTable[i].length; j++)
        {
            newAllocTable[newAllocTableIndex].id = manager->allocTable[i].id;
            newAllocTable[newAllocTableIndex].start = manager->allocTable[i].start;
            newAllocTable[newAllocTableIndex].length = manager->allocTable[i].length;

            newAllocTableIndex++;
        }
    }

    return newAllocTable;
}


void shrinkAllocTable(NVMManager* manager, biggerAllocTable* newAllocTable) {

    int j = 0;

    for (int i = 0; i < NVM_SIZE && j < ALLOC_TABLE_SIZE; i++) 
    {
        manager->allocTable[i].id = newAllocTable[i].id;
        manager->allocTable[i].start = newAllocTable[i].start;
        manager->allocTable[i].length = newAllocTable[i].length;

        j += newAllocTable[i].length;
        
    }

    //Speicher wieder freigeben
    free(newAllocTable);
}