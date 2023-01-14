#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NVM_SIZE 1014 
#define NVM_HEADER_SIZE 4
#define ALLOC_TABLE_SIZE 100
#define QUEUE_SIZE 100

// Struktur f�r den Header eines NVM-Records
typedef struct {
    int id; // ID des Records
    int length; // L�nge des Records
} NVMRecordHeader;

// Struktur f�r einen NVM-Record
typedef struct {
    NVMRecordHeader header; // Header des Records
    unsigned char checksum; // Checksumme des Records
    unsigned char data[NVM_SIZE];
} NVMRecord;

// Struktur f�r die Verwaltung von NVM Records+
typedef struct {
    int id; // ID des Records
    int start; // Startposition des Records im NVM-Speicher
    int length; // L�nge des Records
    bool used; // Flag, ob der Record genutzt wird
    bool readonly; // Flag, ob der Record schreibgesch�tzt ist
    bool redundant; // Flag, ob der Record redundant gespeichert wird
    int redundancy_start; // Startposition des Redundanzblocks
    bool valid; // Flag, ob der Record valide ist
    unsigned char checksum;
} NVMRecordInfo;

typedef void (*NVMWriteCallback)(int status);

// Struktur f�r den NVM-Manager
typedef struct {
    NVMRecordInfo allocTable[ALLOC_TABLE_SIZE]; // Allokationstabelle
    unsigned char nvm_data[NVM_SIZE]; // NVM-Speicher
    int queue[QUEUE_SIZE]; // Asynchrone Schreib-/Lesewarteschlange
    int queueStart; // Startposition der Warteschlange
    int queueEnd; // Endposition der Warteschlange
    int queueCount; // Anzahl der Eintr�ge in der Warteschlange
    NVMWriteCallback writeCallbacks[QUEUE_SIZE];
    int queueRecords[];
} NVMManager;

