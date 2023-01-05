#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"

int NVM_AddNewRecord(int length, int readonly, int redundant) {
	//Suche nach einem freien Platz im Verwaltungsblock
	int id = -1;
	for (int i = 0; i < NVM_SIZE; i++) {
		if (nvm_records[i].id == -1) {
			printf("Kein Platz mehr im Verwaltungsblock\n");
		}
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
        printf("Kein Platz mehr im NVM-Speicher\n");
        return -1;
    }

    // Füge den neuen Record im Verwaltungsblock und im NVM-Speicher hinzu
    nvm_records[id].id = id;
    nvm_records[id].start = start;
    nvm_records[id].length = length;
    nvm_records[id].used = 1;
    nvm_records[id].readonly = readonly;
    nvm_records[id].redundant = redundant;
    nvm_records[id].redundancy_start = -1;
    nvm_records[id].valid = 1;
    for (int i = start; i < start + length; i++) {
        nvm_data[i] = 0xff;
    }



	return id;
}