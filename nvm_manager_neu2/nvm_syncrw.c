#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"

// Berechnet die Checksumme eines Records (LRC)
unsigned char NVM_CalculateChecksum(NVMRecord* record) {
    unsigned char checksum = 0;
    int i;
    for (i = 0; i < record->header.length; i++) {
        checksum ^= record->data[i];
    }
    return checksum;
}


