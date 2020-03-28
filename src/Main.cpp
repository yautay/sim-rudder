
#include "Arduino.h"
#include <Joystick.h>
#include <Adafruit_ADS1015.h>
#include <Leds.h>
#include <Calibration.h>

#define LED1 5
#define LED2 6
#define LED3 7
#define JUMPER 9
#define GROUND 8
#define ADS1115 0x48 //I2C

int16_t tmp = 0;

Adafruit_ADS1115 adafruitAds1115(ADS1115);

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
unsigned int yawMin = 0;
unsigned int yawMax = 0;
unsigned int leftBrakeMin = 0;
unsigned int rightBrakeMin = 0;

unsigned int channelYaw;
unsigned int channelLeftBrk;
unsigned int channelRightBrk;

bool debug = true;

void setup() {

Serial.begin(9600);

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

adafruitAds1115.begin();
adafruitAds1115.setGain(GAIN_ONE);
joystick.begin();


/*
 * Calibration variables are being read fm EEPROM on srartup
 * */

readEEPROM(debug,leftBrakeMax,rightBrakeMax,yawMin,yawMax,leftBrakeMin,rightBrakeMin);

//joystick.setRzAxisRange(yawMin,yawMax);
//joystick.setRxAxisRange(leftBrakeMin,leftBrakeMax);
//joystick.setRyAxisRange(rightBrakeMin,rightBrakeMax);

joystick.setRzAxisRange(0,INT16_MAX);
joystick.setRxAxisRange(0,INT16_MAX);
joystick.setRyAxisRange(0,INT16_MAX);

}

void loop() {

//    channelYaw = adafruitAds1115.readADC_SingleEnded(0);
//    channelRightBrk = adafruitAds1115.readADC_SingleEnded(1);
//    channelLeftBrk = adafruitAds1115.readADC_SingleEnded(2);

    channelYaw = 0 + tmp;
    channelRightBrk = 0 + tmp;
    channelLeftBrk = 0 + tmp;

    tmp++; //DEBUG
    delay(25); //DEBUG

    if (debug) {
        Serial.print("Yaw : ");
        Serial.print(channelYaw);
        Serial.print(" // Left Brake : ");
        Serial.print(channelLeftBrk);
        Serial.print(" // Right Brake : ");
        Serial.println(channelRightBrk);
    }

    // get filtered ADC values and set Axis
    joystick.setRzAxis(channelYaw);  // Yaw
    joystick.setRxAxis(channelLeftBrk);  // LeftBrake
    joystick.setRyAxis(channelRightBrk);  // RightBrake

    joystick.sendState();

    if (!digitalRead(JUMPER)) {
        calibration(debug,joystick, leftBrakeMax, rightBrakeMax, yawMin, yawMax, leftBrakeMin, rightBrakeMin);
    }

}