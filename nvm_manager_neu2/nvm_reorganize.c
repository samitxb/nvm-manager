#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"
#include "resizeAllocTable.h"
#include "updateAllocTable.h"

void NVM_ReorganizeRecords(NVMManager* manager) {
    int i, j;

    // Erstelle temporäres NVM-Speicher-Array
    unsigned char tempNVM[NVM_SIZE];
    memset(tempNVM, 0xFF, NVM_SIZE);

    int currentPos = 0;

    // Iteriere durch die Allocationstabelle
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        // Überprüfe, ob der aktuelle Eintrag in der Allocationstabelle verwendet wird
        if (manager->allocTable[i].used) {
            // Kopiere den Record-Datenblock in den temporären NVM-Speicher
            for (j = 0; j < manager->allocTable[i].length; j++) {
                tempNVM[currentPos + j] = manager->nvm_data[manager->allocTable[i].start + j];
            }
            // Aktualisiere die Startposition des Eintrags in der Allocationstabelle
            manager->allocTable[i].start = currentPos;
            // Erhöhe currentPos um die Länge des Eintrags
            currentPos += manager->allocTable[i].length;
        }
    }
    // Kopiere den reorganisierten NVM-Speicher zurück in den originalen NVM-Speicher
    for (i = 0; i < NVM_SIZE; i++) {
        manager->nvm_data[i] = tempNVM[i];
    }
    // rufen Sie die updateAllocTable() Funktion auf, um die Allocationstabelle zu aktualisieren
    updateAllocTable(manager);
}
