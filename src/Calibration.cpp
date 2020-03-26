#include <EEPROM.h>
#include <Arduino.h>
#include <Joystick.h>
#include <MCP3424.h>
//
// Created by yauta on 26.03.2020.
//

long filter(long channel)
{
    long sum=0;
    int i=0;

    for (i=0; i<50; i++)
    {
        sum = sum + channel;
    }

    sum = sum / 50;
    return(sum);
}

void readEEPROM(unsigned int leftBrakeMax,unsigned int rightBrakeMax,long yawMin,long yawMax,long temp,unsigned int leftBrakeMin,unsigned int rightBrakeMin) {

    yawMin = (EEPROM.read(0) | (EEPROM.read(1) << 8));
    yawMax = (EEPROM.read(2) | (EEPROM.read(3) << 8));
    leftBrakeMin = (EEPROM.read(4) | (EEPROM.read(5) << 8));
    leftBrakeMax = (EEPROM.read(6) | (EEPROM.read(7) << 8));
    rightBrakeMin = (EEPROM.read(8) | (EEPROM.read(9) << 8));
    rightBrakeMax = (EEPROM.read(10) | (EEPROM.read(11) << 8));

}

void calibration(Joystick_ joystick,unsigned int leftBrakeMax,unsigned int rightBrakeMax,long yawMin,long yawMax,long temp,unsigned int leftBrakeMin,unsigned int rightBrakeMin){
    MCP3424 mcp3424(0x68);
    leftBrakeMax=INT16_MIN;
    rightBrakeMax=INT16_MIN;
    yawMin=INT16_MAX;
    yawMax=INT16_MIN;
    temp=0;

    // start calibration with brakes in neutral position
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    delay(5000);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    delay(1000);
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    leftBrakeMin = filter(3);
    rightBrakeMin = filter(2);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    delay(5000);

    while (digitalRead(9) == LOW) {

        // capture max/min values

        if (true) {
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(100);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(100);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(100);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(100);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(100);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
        } // blinks 1sec

        temp = filter(1);
        if (temp < yawMin) {
            yawMin = temp;
        }
        if (temp > yawMax) {
            yawMax = temp;
        }

        temp = filter(3);
        if (temp > leftBrakeMax) {
            leftBrakeMax = temp;
        }

        temp = filter(2);
        if (temp > rightBrakeMax) {
            rightBrakeMax = temp;
        }
    }

    // set and save new Ranges
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

    delay(1000);

}
