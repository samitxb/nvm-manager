#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVM_SIZE 1014 
#define NVM_HEADER_SIZE 4
#define NVM_BLOCK_SIZE 16
#define NVM_NUM_BLOCKS (NVM_SIZE / NVM_BLOCK_SIZE)
#define NVM_NUM_RECORDS 64

#define NVM_QUEUE_SIZE 100

typedef struct {
	int id;
	unsigned char* data;

}NVM_QueueEntry;


// Struktur für die Verwaltung von NVM Records
typedef struct {
    int id; // ID des Records
    int start; // Startposition des Records im NVM-Speicher
    int length; // Länge des Records
    int used; // Flag, ob der Record genutzt wird
    int readonly; // Flag, ob der Record schreibgeschützt ist
    int redundant; // Flag, ob der Record redundant gespeichert wird
    int redundancy_start; // Startposition des Redundanzblocks
    int valid; // Flag, ob der Record valide ist
} NVMRecord;

// Verwaltungsblock für NVM Records
NVMRecord nvm_records[NVM_SIZE];

// NVM-Speicher
unsigned char nvm_data[NVM_SIZE];