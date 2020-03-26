
#include "Arduino.h"
#include <Joystick.h>
#include <Wire.h>
#include <MCP3424.h>
#include <EEPROM.h>
#include <Leds.h>
#include <Calibration.h>

#define LED1 5
#define LED2 6
#define LED3 7
#define JUMPER 9
#define GROUND 8

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
        0,
        0,
        false,
        false,
        false,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false);

void setup() {

    /*
 * Pin definitions
 * Arduino no. 8 - ground pin for calibration jumper
 * Arduino no. 9 - state pin for calibration jumper
 * */

pinMode(GROUND,OUTPUT);
pinMode(JUMPER,INPUT_PULLUP);
pinMode(LED1,OUTPUT);
pinMode(LED2,OUTPUT);
pinMode(LED3,OUTPUT);
digitalWrite(GROUND,LOW);

ledSong(LED1,LED2,LED3);

Joystick.begin();

/*
 * Calibration variables are initialized and read fm EEPROM on setup
 * */

unsigned int leftBrakeMax = 0;
unsigned int rightBrakeMax = 0;
unsigned int yawMin = 0;
unsigned int yawMax = 0;
unsigned int temp = 0;
unsigned int leftBrakeMin = 0;
unsigned int rightBrakeMin = 0;

readEEPROM(leftBrakeMax,rightBrakeMax,yawMin,yawMax,temp,leftBrakeMin,rightBrakeMin);

Joystick.setRzAxisRange(yawMin,yawMax);
Joystick.setRxAxisRange(leftBrakeMin,leftBrakeMax);
Joystick.setRyAxisRange(rightBrakeMin,rightBrakeMax);

}

void loop(){
if (digitalRead(JUMPER) == LOW){
    digitalWrite(LED3,HIGH);
} else {
    digitalWrite(LED3,LOW);
}
}