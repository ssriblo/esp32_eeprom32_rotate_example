/*
 *  This sketch shows sector hoping across reboots
 */

#include "EEPROM32_Rotate.h"

#define PARTITION_SIZE  4096
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
        Serial.printf("Commit %s\n", EEPROMr.write_and_commit(data_array[i], COL_SIZE, PARTITION_SIZE) ? "OK" : "KO");
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
    uint32_t size =  EEPROMr.readBytes(0, &datar, sizeof(datar));
    Serial.println("datar[] are:");
    for(int i=0; i<sizeof(datar); i++){
        Serial.printf(" 0x%02X ", p_datar[i]);
    }
    Serial.println(" ");
    Serial.println("COUNTERs are:");
    for(int i=0; i<SLOTN; i++) {
        Serial.printf("COUNTER %u: 0x%08X\n", i, datar.value[i]);

    }
    if(MAGICVALR != datar.magic_val){
        memcpy(p_datar, &datar_def, sizeof(datar));
        Serial.println("Writ default to EEPROM");
        datar.number = 0x55;
        datar.value[0] = 0x300;
        datar.value[1] = 0x301;
        datar.value[2] = 0x302;
        datar.value[3] = 0x303;
    }
    datar.number += 1;
    datar.value[0] += 1;
    datar.value[1] += 1;
    datar.value[2] += 1;
    datar.value[3] += 1;
    Serial.printf("Commit %s\n", EEPROMr.write_and_commit(p_datar, 32, PARTITION_SIZE) ? "OK" : "KO");
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

}

void setup() {

    // Init DEBUG --------------------------------------------------------------
    Serial.begin(115200);
    delay(500);
    rotate_setup();
//    rotate_test();
}

void loop() {}
