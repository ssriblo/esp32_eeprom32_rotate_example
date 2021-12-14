/*
 *  This sketch shows sector hoping across reboots
 */

#include "EEPROM32_Rotate.h"

EEPROM32_Rotate EEPROMr;

void setup() {

    // Init DEBUG --------------------------------------------------------------

    Serial.begin(115200);
    delay(500);
    Serial.println();
    Serial.println();

    // Init EEPROM32_Rotate ----------------------------------------------------

    // You can add partitions manually by name
    // uint8_t ret = EEPROMr.add_by_name("eeprom1");
    // ret = EEPROMr.add_by_name("eeprom2");
    // ret = EEPROMr.add_by_name("eeprom3");
    // ret = EEPROMr.add_by_name("eeprom4");
    // ret = EEPROMr.add_by_name("eeprom5");

    // Or add them by subtype (it will search and add all partitions with that subtype)
    EEPROMr.add_by_subtype(0x99);

    // Offset where the magic bytes will be stored (last 16 bytes block)
    bool bret = EEPROMr.offset(0xFF0);
    Serial.print("offset result: ");
    Serial.println(bret);



    // Look for the most recent valid data and populate the memory buffer
    EEPROMr.begin(4096);

    // -------------------------------------------------------------------------
    // Serial.printf("[EEPROM] Current partition   : %s\n", EEPROMr.current());
    // delay(1000);
    // Serial.printf("[EEPROM] Dumping data for partition %s\n", "eeprom");
    // EEPROMr.dump(Serial, 0);
    // Serial.printf("[EEPROM] Dumping data for partition %s\n", "eeprom2");
    // EEPROMr.dump(Serial, 1);
    // return;
    uint8_t data;
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
        Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
        Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
        Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
        Serial.printf("Data      : 0x%02X\n", data = EEPROMr.read(0));

        Serial.println();
//        Serial.printf("Writing 0x%02X to data\n", (data) + 1);
        Serial.println();

        // EEPROMr.write(0, (data) + 1);
        // Serial.printf("Commit %s\n", EEPROMr.commit() ? "OK" : "KO");
        Serial.printf("Commit %s\n", EEPROMr.write_and_commit(data_array[i], COL_SIZE, 4096) ? "OK" : "KO");

        Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
        Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
        Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
        Serial.printf("Data      : 0x%02X\n", data = EEPROMr.read(0));
    }

    Serial.flush();
    // delay(1000);
    // Serial.printf("[EEPROM] Dumping data for partition %s\n", EEPROMr.current());
    // EEPROMr.dump(Serial);
    delay(1000);
    
    Serial.printf("[EEPROM] Reserved partitions : %u\n", EEPROMr.size());
    Serial.printf("[EEPROM] Partition size      : %u\n", EEPROMr.length());
    Serial.printf("[EEPROM] Parititions in use  : ");
    for (uint32_t i = 0; i < EEPROMr.size(); i++) {
        if (i>0) Serial.print(", ");
        Serial.print(EEPROMr.name(i));
    }
    Serial.println();
    Serial.printf("[EEPROM] Current partition   : %s\n", EEPROMr.current());

    EEPROMr.begin(4096);

}

void loop() {}
