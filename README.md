# esp32_eeprom32_rotate_example
EEPROM32_Rotate library based on this original code: 
- https://github.com/xoseperez/eeprom32_rotate

Due to EEPROM library issue it does not works more

EEPROM.h and EEPROM.cpp renamed to EEPROM32.h and EEPROM32.cpp to avoid use global broken library

The issue is that original code does not switch partitionals at commit() method. Now this method does not used and instead added method write_and_commit(...)

How to add partitilon CSV file:
* New CSV file: eeprom32_rotate.csv
* Add to board.txt file at ~/arduino-1.8.16/portable/packages/esp32/hardware/esp32/2.0.1 (this is for Linux portable installation. Path may be vary at different OS) following:
```
####esp32.upload.maximum_data_size=327680
esp32.upload.maximum_data_size=4194304
…
esp32.menu.PartitionScheme.eeprom32_rotate=EEPROM32 ROTATE APP (default with eeprom_1/eeprom_2)
esp32.menu.PartitionScheme.eeprom32_rotate.build.partitions=eeprom32_rotate
esp32.menu.PartitionScheme.eeprom32_rotate.upload.maximum_size=4194304
```

How to erase all flash to start partitionals from scratch:
* esptool.py -b 921600 --chip esp32 --port /dev/ttyUSB0 erase_flash

How to check flash partitions data:
* esptool.py -b 921600 --chip esp32 --port /dev/ttyUSB0 read_flash  0x00000 0x400000 flash_4M.bin

Partition table example at CSV format:
```
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x140000,
app1,     app,  ota_1,   0x150000,0x140000,
eeprom1,  data, 0x99,    0x290000,0x1000,
eeprom2,  data, 0x99,    0x291000,0x1000,
eeprom3,  data, 0x99,    0x292000,0x1000,
eprom4,  data, 0x99,    0x293000,0x1000,
eeprom5,  data, 0x99,    0x294000,0x1000,
spiffs,   data, spiffs,  0x295000,0x16B000,
```

How to change partition table at Arduino IDE

Edit **board.txt** file:
```
<ARDUINO-IDE-FOLDER>/packages/esp32/hardware/esp32/2.0.1/board.txt file

####esp32.upload.maximum_data_size=327680
esp32.upload.maximum_data_size=4194304
…
esp32.menu.PartitionScheme.eeprom32_rotate=EEPROM32 ROTATE APP (default with eeprom_1/eeprom_2)
esp32.menu.PartitionScheme.eeprom32_rotate.build.partitions=eeprom32_rotate
```
CSV partition files stored at folder:
```
.../arduino-1.8.16/portable/packages/esp32/hardware/esp32/2.0.1/tools/partitions
```
Need add new CSV file **eeprom32_rotate.csv** at folder above.