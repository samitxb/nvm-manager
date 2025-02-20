#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NVM_SIZE 1014  // Speichergröße
#define ALLOC_TABLE_SIZE 100 // Allokationstabellengröße
#define QUEUE_SIZE 100 // Wartenschlangengröße

// Struktur für den Header eines NVM-Records
typedef struct {
    int id; // ID des Records
    int length; // Länge des Records
} NVMRecordHeader;

// Struktur für einen NVM-Record
typedef struct {
    NVMRecordHeader header; // Header des Records
    unsigned char checksum; // Checksumme des Records
    unsigned char data[100]; // Daten des Records
} NVMRecord;

// Struktur für die Verwaltung von NVM Records+
typedef struct {
    int id; // ID des Records
    int start; // Startposition des Records im NVM-Speicher
    int length; // Länge des Records
    bool used; // Flag, ob der Record genutzt wird
    bool readonly; // Flag, ob der Record schreibgeschützt ist
    bool readonlyFirst; // Flag, ob der Record schreibgeschützt ist
    bool redundant; // Flag, ob der Record redundant gespeichert wird
    int redundancyStart; // Startposition des Redundanzblocks
    bool valid; // Flag, ob der Record valide ist
    unsigned char checksum; // Checksumme des Records
} NVMRecordInfo;


// Struktur für den NVM-Manager
typedef struct {
    NVMRecordInfo allocTable[ALLOC_TABLE_SIZE]; // Allokationstabelle
    unsigned char nvmData[NVM_SIZE]; // NVM-Speicher
    int queue[QUEUE_SIZE]; // Asynchrone Warteschlange
    int queueCount; // Anzahl der Einträge in der Warteschlange
    int queueLesen; // Leseoperation
    int queueSchreiben;  // Schreiboperation
    NVMRecordInfo queueRecords[ALLOC_TABLE_SIZE]; // Tabelle für Warteschlangeneinträge
} NVMManager;
