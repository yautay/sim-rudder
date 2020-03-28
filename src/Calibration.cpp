#include <EEPROM.h>
#include <Arduino.h>
#include <Joystick.h>
#include <Leds.h>
#include <Adafruit_ADS1015.h>

void readEEPROM(bool debug,unsigned int leftBrakeMax,unsigned int rightBrakeMax,unsigned int yawMin,unsigned int yawMax,unsigned int leftBrakeMin,unsigned int rightBrakeMin) {

    yawMin = (EEPROM.read(0) | (EEPROM.read(1) << 8));
    yawMax = (EEPROM.read(2) | (EEPROM.read(3) << 8));
    leftBrakeMin = (EEPROM.read(4) | (EEPROM.read(5) << 8));
    leftBrakeMax = (EEPROM.read(6) | (EEPROM.read(7) << 8));
    rightBrakeMin = (EEPROM.read(8) | (EEPROM.read(9) << 8));
    rightBrakeMax = (EEPROM.read(10) | (EEPROM.read(11) << 8));

    if (debug){
        for (int x = 10; x > 0; x++){
            Serial.println(".");
            delay(1000);
        }
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
        Serial.print("Wait 10 sec...");
        for (int x = 10; x > 0; x++){
            Serial.print(".");
            delay(1000);
        }
    }

}

void calibration(bool debug, Joystick_ joystick,unsigned int leftBrakeMax,unsigned int rightBrakeMax,unsigned int yawMin,unsigned int yawMax,unsigned int leftBrakeMin,unsigned int rightBrakeMin){

    Adafruit_ADS1115 adafruitAds1115(0x48);
    adafruitAds1115.begin();
    adafruitAds1115.setGain(GAIN_ONE);

    leftBrakeMax=INT16_MIN;
    rightBrakeMax=INT16_MIN;
    yawMin=INT16_MAX;
    yawMax=INT16_MIN;

    // start calibration with brakes in neutral position
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    delay(5000);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    delay(1000);
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    leftBrakeMin = adafruitAds1115.readADC_SingleEnded(2);
    rightBrakeMin = adafruitAds1115.readADC_SingleEnded(1);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    delay(2000);

    if(debug) {
        Serial.println("Calibration 1/2 ... ");
        Serial.print("Left brake min: ");
        Serial.println(leftBrakeMin);
        Serial.print("Right brake min: ");
        Serial.println(rightBrakeMin);
    }

    while (!digitalRead(9)) {

        int pulses = 10;
        int interval = 500; //milis
        unsigned long checkpoint;

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
                if (adafruitAds1115.readADC_SingleEnded(0) < yawMin) {
                    yawMin = adafruitAds1115.readADC_SingleEnded(0);
                }
                if (adafruitAds1115.readADC_SingleEnded(0) > yawMax) {
                    yawMax = adafruitAds1115.readADC_SingleEnded(0);
                }

                if (adafruitAds1115.readADC_SingleEnded(2) > leftBrakeMax) {
                    leftBrakeMax = adafruitAds1115.readADC_SingleEnded(2);
                }

                if (adafruitAds1115.readADC_SingleEnded(1) > rightBrakeMax) {
                    rightBrakeMax = adafruitAds1115.readADC_SingleEnded(1);
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
    // set and save new ranges
    joystick.setRzAxisRange(yawMin,yawMax);
    joystick.setRxAxisRange(leftBrakeMin,leftBrakeMax);
    joystick.setRyAxisRange(rightBrakeMin,rightBrakeMax);

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
        for (int x = 5; x > 0; x++){
            Serial.println(".");
            delay(1000);
        }
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
        Serial.print("Wait 5 sec...");
        for (int x = 5; x > 0; x++){
            Serial.print(".");
            delay(1000);
        }
    }

    ledSong(5,6,7);

}
