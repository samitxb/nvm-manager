#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

int NVM_ReorganizeRecords(NVMManager* manager) {
    int i, j;
    // Erstelle temporäres NVM-Speicher-Array
    unsigned char tempNVM[NVM_SIZE];
    memset(tempNVM, 0, NVM_SIZE);
    int currentPos = 0;
    // Iteriere durch den AllocTable
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        // Überprüfe im AllocTable, ob der aktuelle Record verwendet wird
        if (manager->allocTable[i].used) {
            // Kopiere den Record-Datenblock in den temporären NVM-Speicher
            for (j = 0; j < manager->allocTable[i].length; j++) {
                tempNVM[currentPos + j] = manager->nvmData[manager->allocTable[i].start + j];
            }
            // Aktualisiere die Startposition des Records im AllocTable
            manager->allocTable[i].start = currentPos;
            // Erhöhe currentPos um die Länge des Records
            currentPos += manager->allocTable[i].length;
        }
    }
    //Überschreibe den originalen NVM_Speicher mit dem überarbeitetem/reorganisiertem Speicher
    for (i = 0; i < NVM_SIZE; i++) {
        manager->nvmData[i] = tempNVM[i];
    }
    return 0;
}
