
#include "Arduino.h"
#include <Joystick.h>
#include <Wire.h>
#include <MCP3424.h>
#include <EEPROM.h>

#define JUMPERPIN 9
#define GROUNDPIN 8

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,0,0, false, false, false, true, true, true, false,
                   false, false, false, false);

void setup() {

    /*
 * Pin definitions
 * Arduino no. 8 - ground pin for calibration jumper
 * Arduino no. 9 - state pin for calibration jumper
 * */

pinMode(GROUNDPIN,OUTPUT);
pinMode(JUMPERPIN,INPUT_PULLUP);
digitalWrite(GROUNDPIN,LOW);

Joystick.begin();

/*
 *
 * */

unsigned int leftBrakeMax;
unsigned int rightBrakeMax;
unsigned int yawMin;
unsigned int yawMax;
unsigned int temp;
unsigned int leftBrakeMin;
unsigned int rightBrakeMin;

yawMin = ( EEPROM.read(0) | (EEPROM.read(1) << 8) );
yawMax = ( EEPROM.read(2) | (EEPROM.read(3) << 8) );
leftBrakeMin = ( EEPROM.read(4) | (EEPROM.read(5) << 8) );
leftBrakeMax = ( EEPROM.read(6) | (EEPROM.read(7) << 8) );
rightBrakeMin = ( EEPROM.read(8) | (EEPROM.read(9) << 8) );
rightBrakeMax = ( EEPROM.read(10) | (EEPROM.read(11) << 8) );

Joystick.setRzAxisRange(yawMin,yawMax);
Joystick.setRxAxisRange(leftBrakeMin,leftBrakeMax);
Joystick.setRyAxisRange(rightBrakeMin,rightBrakeMax);

}

void loop(){

}