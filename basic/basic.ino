/*
 *  This sketch shows sector hoping across reboots
 */

#include "EEPROM32_Rotate.h"

EEPROM32_Rotate EEPROMr;

void setup() {

    // Init DEBUG --------------------------------------------------------------

    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println();

    // Init EEPROM32_Rotate ----------------------------------------------------

    // You can add partitions manually by name
    uint8_t ret = EEPROMr.add_by_name("eeprom");
    Serial.print("eeprom add result: ");
    Serial.println(ret);
    ret = EEPROMr.add_by_name("eeprom2");
    Serial.print("eeprom2 add result: ");
    Serial.println(ret);

    // Or add them by subtype (it will search and add all partitions with that subtype)
    //EEPROMr.add_by_subtype(0x99);

    // Offset where the magic bytes will be stored (last 16 bytes block)
    bool bret = EEPROMr.offset(0xFF0);
    Serial.print("offset result: ");
    Serial.println(bret);



    // Look for the most recent valid data and populate the memory buffer
    EEPROMr.begin(4096);

    // -------------------------------------------------------------------------

    uint8_t data;

    Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
    Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
    Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
    Serial.printf("Data      : 0x%02X\n", data = EEPROMr.read(0));

    Serial.println();
    Serial.printf("Writing 0x%02X to data\n", (data/2) + 1);
    Serial.println();

    EEPROMr.write(0, (data/2) + 1);
    Serial.printf("Commit %s\n", EEPROMr.commit() ? "OK" : "KO");

    Serial.printf("Position 0: 0x%02X\n", EEPROMr.read(0));
    Serial.printf("Position 1: 0x%02X\n", EEPROMr.read(1));
    Serial.printf("Position 2: 0x%02X\n", EEPROMr.read(2));
    Serial.printf("Data      : 0x%02X\n", data = EEPROMr.read(0));

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
}

void loop() {}
