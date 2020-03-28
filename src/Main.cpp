
#include "Arduino.h"
#include <Joystick.h>
#include <Adafruit_ADS1015.h>
#include <Leds.h>
#include <EEPROM.h>

#define LED1 5
#define LED2 6
#define LED3 7
#define JUMPER 9
#define GROUND 8
#define ADS1115 0x48 //I2C

bool debug = true;

int16_t leftBrakeMax = 0;
int16_t rightBrakeMax = 0;
int16_t yawMin = 0;
int16_t yawMax = 0;
int16_t leftBrakeMin = 0;

int16_t rightBrakeMin = 0;
int16_t channelYaw;
int16_t channelLeftBrk;

int16_t channelRightBrk;

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

void readEEPROM();
void setRanges();
void calibration();

void setup() {

    if (debug) Serial.begin(9600);

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
     * Calibration variables are being read fm EEPROM on start'up
     * */

    readEEPROM();
    setRanges();

}

void loop() {

    channelYaw = adafruitAds1115.readADC_SingleEnded(0);
    channelRightBrk = adafruitAds1115.readADC_SingleEnded(1);
    channelLeftBrk = adafruitAds1115.readADC_SingleEnded(2);

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

    if (!digitalRead(JUMPER)) {
        calibration();
        readEEPROM();
        setRanges();
    }
    delay(25);
}

