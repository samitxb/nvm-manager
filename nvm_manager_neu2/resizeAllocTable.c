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
