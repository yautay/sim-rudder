//
// Created by yauta on 26.03.2020.
//
#include <EEPROM.h>

void readEEPROM(unsigned int leftBrakeMax,unsigned int rightBrakeMax,unsigned int yawMin,unsigned int yawMax,unsigned int temp,unsigned int leftBrakeMin,unsigned int rightBrakeMin) {

    yawMin = (EEPROM.read(0) | (EEPROM.read(1) << 8));
    yawMax = (EEPROM.read(2) | (EEPROM.read(3) << 8));
    leftBrakeMin = (EEPROM.read(4) | (EEPROM.read(5) << 8));
    leftBrakeMax = (EEPROM.read(6) | (EEPROM.read(7) << 8));
    rightBrakeMin = (EEPROM.read(8) | (EEPROM.read(9) << 8));
    rightBrakeMax = (EEPROM.read(10) | (EEPROM.read(11) << 8));

};