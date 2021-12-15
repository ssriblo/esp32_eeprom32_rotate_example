/*
 *  This sketch shows sector hoping across reboots
 */

#include "EEPROM32_Rotate.h"

//#define PARTITION_SIZE  4096
//#define PARTITION_OFFSET    0xFF0
#define PARTITION_SIZE      0x40 /* 32 for data; 16 for magic byte+crc; 16 spare */
#define PARTITION_OFFSET    0x30 /* 48+16=64 */
#define SLOTN           4
#define MAGICVALR       0x55332299

EEPROM32_Rotate EEPROMr;

typedef struct eeprom_data_str{
//    const char* name[SLOTN] = {"counter_0", "counter_1", "counter_2", "counter_3"};
    uint32_t value[SLOTN];
    uint32_t timestamp;
    uint32_t number;
    uint32_t magic_val;
    uint32_t spare;
} datar_t;
const datar_t datar_def = { {0,0,0,0}, 0,0, MAGICVALR, 0 };
datar_t datar;
uint8_t* p_datar = (uint8_t*) & datar;

uint32_t eeprom32_get_counter(uint8_t ind, uint32_t defaultValue=0) {
    uint32_t ret = defaultValue;
    if(MAGICVALR != datar.magic_val){return ret;}
    return datar.value[ind];
}
uint32_t eeprom32_get_timestamp(uint32_t defaultValue=0) {
    uint32_t ret = defaultValue;
    if(MAGICVALR != datar.magic_val){return ret;}
    return datar.timestamp;
}
uint32_t eeprom32_get_number(uint32_t defaultValue=0) {
    uint32_t ret = defaultValue;
    if(MAGICVALR != datar.magic_val){return ret;}
    return datar.number;
}

void rotate_setup(bool isPrint) {
        // Init EEPROM32_Rotate ----------------------------------------------------
    // Or add them by subtype (it will search and add all partitions with that subtype)
    EEPROMr.add_by_subtype(0x99);
    // Offset where the magic bytes will be stored (last 16 bytes block)
    EEPROMr.offset(PARTITION_OFFSET);
    // Look for the most recent valid data and populate the memory buffer
    EEPROMr.begin(PARTITION_SIZE);
    // -------------------------------------------------------------------------
    uint32_t size =  EEPROMr.readBytes(0, p_datar, sizeof(datar));
    if(isPrint == true) {
        Serial.println("COUNTERs are:");
        for(int i=0; i<SLOTN; i++) {
            Serial.printf("COUNTER %u: 0x%08X\n", i, datar.value[i]);
        }
        Serial.printf("NUMBER: 0x%08X\n", datar.number);
        Serial.printf("TIMESTAMP: 0x%08X\n", datar.timestamp);
    }
    if(MAGICVALR != datar.magic_val){
        memcpy(p_datar, &datar_def, sizeof(datar));
        Serial.println("***** WARNING ***** Write default to EEPROM");
    }
    // How to use:
    //nn = datar.value[j];
    //datar.value[j] = 123;
    Serial.printf("Commit %s\n", EEPROMr.write_and_commit(p_datar, sizeof(datar), PARTITION_SIZE) ? "OK" : "KO");
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

void setup() {

    // Init DEBUG --------------------------------------------------------------
    Serial.begin(115200);
    delay(500);
//    rotate_setup(false);
    rotate_setup(true);
}

void loop() {}
