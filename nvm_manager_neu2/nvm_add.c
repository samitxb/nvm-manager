#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"

int NVM_AddNewRecord(NVMRecordManager* nvm_alloctable,  NVMRecord* record, unsigned char nvm_data[], int length, bool readonly, bool redundant) {
    //Suche nach einem freien Platz im Verwaltungsblock
    int id = -1;
    for (int i = 0; i < NVM_SIZE; i++) {
        if (!nvm_alloctable[i].used) {
            id = i;
            printf("ID: %d\n", id);
            break;
        }
    }
    if (id == -1) {
        printf("Error: Kein Platz mehr im Verwaltungsblock\n");
        return -1;
    }

  //Suche nach einem freien Platz im NVM-Speicher
    int start = -1;
    for (int i = 0; i < NVM_SIZE - length; i++) {
        int free = 1;
        for (int j = i; j < i + length; j++) {
            if (nvm_data[j] != 0xff) {
                free = 0;
                break;
            }
        }
        if (free) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        printf("Error: Kein Platz mehr im NVM-Speicher\n");
        return -1;
    }


    // Füge den neuen Record im Verwaltungsblock und im NVM-Speicher hinzu
    printf("%d, %d, %d, %d, %d \n", id, start, length, readonly, redundant);
    nvm_alloctable[id].id = id;
    nvm_alloctable[id].start = start;
    nvm_alloctable[id].length = length;
    nvm_alloctable[id].used = 1;
    nvm_alloctable[id].readonly = readonly;
    nvm_alloctable[id].redundant = redundant;
    if (redundant) {
        nvm_alloctable[id].redundancy_start = id + length;
    }
    else {
        nvm_alloctable[id].redundancy_start = -1;
    }
    nvm_alloctable[id].valid = 1;


    for (int i = start; i < start + length; i++) {
        nvm_data[i] = 0xff;
    }

	return id;
}