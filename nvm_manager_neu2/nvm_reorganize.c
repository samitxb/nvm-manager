#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"
#include "resizeAllocTable.h"
#include "updateAllocTable.h"

void NVM_ReorganizeRecords(NVMManager* manager) {
    int i, j;

    // Erstelle tempor�res NVM-Speicher-Array
    unsigned char tempNVM[NVM_SIZE];
    memset(tempNVM, 0, NVM_SIZE);

    int currentPos = 0;

    // Iteriere durch den AllocTable
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        // �berpr�fe im AllocTable, ob der aktuelle Record verwendet wird
        if (manager->allocTable[i].used) {
            // Kopiere den Record-Datenblock in den tempor�ren NVM-Speicher
            for (j = 0; j < manager->allocTable[i].length; j++) {
                tempNVM[currentPos + j] = manager->nvm_data[manager->allocTable[i].start + j];
            }
            // Aktualisiere die Startposition des Records im AllocTable
            manager->allocTable[i].start = currentPos;
            // Erh�he currentPos um die L�nge des Records
            currentPos += manager->allocTable[i].length;
        }
    }
    //�berschreibe den originalen NVM_Speicher mit dem �berarbeitetem/reorganisiertem Speicher
    for (i = 0; i < NVM_SIZE; i++) {
        manager->nvm_data[i] = tempNVM[i];
    }

    updateAllocTable(manager);
}
