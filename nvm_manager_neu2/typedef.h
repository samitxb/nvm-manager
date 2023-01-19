#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NVM_SIZE 1014  // Speichergr��e
#define ALLOC_TABLE_SIZE 100 // Allokationstabellengr��e
#define QUEUE_SIZE 100 // Wartenschlangengr��e

// Struktur f�r den Header eines NVM-Records
typedef struct {
    int id; // ID des Records
    int length; // L�nge des Records
} NVMRecordHeader;

// Struktur f�r einen NVM-Record
typedef struct {
    NVMRecordHeader header; // Header des Records
    unsigned char checksum; // Checksumme des Records
    unsigned char data[100]; // Daten des Records
} NVMRecord;

// Struktur f�r die Verwaltung von NVM Records+
typedef struct {
    int id; // ID des Records
    int start; // Startposition des Records im NVM-Speicher
    int length; // L�nge des Records
    bool used; // Flag, ob der Record genutzt wird
    bool readonly; // Flag, ob der Record schreibgesch�tzt ist
    bool readonlyFirst; // Flag, ob der Record schreibgesch�tzt ist
    bool redundant; // Flag, ob der Record redundant gespeichert wird
    int redundancyStart; // Startposition des Redundanzblocks
    bool valid; // Flag, ob der Record valide ist
    unsigned char checksum; // Checksumme des Records
} NVMRecordInfo;


// Struktur f�r den NVM-Manager
typedef struct {
    NVMRecordInfo allocTable[ALLOC_TABLE_SIZE]; // Allokationstabelle
    unsigned char nvmData[NVM_SIZE]; // NVM-Speicher
    int queue[QUEUE_SIZE]; // Asynchrone Warteschlange
    int queueCount; // Anzahl der Eintr�ge in der Warteschlange
    int queueLesen; // Leseoperation
    int queueSchreiben;  // Schreiboperation
    NVMRecordInfo queueRecords[ALLOC_TABLE_SIZE]; // Tabelle f�r Warteschlangeneintr�ge
} NVMManager;
