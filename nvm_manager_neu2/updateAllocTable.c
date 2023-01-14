#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

void updateAllocTable(NVMManager* manager) {
    int i, currentPos = 0;
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        // Überprüfe, ob der aktuelle Eintrag in der Allocationstabelle verwendet wird
        if (manager->allocTable[i].used) {
            // Aktualisiere die Startposition des Eintrags in der Allocationstabelle
            manager->allocTable[i].start = currentPos;
            // Erhöhe currentPos um die Länge des Eintrags
            currentPos += manager->allocTable[i].length;
        }
    }
}