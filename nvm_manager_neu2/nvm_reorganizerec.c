#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"
#include <cstdint>

void NVM_ReorganizeRecords(struct NVMManager* nvm)
{
    uint32_t current_pos = 0;
    for (uint32_t i = 0; i < nvm->num_records; i++)
    {
        if (!nvm->records[i].used)
        {
            continue;
        }

        if (nvm->records[i].start_pos == current_pos)
        {
            current_pos += nvm->records[i].length;
            if (nvm->records[i].redundancy)
            {
                current_pos += nvm->records[i].length;
            }
            continue;
        }

        // Move data to new location
        for (uint32_t j = 0; j < nvm->records[i].length; j++)
        {
            nvm->memory[current_pos + j] = nvm->memory[nvm->records[i].start_pos + j];
        }
        nvm->records[i].start_pos = current_pos;
        current_pos += nvm->records[i].length;

        // Move redundancy data to new location
        if (nvm->records[i].redundancy)
        {
            for (uint32_t j = 0; j < nvm->records[i].length; j++)
            {
                nvm->memory[current_pos + j] = nvm->memory[nvm->records[i].redundancy_start_pos + j];
            }
            nvm->records[i].redundancy_start_pos = current_pos;
            current_pos += nvm->records[i].length;
        }
    }
}