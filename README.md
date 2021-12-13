# esp32_eeprom32_rotate_example
EEPROM32_Rotate library works well for PlatformIO but face some issues at Arduino. Some adjust made to work at Arduino IDE well

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
