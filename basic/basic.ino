/*
 *  This sketch shows sector hoping across reboots
 */

#include "EEPROM32_Rotate.h"

#define PARTITION_SIZE  4096

EEPROM32_Rotate EEPROMr;

void rotate_test() {
    #define CYCLE       7
#define COL_SIZE   3
    uint8_t data_array[CYCLE][COL_SIZE] = {
        {0x01, 0x02, 0x03},
        {0x11, 0x12, 0x13},
        {0x21, 0x22, 0x23},
        {0x31, 0x32, 0x33},
        {0x41, 0x42, 0x43},
        {0x51, 0x52, 0x53},
        {0x61, 0x62, 0x63},
    };
    for(int i=0; i<CYCLE; i++){
        Serial.println("********************");
        //Serial.printf_P(PSTR("<<<<<<< free heap memory BEFORE: %d >>>>>>>>\n"), ESP.getFreeHeap());
        Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
        Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
        Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
        Serial.println();
        Serial.printf("Commit %s\n", EEPROMr.write_and_commit(data_array[i], COL_SIZE, 4096) ? "OK" : "KO");
        Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
        Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
        Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
        //Serial.printf_P(PSTR("<<<<<<< free heap memory AFTER: %d >>>>>>>>\n"), ESP.getFreeHeap());
    }
    delay(500);
    Serial.printf("[EEPROM] Reserved partitions : %u\n", EEPROMr.size());
    Serial.printf("[EEPROM] Partition size      : %u\n", EEPROMr.length());
    Serial.printf("[EEPROM] Parititions in use  : ");
    for (uint32_t i = 0; i < EEPROMr.size(); i++) {
        if (i>0) Serial.print(", ");
        Serial.print(EEPROMr.name(i));
    }
    Serial.println();
    Serial.printf("[EEPROM] Current partition   : %s\n", EEPROMr.current());
    EEPROMr.begin(PARTITION_SIZE);

}

void rotate_setup() {
        // Init EEPROM32_Rotate ----------------------------------------------------
    // Or add them by subtype (it will search and add all partitions with that subtype)
    EEPROMr.add_by_subtype(0x99);
    // Offset where the magic bytes will be stored (last 16 bytes block)
    EEPROMr.offset(0xFF0);
    // Look for the most recent valid data and populate the memory buffer
    EEPROMr.begin(PARTITION_SIZE);
    // -------------------------------------------------------------------------
}
void setup() {

    // Init DEBUG --------------------------------------------------------------
    Serial.begin(115200);
    delay(500);
    rotate_setup();
    rotate_test();
}

void loop() {}
