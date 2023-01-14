#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

void updateAllocTable(NVMManager* manager) {
    int i, currentPos = 0;
    for (i = 0; i < ALLOC_TABLE_SIZE; i++) {
        // �berpr�fe, ob der aktuelle Eintrag in der Allocationstabelle verwendet wird
        if (manager->allocTable[i].used) {
            // Aktualisiere die Startposition des Eintrags in der Allocationstabelle
            manager->allocTable[i].start = currentPos;
            // Erh�he currentPos um die L�nge des Eintrags
            currentPos += manager->allocTable[i].length;
        }
    }
}