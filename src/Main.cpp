
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
#define MCP 0x68 //I2C

MCP3424 mcp3424(MCP);

Joystick_ joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
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

unsigned int leftBrakeMax = 0;
unsigned int rightBrakeMax = 0;
long yawMin = 0;
long yawMax = 0;
long temp = 0;
unsigned int leftBrakeMin = 0;
unsigned int rightBrakeMin = 0;

long channelYaw;
long channelLeftBrk;
long channelRightBrk;

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

joystick.begin();


/*
 * Calibration variables are being read fm EEPROM on setup
 * */

readEEPROM(leftBrakeMax,rightBrakeMax,yawMin,yawMax,temp,leftBrakeMin,rightBrakeMin);

joystick.setRzAxisRange(yawMin,yawMax);
joystick.setRxAxisRange(leftBrakeMin,leftBrakeMax);
joystick.setRyAxisRange(rightBrakeMin,rightBrakeMax);

}

void loop() {
    mcp3424.Configuration(1,14,1,1);
    channelYaw = mcp3424.Measure();
    mcp3424.Configuration(2,12,1,1);
    channelRightBrk = mcp3424.Measure();
    mcp3424.Configuration(3,12,1,1);
    channelLeftBrk = mcp3424.Measure();

    // get filtered ADC values and set Axis
    joystick.setRzAxis(channelYaw);  // Yaw
    joystick.setRxAxis(channelLeftBrk);  // LeftBrake
    joystick.setRyAxis(channelRightBrk);  // RightBrake

    if (digitalRead(JUMPER) == LOW) {
        calibration(joystick, leftBrakeMax, rightBrakeMax, yawMin, yawMax, temp, leftBrakeMin, rightBrakeMin);
    }
}