void readEEPROM(){
    yawMin = (EEPROM.read(0) | (EEPROM.read(1) << 8));
    yawMax = (EEPROM.read(2) | (EEPROM.read(3) << 8));
    leftBrakeMin = (EEPROM.read(4) | (EEPROM.read(5) << 8));
    leftBrakeMax = (EEPROM.read(6) | (EEPROM.read(7) << 8));
    rightBrakeMin = (EEPROM.read(8) | (EEPROM.read(9) << 8));
    rightBrakeMax = (EEPROM.read(10) | (EEPROM.read(11) << 8));

    if (debug){
        for (int x = 3; x > 0; x--){
            Serial.print(".");
            delay(1000);
        }
        Serial.println("");
        Serial.println("EEPROM bits: ");
        for (int x = 0; x < 11; x++){
            Serial.print("Bit index ");
            Serial.print(x);
            Serial.print(" + ");
            Serial.print(x + 1);
            Serial.print(" : ");
            Serial.println(EEPROM.read(x) | (EEPROM.read(++x) << 8));
        }
        Serial.println("#### Summary: ####");
        Serial.print("Left Brake min: ");
        Serial.print(leftBrakeMin);
        Serial.print(" ; Left Brake max: ");
        Serial.println(leftBrakeMax);
        Serial.print("Right Brake min: ");
        Serial.print(rightBrakeMin);
        Serial.print(" ; Right Brake max: ");
        Serial.println(rightBrakeMax);
        Serial.print("Yaw min: ");
        Serial.print(yawMin);
        Serial.print(" ; Yaw max: ");
        Serial.println(yawMax);
        Serial.println("################");
        Serial.print("Wait 3 sec...");
        for (int x = 3; x > 0; x--){
            Serial.print(".");
            delay(1000);
        }
        Serial.println("");
    }
}
void setRanges(){
    joystick.setRzAxisRange(yawMin, yawMax);
    joystick.setRxAxisRange(leftBrakeMin, leftBrakeMax);
    joystick.setRyAxisRange(rightBrakeMin, rightBrakeMax);
}
void calibration(){

    leftBrakeMax = INT16_MIN;
    rightBrakeMax = INT16_MIN;
    yawMin = INT16_MAX;
    yawMax = INT16_MIN;

    // start calibration with brakes in neutral position
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    delay(3000);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    delay(250);
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    leftBrakeMin = adafruitAds1115.readADC_SingleEnded(2);
    rightBrakeMin = adafruitAds1115.readADC_SingleEnded(1);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);

    if(debug) {
        Serial.println("Calibration 1/2 ... ");
        Serial.print("Left brake min: ");
        Serial.println(leftBrakeMin);
        Serial.print("Right brake min: ");
        Serial.println(rightBrakeMin);
    }

    while (!digitalRead(9)) {

        uint8_t pulses = 10;
        uint16_t interval = 500;
        unsigned long checkpoint;
        uint16_t tmp;

        for (int x = 1; x <= pulses; x++){
            checkpoint = millis();
            do {
                if (x % 2 != 0) {
                    digitalWrite(5, HIGH);
                    digitalWrite(6, HIGH);
                    digitalWrite(7, HIGH);
                } else {
                    digitalWrite(5, LOW);
                    digitalWrite(6, LOW);
                    digitalWrite(7, LOW);
                }
                tmp = adafruitAds1115.readADC_SingleEnded(0);
                if (yawMin < 0 || yawMax > tmp){
                    yawMin = tmp;
                }

                tmp = adafruitAds1115.readADC_SingleEnded(0);
                if (yawMax < 0 || yawMax < tmp){
                    yawMax = tmp;
                }

                tmp = adafruitAds1115.readADC_SingleEnded(2);
                if (rightBrakeMax < 0 || rightBrakeMax < tmp){
                    rightBrakeMax = tmp;
                }

                tmp = adafruitAds1115.readADC_SingleEnded(1);
                if (leftBrakeMax < 0 || leftBrakeMax < tmp){
                    leftBrakeMax = tmp;
                }

            } while (checkpoint + (interval / 2) > millis());
        }
    } // capture max/min values

    if (debug){
        Serial.println("Calibration 2/2 ... ");
        Serial.print("Left brake max: ");
        Serial.println(leftBrakeMax);
        Serial.print("Right brake max: ");
        Serial.println(rightBrakeMax);
        Serial.print("Yaw min: ");
        Serial.println(yawMin);
        Serial.print("Yaw max: ");
        Serial.println(yawMax);
    }

    EEPROM.write(0, lowByte(yawMin));
    EEPROM.write(1, highByte(yawMin));
    EEPROM.write(2, lowByte(yawMax));
    EEPROM.write(3, highByte(yawMax));
    EEPROM.write(4, lowByte(leftBrakeMin));
    EEPROM.write(5, highByte(leftBrakeMin));
    EEPROM.write(6, lowByte(leftBrakeMax));
    EEPROM.write(7, highByte(leftBrakeMax));
    EEPROM.write(8, lowByte(rightBrakeMin));
    EEPROM.write(9, highByte(rightBrakeMin));
    EEPROM.write(10, lowByte(rightBrakeMax));
    EEPROM.write(11, highByte(rightBrakeMax));

    if (debug){
        for (int x = 3; x > 0; x--){
            Serial.print(".");
            delay(1000);
        }
        Serial.println("");
        Serial.println("EEPROM bits: ");
        for (int x = 0; x < 11; x++){
            Serial.print("Bit index ");
            Serial.print(x);
            Serial.print(" + ");
            Serial.print(x + 1);
            Serial.print(" : ");
            Serial.println(EEPROM.read(x) | (EEPROM.read(++x) << 8));
        }
        Serial.println("#### Summary: ####");
        Serial.print("Left Brake min: ");
        Serial.print(leftBrakeMin);
        Serial.print(" ; Left Brake max: ");
        Serial.println(leftBrakeMax);
        Serial.print("Right Brake min: ");
        Serial.print(rightBrakeMin);
        Serial.print(" ; Right Brake max: ");
        Serial.println(rightBrakeMax);
        Serial.print("Yaw min: ");
        Serial.print(yawMin);
        Serial.print(" ; Yaw max: ");
        Serial.println(yawMax);
        Serial.println("################");
        Serial.print("Wait 3 sec...");
        for (int x = 3; x > 0; x--){
            Serial.print(".");
            delay(1000);
        }
        Serial.println("");
    }

    ledSong(5,6,7);

}